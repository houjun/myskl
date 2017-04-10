
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

# define _PROTO_PRIVATE_ERROR
# define _PROTO_PUBLIC_DELETE
# define _PROTO_PUBLIC_INSERT
# define _PROTO_PRIVATE_DBG

# include "skl_priv.h"

static void destroy_node ( MySKL_t l, MySKL_n n );
static void adjustCurLevel ( MySKL_t l );



/** ****************************************************************
 *                                                                 *
 * Cancella dalla lista il nodo con la chiave <key>. Il parametro  *
 * <force> permette di cancellare o meno un nodo che contiene dati *
 * condivisi con almeno un'altra lista.                            *
 *                                                                 *
 ** ****************************************************************/

static MySKL_e deleteI ( MySKL_t l, MySKL_n nodetodel,
    bool ( * dstdatafunc )( MySKL_n ), bool force )
{
    MySKL_n update[MYSKL_MAXLEVELS], x = l->header;
    uint i, curlevel = l->curlevel;

    /*  debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    i = curlevel;
    while ( i )
    {
        /*  si procede finche' key e' > dell'item del nodo
         * successivo a x, oppure il nodo succ. e' NIL
         */

        i--;
        while (
            x->next[ i ] != NIL ( l ) &&
            less ( l, x->next[ i ], nodetodel )
        ) x = next_node_level( x, i );

        /* si registrano i nodi dove si scende di
         * liv. perche' dovranno essere aggiornati
         */

        update[ i ] = x;
    }

    /* appena usciti dal while, key e' <= dell'item del
     * nodo succ. a x, oppure x punta all'ultimo nodo
     */

    x = next_node ( x );

    if ( ( x == NIL ( l ) ) || !eq ( l, x, nodetodel ) )
        X_RETURN ( MYSKL_KEY_NOT_FOUND );

    /* deletion of the node's data (not of the node) */

    if ( dstdatafunc != NULL )
    {
        if ( ( *dstdatafunc )( x ) == FALSE && !force )
            X_RETURN ( MYSKL_NODE_NOT_DELETED );
    }

    for ( i = 0; i < curlevel; i++ )
    {
        /* se vero, non abbiamo piu' bisogno di fare
         * update dei puntatori del nodo prima di x
         */

        if ( update[ i ]->next[ i ] != x ) break;

        /* aggiorna i puntatori in modo che saltino
         * il nodo x ( quello da cancellare )
         */

        update[ i ]->next[ i ] = x->next[ i ];
        IFDOUBLE ( x->next[ i ]->prev[ i ] = update[ i ] );
    }

    /* se si sta per eliminare l'ultimo nodo
     * va aggiornato il puntatore alla coda
     */

    if ( x->next[ 0 ] == NIL ( l ) )
        l->tail = *update;

    destroy_node ( l, x );
    adjustCurLevel ( l );

    l->nodenum--;
    X_RETURN ( MYSKL_STATUS_OK );
}



/** ****************************************************************
 *                                                                 *
 * Distrugge il nodo <n> e l'array di puntatori in esso contenuto. *
 * Il puntatore a funzione <dstfunc> serve a cancellare i dati del *
 * nodo <n> ( key e item ) ed e' definito dall'utente              *
 *                                                                 *
 ** ****************************************************************/

static void destroy_node ( MySKL_t l, MySKL_n n )
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* destroy node */

    IFDOUBLE ( X_FREE ( n->prev ) );
    X_FREE ( n->next );

    if ( l->dst != NULL )(*(l->dst))( n );

    /* debug call if enabled */
    X_RETURN_VOID ();
}



/** **************************************************************
 *                                                               *
 * Se necessario aggiusta la variabile che tiene traccia della   *
 * altezza massima - massimo livello - della lista               *
 *                                                               *
 ** **************************************************************/

static void adjustCurLevel ( MySKL_t l )
{
    uint curlevel = l->curlevel - 1;

    X_ENTER ( THIS_IS_ME );

    while ( curlevel > 0 &&
            ( l->header->next[ curlevel-- ] == NIL ( l ) )
            ) l->curlevel--;

    /* debug call if enabled */
    X_RETURN_VOID ();
}



/** ***********************************************************
 ** ***********************************************************
 +                                                            +
 +                                                            +
 +               FUNZIONI/PROCEDURE PUBBLICHE                 +
 +                                                            +
 +                                                            +
 ** ***********************************************************
 ** ***********************************************************/

MySKL_e MySKLdeleteNF ( MySKL_t l, MySKL_n nodetodel,
    bool ( * dstdatafunc )( MySKL_n ) )
{
    MySKL_e ret = MYSKL_KEY_NOT_FOUND;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    WRITER_ENTER( l );

    /* 'dstdatafunc' is not the node destroyer, but
     * the data destroyer and may be NULL
     */

    if ( l->nodenum )
        ret = deleteI( l, nodetodel, dstdatafunc, FALSE );

     WRITER_EXIT( l );
     X_RETURN ( ret );
}



MySKL_e MySKLdeleteAF ( MySKL_t l, MySKL_n nodetodel,
    bool ( * dstdatafunc )( MySKL_n ) )
{
    MySKL_e ret = MYSKL_KEY_NOT_FOUND;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    WRITER_ENTER( l );

    if ( l->nodenum )
        ret = deleteI ( l, nodetodel, dstdatafunc, TRUE );

    WRITER_EXIT( l );
    X_RETURN ( ret );
}
