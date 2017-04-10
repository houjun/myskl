
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

# define _PROTO_PRIVATE_SEARCH
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PUBLIC_SEARCH
# define _PROTO_PRIVATE_DBG

# include "skl_priv.h"

# define set_dup_iterator(l,iter,n,t)   \
{                                       \
    mutex_lock( (iter)->mtx_lock );     \
    (iter)->magic = MYSKL_ITER_MAGIC;   \
    (iter)->prevlev = 1;                \
    (iter)->next = n;                   \
                                        \
    /* register the terminator node     \
     * and list                         \
     */                                 \
    (iter)->term = t;                   \
    (iter)->list = l;                   \
    mutex_unlock( (iter)->mtx_lock );   \
}


/** *************************************************************
 *                                                              *
 * Cerca la chiave <key> partendo dal nodo x - l'header. Trova  *
 * il primo e l'ultimo nodo duplicati                           *
 *                                                              *
 ** *************************************************************/

static MySKL_n * searchAll ( MySKL_t l, MySKL_n nodetofound,
    MySKL_n * term )
{
    MySKL_n x = l->header;
    uint i;

    X_ENTER ( THIS_IS_ME );


        i=0;
        while (
            x->next[ i ] != NIL ( l ) &&
            less ( l, x->next[ i ], nodetofound )
            ) x = next_node_level ( x, i );

        if (
            ( x->next[ i ] != NIL ( l ) ) &&
              eq ( l, x->next[ i ], nodetofound )
            )
        {
            /* save the address of the pointer referring
             * to the node found (the first) and iterate
             * to found all duplications
             */
             MySKL_n * addr = &(x->next[ i ]);
             x = next_node_level( x, i );

             while (
                x->next[ 0 ] != NIL ( l ) &&
                eq ( l, x->next[ 0 ], nodetofound )
            )  x = next_node ( x );

            /* save the node after the last node found
             * (terminator)
             */
            *term = x->next[ 0 ];

            /* return the address of the pointer to the
             * first node
             */

            X_RETURN ( addr );
        }

    X_RETURN ( NULL );
}


/** *************************************************************
 *                                                              *
 * Cerca la chiave <key> partendo dal nodo x - l'header         *
 *                                                              *
 ** *************************************************************/

MySKL_n search ( MySKL_t l, MySKL_n nodetofound )
{
    MySKL_n x = l->header;
    uint i;

    X_ENTER ( THIS_IS_ME );

    i = l->curlevel;
    while ( i )
    {
        /* si procede finche' key e' maggiore dell'elemento
         * successivo a x, oppure quest'ultimo e' NIL
         */

        i--;
        while (
            x->next[ i ] != NIL ( l ) &&
              less ( l, x->next[ i ], nodetofound )
            ) x = next_node_level ( x, i );

        if (
            ( x->next[ i ] != NIL ( l ) ) &&
              eq ( l, x->next[ i ], nodetofound )
            )
        {
            /* return node found */
            X_RETURN ( x->next[ i ] );
        }
    }

    X_RETURN ( NULL );
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

MySKL_e MySKLsearch ( MySKL_t l, MySKL_n tofound,
    MySKL_n * nodefound )
{
    MySKL_n n = NULL;

    X_ENTER ( THIS_IS_ME );
    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );

    /* for thread mode */
    READER_ENTER( l );

    if ( l->nodenum ) n = search ( l, tofound );
    if ( nodefound != NULL ) *nodefound = n;

    /* for thread mode */
    READER_EXIT( l );

    if ( n ) X_RETURN ( MYSKL_KEY_FOUND );
    X_RETURN ( MYSKL_KEY_NOT_FOUND );
}



MySKL_e MySKLsearchALL ( MySKL_t l, MySKL_n tofound,
    MySKL_i * i )
{
    MySKL_n * n = NULL;
    MySKL_n trn = NULL;  /* terminator node */
    size_t dim;

    X_ENTER ( THIS_IS_ME );
    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );

    READER_ENTER( l );
    EXP_BC_X_RETV ( i, MYSKL_ERROR );

    dim = X_ALLOC ( sizeof( MySKL_is ), *i );
    MEM_BC_X_RETV ( *i, dim, MYSKL_ERROR );

    mutex_init_default( (*i)->mtx_lock );
    if ( l->nodenum ) n = searchAll ( l, tofound, &trn );

    if ( n )
    {
        set_dup_iterator( l, *i, n, trn );

        READER_EXIT( l );
        X_RETURN ( MYSKL_KEY_FOUND );
    }
    else set_dup_iterator( l, *i, NULL, NULL );

    READER_EXIT( l );
    X_RETURN ( MYSKL_KEY_NOT_FOUND );
}
