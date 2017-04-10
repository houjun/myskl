
/***************************************************************************
 *   Copyright (C) 2008 by Andrea Ghersi                                   *
 *   ghangenit@users.sourceforge.net                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

# define _PROTO_PUBLIC_DESTROY
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_DBG
# define _PROTO_PUBLIC_INIT
# define _PROTO_PRIVATE_LC
# define _PROTO_PUBLIC_LC

# include <pthread.h>

# include "skl_priv.h"



/** *************************************************************
 *                                                              *
 * Prototipi, macro e definizioni                               *
 *                                                              *
 ** *************************************************************/

# ifdef THREAD
MySKL_cs lc = { PTHREAD_MUTEX_INITIALIZER, NULL, LC_MSTL, 0, LC_NONE };
# endif

# ifndef THREAD
MySKL_cs lc = { NULL, LC_MSTL, 0, LC_NONE };
# endif

static void destroyAoCL ( void );

# define REALLOC_WARN                                 \
    "Warning: cannot reallocate '%u' bytes in size. " \
    "Reallocation failed! List's being created will " \
    "not be automatically free'd by calling "         \
    "<MySKLfreeListsContainer()>\n"

# define aclsize() ( sizeof( MySKL_t ) * ( lc.max_lists + LC_MSTL ) )
# define X_MESSAGE "trying allocating '%d' bytes for Acl"



/** *************************************************************
 *                                                              *
 * Distrugge l'array che contiene i riferimenti alle liste che  *
 * sono state create. Ogni elemento dell'array viene settato    *
 * automaticamente a NULL. AoCL = Array of Created Lists        *
 *                                                              *
 ** *************************************************************/

static void destroyAoCL ( void )
{
    MySKL_t * a = lc.acl;
    uint i = 0;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    while ( i < lc.lists_cnt )
    {
        /* MySKLdestroy ( a + i, a[ i ]->dst ); */
        MySKLdestroy ( a + i, NULL );
        i++;
    }

    X_FREE ( a );
    X_RETURN_VOID ();
}



/** *************************************************************
 *                                                              *
 * Ridimensiona l'array di liste 'acl'. Viene chiamata quando   *
 * la sua dimensione non e' piu' sufficiente. LC_MSTL = maxnum  *
 * of storable lists                                            *
 *                                                              *
 ** *************************************************************/

static void * allocExtraMemory ()
{
    size_t arrdim = aclsize ();
    MySKL_t * tmp;

    X_ENTER ( THIS_IS_ME );
    X_PRINT ( THIS_IS_ME, ( X_MESSAGE, arrdim ) );

    if ( !( tmp = REALLOC ( lc.acl, arrdim ) ) )
    {
        fprintf ( stderr, REALLOC_WARN, arrdim );
        X_RETURN ( NULL );
    }

    lc.max_lists += LC_MSTL;
    lc.acl = tmp;

    /* debug call if enabled */
    X_RETURN ( lc.acl );
}



/** *************************************************************
 ** *************************************************************
 +                                                              +
 +                                                              +
 +               FUNZIONE/PROCEDURE PRIVATE                     +
 +                  usabili in altri file                       +
 +                                                              +
 +                                                              +
 ****************************************************************
 * **************************************************************/

/** ************************************************************
 *                                                             *
 * Crea una specie di garbace collector, il quale conterra' un *
 * array di liste ( acl = array of created lists ) con al suo  *
 * interno i riferimenti alle liste create.                    *
 *                                                             *
 ** ************************************************************/

bool _createListsContainer ( char mode )
{
    size_t dim;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    lc.mode = mode;
    if ( lc.mode == LC_FULL )
    {
        dim = X_ALLOC ( aclsize (), lc.acl );
        MEM_BC_X_RETV ( lc.acl, dim, 0 );
    }

    /* initialize the mutex
     * mutex_init_default( lc.mtx_lock ); */

    /* debug call if enabled */
    X_RETURN ( 1 );
}



/** ************************************************************
 *                                                             *
 * Distrugge il garbace collector, deallocando l'array dellle  *
 * liste create. Reinizializza le info nella struttura del lc  *
 *                                                             *
 ** ************************************************************/

void _destroyListsContainer ( void )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    if ( lc.acl != NULL )
        destroyAoCL ();

    /* destroy the mutex
     * mutex_destroy( lc.mtx_lock ); */

    lc.max_lists = LC_MSTL;
    lc.lists_cnt = 0;

    lc.mode = LC_NONE;
    lc.acl  = NULL;

    /* debug call if enabled */
    X_RETURN_VOID ();
}



/** *************************************************************
 *                                                              *
 * Aggiunge la lista <l> nell'arry di liste acl, controllando   *
 * che l'array abbia spazio a sufficienza. Altrimenti prova a   *
 * rialloccarlo                                                 *
 *                                                              *
 ** *************************************************************/

void _containerPushAcl ( MySKL_t l )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* l'array di liste deve esistere */
    EXP_BC_VOID ( lc.acl != NULL );

    /* acquire the mutex */
    mutex_lock( lc.mtx_lock );

    if ( lc.lists_cnt >= lc.max_lists )
        if ( !allocExtraMemory () )
        {
            mutex_unlock( lc.mtx_lock );
            X_RETURN_VOID ();
        }

    lc.acl[ lc.lists_cnt++ ] = l;

    mutex_unlock( lc.mtx_lock );
    X_RETURN_VOID ();
}



/** ***********************************************************
 ** ***********************************************************
 +                                                            +
 +                                                            +
 +              FUNZIONI/PROCEDURE PUBBLICHE                  +
 +                                                            +
 +                                                            +
 ** ***********************************************************
 ** ***********************************************************/

void MySKLfreeListsContainer ( void )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    if ( lc.mode == LC_NONE )
        X_RETURN_VOID ();

    _destroyListsContainer ();
    X_RETURN_VOID ();
}



bool MySKLinitListsContainer ( void )
{
    bool ret;

    X_ENTER ( THIS_IS_ME );

    ret = _createListsContainer ( LC_FULL );
    X_RETURN ( ret );
}


void MySKLaddtoListsContainer ( MySKL_t l )
{
    X_ENTER ( THIS_IS_ME );

    _containerPushAcl( l );
    X_RETURN_VOID ();
}
