
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

# define _PROTO_PUBLIC_INTERSECT
# define _PROTO_PRIVATE_INSERT
# define _PROTO_PRIVATE_SEARCH
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PUBLIC_JOIN

# include "skl_priv.h"
# include <stdlib.h>

# define SZ_NULL  0



/** **************************************************************
 *                                                               *
 * Effettua l'unione della lista <source> con la lista <target>  *
 * In altre parole, si prende ogni elemento della prima lista e  *
 * lo si aggiunge alla seconda usando le funzioni in:            *
 * <skl_ins.c>                                                   *
 *                                                               *
 ** **************************************************************/

static MySKL_n join (
    MySKL_t target,
    MySKL_t source,
    uint8 options,
    MySKL_n cpyfunc ( const void * n )
)
{
    MySKL_n node, x = source->header->next[ 0 ];
    MySKL_e ret;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* iterate through the list node by node and
     * insert each node into the 'target' list
     */

    do
    {
        /* make and return a copy of the node 'x' */
        node = cpyfunc( x );

        /* insert to 'target' list */
        ret = _insertI ( target, node, NULL, SZ_NULL, options );

        if ( ret == MYSKL_MEM_EXHAUSTED )
            X_RETURN ( NULL );

        if ( ret == MYSKL_DUPLICATE_KEY )
            if ( target ) ( *( target->dst ) )( node );

        /* move to the next node */
        x = next_node( x );
    }
    while ( x != NIL ( source ) );
    X_RETURN ( x );
}



/** **************************************************************
 *                                                               *
 * Effettua l'intersezione tra la lista <source1> e <source2> e  *
 * pone il risultato nella lista <target>.                       *
 *                                                               *
 ** **************************************************************/

static MySKL_n intersect (
    MySKL_t source1,
    MySKL_t source2,
    MySKL_t target,
    MySKL_n cpyfunc ( const void * n ),
    uint8 options
)
{
    MySKL_n x = source1->header->next[ 0 ];
    MySKL_n y = source2->header->next[ 0 ];
    uint tovisit1, tovisit2;
    MySKL_n node;
    MySKL_e ret;

    X_ENTER ( THIS_IS_ME );

    tovisit1 = source1->nodenum;
    tovisit2 = source2->nodenum;

    while ( tovisit1 && tovisit2 )
    {
        if ( eq ( source1, x, y ) )
        {
            /* make and return a copy of the node 'x' */
            node = cpyfunc( x );

            ret = _insertPlain ( target, node, SZ_NULL, options );
            if ( ret == MYSKL_MEM_EXHAUSTED ) X_RETURN ( NULL );

            if ( options & ND_MODE )
            {
                while ( --tovisit1 > 0 && eq ( source1, x->next[ 0 ], x ) )
                    x = next_node( x );

                while ( --tovisit2 > 0 && eq ( source1, y->next[ 0 ], y ) )
                    y = next_node( y );

                x = next_node( x );
                y = next_node( y );
            }
            else { x = next_node( x ); tovisit1--; }
        }
        else if ( less ( source1, x, y ) )
        {
            x = next_node( x );
            tovisit1--;
        }
        else { y = next_node( y ); tovisit2--; }
    }

    X_RETURN ( x );
/*
    do
    {
        res = search ( source2, x );
        if ( res != NULL )
        {
            MySKL_n node = cpyfunc( x );

            ret = _insertI ( target, node, NULL, SZ_NULL, options );

            if ( ret == MYSKL_MEM_EXHAUSTED )
                X_RETURN ( NULL );
        }

        next_node_while_key_equal( source1, x );
        x = next_node( x );
    }
    while ( x != NIL ( source1 ) );*/
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

MySKL_e MySKLjoinAD ( MySKL_t target, MySKL_t source,
    MySKL_n cpyfunc ( const void * n ) )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( target, DSKIP, NSKIP, MYSKL_ERROR );
    LDN_BC_X_RETV ( source, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( cpyfunc != NULL, MYSKL_ERROR );

    READER_ENTER( source );
    WRITER_ENTER( target );

    if (
        ( source->header->next[ 0 ] == NIL ( source ) ) ||
          join ( target, source, AD_MODE, cpyfunc )

        ) {
        WRITER_EXIT( target );
        READER_EXIT( source );

        X_RETURN ( MYSKL_STATUS_OK );
    }

    WRITER_EXIT( target );
    READER_EXIT( source );

    X_RETURN ( MYSKL_MEM_EXHAUSTED );
}



MySKL_e MySKLjoinND ( MySKL_t target, MySKL_t source,
    MySKL_n cpyfunc ( const void * n ) )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( target, DSKIP, NSKIP, MYSKL_ERROR );
    LDN_BC_X_RETV ( source, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( cpyfunc != NULL, MYSKL_ERROR );

    READER_ENTER( source );
    WRITER_ENTER( target );

    if (
        ( source->header->next[ 0 ] == NIL ( source ) ) ||
          join ( target, source, ND_MODE, cpyfunc )

        ) {
            READER_EXIT( source );
            WRITER_EXIT( target );
            X_RETURN ( MYSKL_STATUS_OK );
    }

    READER_EXIT( source );
    WRITER_EXIT( target );

    X_RETURN ( MYSKL_MEM_EXHAUSTED );
}



MySKL_e MySKLintersect ( MySKL_t source1, MySKL_t source2,
    MySKL_t target, MySKL_n cpyfunc ( const void * n ) )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( source1, DSKIP, NSKIP, MYSKL_ERROR );
    LDN_BC_X_RETV ( source2, DSKIP, NSKIP, MYSKL_ERROR );
    LDN_BC_X_RETV ( target, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( cpyfunc, MYSKL_ERROR );

    READER_ENTER( source1 );
    READER_ENTER( source2 );
    WRITER_ENTER( target );

    if (
        ( source1->header->next[ 0 ] == NIL ( source1 ) ) ||
        ( source2->header->next[ 0 ] == NIL ( source2 ) ) ||
        intersect ( source1, source2, target, cpyfunc, ND_MODE )

    ) {
        READER_EXIT( source1 );
        READER_EXIT( source2 );
        WRITER_EXIT( target );

        X_RETURN ( MYSKL_STATUS_OK );
    }

    READER_EXIT( source1 );
    READER_EXIT( source2 );
    WRITER_EXIT( target );

    X_RETURN ( MYSKL_MEM_EXHAUSTED );
}
