
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

# define _PROTO_PRIVATE_ITERATOR
# define _PROTO_PUBLIC_ITERATOR
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_DBG

# include "skl_priv.h"

# define FUNC_NOT_AVAIL                                       \
    "Warning: '%s()' not implemented here.\nIt is available " \
    "only with macro 'MYSKL_DOUBLE_LL_TYPE' defined in "      \
    "<skl_defs.h>\n"

# define someSanityChecks( iter, lev )                   \
    ITR_BC_RETV ( iter, NULL );                          \
    EXP_BC_RETV ( lev <= ( *iter )->prevlev && lev >= 1, NULL );



/** ***************************************************************
 *                                                                *
 *  Prende il nodo successivo rispetto all'iteratore <iter> al    *
 *  livello <lev>                                                 *
 *                                                                *
 ** ***************************************************************/

MySKL_n getNext ( MySKL_i iter, uint lev )
{
    MySKL_n node;

    lev--;
    if ( iter->next == NULL ||
         iter->next[ lev ] == NIL ( iter->list ) )
       return NULL;

    if ( iter->term != NULL && iter->next[ lev ] ==
         iter->term )
       return NULL;

    /* acquire the lock */
    mutex_lock( iter->mtx_lock );

    /* node that is being returned */
    node = iter->next[ lev ];

    /* update 'iter->prev' for getPrevNode() */
    iter->prev = iter->next;

    iter->prevlev = iter->next[ lev ]->sz;
    iter->next = node->next;

    mutex_unlock( iter->mtx_lock );
    return node;
}



/** ***************************************************************
 *                                                                *
 *  Come sopra, solo che non utilizza alcun mutex. Utile per la   *
 *  drawList, la search..                                         *
 *                                                                *
 ** ***************************************************************/

MySKL_n getNextNoMutex( MySKL_i iter, uint lev )
{
    MySKL_n node;

    lev--;
    if ( iter->next == NULL ||
         iter->next[ lev ] == NIL ( iter->list ) )
       return NULL;

    if ( iter->term != NULL && iter->next[ lev ] ==
         iter->term )
       return NULL;

    /* node that is being returned */
    node = iter->next[ lev ];

    /* update 'iter->prev' for getPrevNode() */
    iter->prev = iter->next;

    iter->prevlev = iter->next[ lev ]->sz;
    iter->next = node->next;

    return node;
}



/** ***************************************************************
 *                                                                *
 *  Prende il nodo precedente rispetto all'iteratore <iter> al    *
 *  livello <lev>. Usata solo con MYSKL_DOUBLE_LL_TYPE definita    *
 *                                                                *
 ** ***************************************************************/

# ifdef MYSKL_DOUBLE_LL_TYPE
static MySKL_n getPrev ( MySKL_i iter, uint lev )
{
    MySKL_n node = NULL;

    lev--;
    if ( iter->prev[ lev ] == NIL ( iter->list ) )
       return NULL;

    /* acquire the lock */
    mutex_lock( iter->mtx_lock );

    /* node that is being returned */
    node = iter->prev[ lev ];

    /* update 'iter->prev' for getNextNode() */
    iter->next = iter->prev;

    iter->prevlev = iter->prev[ lev ]->sz;
    iter->prev = node->prev;

    mutex_unlock( iter->mtx_lock );
    return node;
}


/** ***************************************************************
 *                                                                *
 *  Come sopra, solo che non utilizza alcun mutex. Al momento non *
 *  e' utilizzata                                                 *
 *                                                                *
 ** ***************************************************************/

static MySKL_n getPrevNoMutex ( MySKL_i iter, uint lev )
{
    MySKL_n node = NULL;

    lev--;
    if ( iter->prev[ lev ] == NIL ( iter->list ) )
       return NULL;

    /* node that is being returned */
    node = iter->prev[ lev ];

    /* update 'iter->prev' for getNextNode() */
    iter->next = iter->prev;

    iter->prevlev = iter->prev[ lev ]->sz;
    iter->prev = node->prev;

    return node;
}

#endif



/** ***************************************************************
 *                                                                *
 *  Setta un iteratore <iter> per la lista <l>. Se <n> == NULL    *
 *  si parte dal primo elemento della lista, altrimenti da <n>    *
 *  L'elemento <n> non viene 'consumato' dalla getNext(...)       *
 *  Non e' possibile avere due iter uguali sulla stessa lista;    *
 *  in questo caso il comportamento e' indefinito, perche' lo     *
 *  stesso mutex viene inizializzato piu' volte                   *
 *                                                                *
 ** ***************************************************************/

void setIterator ( MySKL_t l, MySKL_i iter,
    MySKL_n n, MySKL_n term )
{
    MySKL_n * start;

    mutex_init_default( iter->mtx_lock );
    /* mutex_lock( iter->mtx_lock ); */

    /* set start point */
    start = ( !n ? &l->header : &n );

    /* move forward */
    iter->next = ( *start )->next;

    IFDOUBLE (
        /* move backward */
        iter->prev = ( *start )->prev;
    )

    iter->magic = MYSKL_ITER_MAGIC;
    iter->prevlev = ( *start )->sz;

    /* register the terminator node */
    iter->term = term;

    iter->list = l;
    /* mutex_unlock( iter->mtx_lock ); */
}


/** ***************************************************************
 *                                                                *
 * Come la funzione sopra, solo che non inizializza alcun mutex   *
 * Questo e' utile ad esempio per la drawList, per la searchAll   *
 *                                                                *
 ** ***************************************************************/

void setIteratorNoMutex( MySKL_t l, MySKL_i iter,
    MySKL_n n, MySKL_n term )
{
    MySKL_n * start;

    /* set start point */
    start = ( !n ? &l->header : &n );

    /* move forward */
    iter->next = ( *start )->next;

    IFDOUBLE (
        /* move backward */
        iter->prev = ( *start )->prev;
    )

    iter->magic = MYSKL_ITER_MAGIC;
    iter->prevlev = ( *start )->sz;

    /* register the terminator node */
    iter->term = term;

    iter->list = l;
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

void MySKLsetIterator ( MySKL_t l, MySKL_i * iter,
    MySKL_n n )
{
    size_t dim;

    X_ENTER ( THIS_IS_ME );
    READER_ENTER( l );

    LDN_BC_X_VOID ( l, DSKIP, NSKIP );
    EXP_BC_VOID ( iter != NULL );

    dim = X_ALLOC ( sizeof( MySKL_is ), *iter );
    MEM_BC_X_VOID ( *iter, dim );

    setIterator ( l, *iter, n, NULL );

    READER_EXIT( l );
    X_RETURN_VOID();
}



MySKL_n MySKLgetPrevNode ( MySKL_i * iter,
    uint lev )
{
# ifdef MYSKL_DOUBLE_LL_TYPE

    MySKL_n node = NULL;

    X_ENTER ( THIS_IS_ME );
    READER_ENTER( (*iter)->list );

    someSanityChecks ( iter, lev );
    node = getPrev ( *iter, lev );

    READER_EXIT( (*iter)->list );
    X_RETURN ( node );

# else /* just print a warn */

    /* avoid cc warning */
    iter = iter;
    lev  = lev;

    warnfna ( FUNC_NOT_AVAIL );
    return NULL;

# endif
}



MySKL_n MySKLgetNextNode ( MySKL_i * iter,
    uint lev )
{
    MySKL_n node = NULL;

    X_ENTER ( THIS_IS_ME );
    someSanityChecks ( iter, lev );

    READER_ENTER( (*iter)->list );
    node = getNext ( *iter, lev );

    READER_EXIT( (*iter)->list );
    X_RETURN ( node );
}



void MySKLresetIterator ( MySKL_i * iter,
    MySKL_n n )
{
    X_ENTER ( THIS_IS_ME );
    ITR_BC_VOID ( iter );

    setIteratorNoMutex( ( *iter )->list, *iter, n, NULL );
    X_RETURN_VOID ();
}



void MySKLdestroyIterator ( MySKL_i * iter )
{
    X_ENTER ( THIS_IS_ME );
    ITR_BC_VOID ( iter );

    IFTHREAD(
        mutex_destroy( (*iter)->mtx_lock );
    )

    X_FREE ( *iter );
    X_RETURN_VOID ();
}
