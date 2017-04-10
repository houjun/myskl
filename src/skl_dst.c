
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
# define _PROTO_PUBLIC_INSERT
# define _PROTO_PRIVATE_RAND
# define _PROTO_PRIVATE_DBG

# include "skl_priv.h"



/** ****************************************************************
 *                                                                 *
 * Distrugge il nodo <n> e l'array di puntatori in esso contenuto. *
 * Il puntatore a funzione <dstfunc> serve a cancellare i dati del *
 * nodo <n> ( key e item ) ed e' definito dall'utente              *
 *                                                                 *
 ** ****************************************************************/

static void destroy_node ( MySKL_t l, MySKL_n n,
    void ( * dstdatafunc )( MySKL_n ) )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* destroy the node's data (not node) */

    if ( dstdatafunc != NULL )
        ( *dstdatafunc ) ( n );

    IFDOUBLE( X_FREE ( n->prev ); )
    X_FREE ( n->next );

    /* destroy the node */
    ( *(l->dst) ) ( n );

    /* debug call if enabled */
    X_RETURN_VOID ();
}



/** ****************************************************************
 *                                                                 *
 * Distrugge l'header della lista <l> ponendo il contenuto dello   *
 * stesso <l> a NULL                                               *
 *                                                                 *
 ** ****************************************************************/

static void destroy_hdr ( MySKL_t * l )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    IFDOUBLE (
        X_FREE ( ( *l )->header->prev );
    )

    X_FREE ( ( *l )->header->next );
    X_FREE ( ( *l )->header );

    IFRAND(
        if ( ( *l )->rng_item != NULL )
            RNGfree ( ( *l )->rng_item );
    )

    RW_DESTROY( *l );

    X_FREE ( *l );
    *l = NULL;

    /* debug call if enabled */
    X_RETURN_VOID ();
}



/** *************************************************************
 ** *************************************************************
 +                                                              +
 +                                                              +
 +              FUNZIONI/PROCEDURE PUBBLICHE                    +
 +                                                              +
 +                                                              +
 ** *************************************************************
 ** *************************************************************/

void MySKLdestroy ( MySKL_t * l, void ( * dstdatafunc )( MySKL_n ) )
{
    MySKL_n node, save;

    /* NOTE: this function does not use any mutex since the
     * thread safety, in the case of concurrent runs on the
     * same list <l>, is not guaranteed the same
     */

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_VOID ( *l, DSKIP, NSKIP );
    EXP_BC_X_VOID ( (*l)->dst );

    node = ( *l )->header->next[ 0 ];
    while ( node != NIL ( *l ) )
    {
        save = node->next[ 0 ];
        destroy_node ( *l, node, dstdatafunc );

        node = save;
    }

    destroy_hdr ( l );
    X_RETURN_VOID ();
}
