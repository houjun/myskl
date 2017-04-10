
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

# define _PROTO_PRIVATE_INSERT
# define _PROTO_PUBLIC_INSERT
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_RAND
# define _PROTO_PRIVATE_INIT
# define _PROTO_PRIVATE_DBG
# define _PROTO_PRIVATE_GEN

# include "skl_priv.h"

# define SZ_NULL  0



/** **************************************************************
 *                                                               *
 * Aggiorna il nodo <n> utilizzando la funzione <updfunc> il cui *
 * compito e' copiare il contenuto del nodo <newnode> su <n>     *
 *                                                               *
 ** **************************************************************/

static void updateNodeData (
    MySKL_t l,
    MySKL_n n,
    MySKL_n newnode,
    void updfunc ( const void * n, const void * newnode ),
    uint8 options
)
{
    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    do
    {
        /* if ( n->data != data )
         *   update_data( n, n->data, l->dst );
         * update_data( n, newnode, updfunc );
         */

        /* update node 'n' by using node 'newnode' */
        (*updfunc)(n,newnode);

        if ( options & UP_ONE_MODE )
           break;

        /* move to the next node */
        n = next_node ( n );
    }
    while (
        n != NIL ( l ) &&
        eq ( l, n->next[ 0 ], newnode )
    );

    /* debug call if enabled */
    X_RETURN_VOID();
}


/** **************************************************************
 *                                                               *
 * Crea un nuovo nodo che utilizza <lev> livelli inizializzando  *
 * l'array di puntatori che rappresenta i livelli a NIL          *
 * Ritorna il nodo creato                                        *
 *                                                               *
 ** **************************************************************/

static MySKL_n newNode ( MySKL_t l, MySKL_n x, uint lev )
{
    uint i = 0;
    size_t dim;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* array of size 'lev' for next node pointers */

    dim = X_ALLOC ( lev * sizeof( MySKL_n ), x->next );
    MEM_BC_X_RETV ( x->next, dim, NULL );

    IFDOUBLE (
        dim = X_ALLOC ( lev * sizeof( MySKL_n ), x->prev );
        ON_ERROR_FREE_MEM ( x->prev, dim, x->next );
    )

    x->sz = lev;
    for ( ; i < lev; i++ )
    {
        x->next[ i ] = NIL ( l );
        IFDOUBLE ( x->prev[ i ] = NIL ( l ); )
    }

    X_RETURN ( x );
}


/** **************************************************************
 *                                                               *
 * Crea e inserisce il nodo x nella skiplist, aggiornando i link *
 * dei nodi precedenti ad  x - e del nodo x stesso - livello per *
 * livello. I nodi che precedono x sono contenuti nell'array     *
 * <update>                                                      *
 *                                                               *
 ** **************************************************************/

static MySKL_e addNodeAndUpdateLinks ( MySKL_t l,
    MySKL_n * update, MySKL_n newnode, uint newLevel,
    uint8 options
    )
{
    uint curlevel = l->curlevel, i;
    MySKL_n x;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* se stiamo facendo una copia non usiamo la
     * genLevel()
     */

    if ( !(options & CP_MODE) )
        if ( !( newLevel = genLevel ( l ) ) )
            X_RETURN ( MYSKL_MEM_EXHAUSTED );

    /* crea il nodo, altrimenti ritorna errore
     * memoria insufficiente */

    x = newNode ( l, newnode, newLevel );
    if ( !x ) X_RETURN ( MYSKL_MEM_EXHAUSTED );

    if ( newLevel > curlevel )
    {
        /* il nodo che precede x per i livelli
         * maggiori di curlevel e' l'header NIL
         */

        for ( i = curlevel; i < newLevel; i++ )
            update[ i ] = NIL ( l );

        l->curlevel = newLevel;
    }

    /* aggiustiamo i link di x e dei nodi
     * precedenti livello per livello
     */

    for ( i = 0; i < newLevel; i++ )
    {
        x->next[ i ] = update[ i ]->next[ i ];
        update[ i ]->next[ i ] = x;

        /*  *(x->next + i) =  *((*(*(update + i))).next+i);
         *  *((*(*(update + i))).next + i ) = x;
         *
         */

        IFDOUBLE (
            x->next[ i ]->prev[ i ] = x;
            x->prev[ i ] = update [ i ];
        )
    }

    /* abbiamo inserito x come ultimo nodo
     * aggiorniamo la coda
     */

    if ( x->next[ 0 ] == NIL ( l ) )
        l->tail = x;

    l->nodenum++;
    X_RETURN ( MYSKL_STATUS_OK );
}


/** **************************************************************
 *                                                               *
 * Funzione di inserimento iterativa. Inserisce il node nella    *
 * lista <l>                                                     *
 *                                                               *
 ** **************************************************************/

MySKL_e _insertPlain (

    MySKL_t l,
    MySKL_n newnode,
    uint newLevel,
    uint8 options
)
{
    MySKL_n x, nnode;
    uint i;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    if ( !(options & CP_MODE) )
        if ( !( newLevel = genLevel ( l ) ) )
            X_RETURN ( MYSKL_MEM_EXHAUSTED );

    nnode = newNode ( l, newnode, newLevel );
    if ( !nnode ) X_RETURN ( MYSKL_MEM_EXHAUSTED );

    if ( newLevel > l->curlevel ) /* NON spostare sopra */
        l->curlevel = newLevel;

    i = l->curlevel;  /* NON spostare sopra*/
    x = l->header;

    while ( i )
    {
        /* si procede finche' key e' > dell'item del nodo
         * successivo a x, oppure il nodo succ. e' NIL
         */

        i--;
        while (
            x->next[ i ] != NIL ( l ) &&
            less ( l, x->next[ i ], newnode )
        ) x = next_node_level( x, i );

        if ( i < newLevel )
        {
            nnode->next[ i ] = x->next[ i ];
            x->next[ i ] = nnode;

            IFDOUBLE (
                nnode->next[ i ]->prev[ i ] = nnode;
                nnode->prev[ i ] = x;
            )
        }
    }

    /* abbiamo inserito nnode come ultimo nodo
     * aggiorniamo la coda
     */

    if ( nnode->next[ 0 ] == NIL ( l ) )
        l->tail = nnode;

    l->nodenum++;
    X_RETURN ( MYSKL_STATUS_OK );
}



/** **************************************************************
 *                                                               *
 * Funzione di inserimento iterativa. Inserisce il nodo nella    *
 * lista <l>, appoggiandosi ad una funzione ausiliaria           *
 *                                                               *
 ** **************************************************************/

MySKL_e _insertI (
    MySKL_t l,
    MySKL_n newnode,
    void updfunc ( const void * n, const void * newnode ),
    uint nodeLevel, uint8 options
)
{
    MySKL_n upd[MYSKL_MAXLEVELS], x;
    MySKL_e retstat;

    uint i;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    x = l->header;
    i = l->curlevel;

    while ( i )
    {
        /* si procede finche' key e' > dell'item del nodo
         * successivo a x, oppure il nodo succ. e' NIL
         */

        i--;
        while (
            x->next[ i ] != NIL ( l ) &&
            less ( l, x->next[ i ], newnode )
        ) x = next_node_level( x, i );

        /* si registrano i nodi dove si scende di
         * liv. perche' dovranno essere aggiornati
         */

        upd[ i ] = x;
    }

    /* appena usciti dal while, key e' <= dell'item del
     * nodo succ. a x, oppure x punta all'ultimo nodo
     */

    x = next_node ( x );
    if ( ( x != NIL ( l ) ) && eq ( l, x, newnode ) )
    {
        if ( options & ND_MODE )
           X_RETURN ( MYSKL_DUPLICATE_KEY );

        if ( (options & UP_ALL_MODE) ||
            (options & UP_ONE_MODE)
        )
        {
            updateNodeData ( l, x, newnode, updfunc, options );
            X_RETURN ( MYSKL_STATUS_OK );
        }
    }

    /* prova a creare il nodo da aggiungere alla
     * lista e ad aggiornare i link
     */

    retstat = addNodeAndUpdateLinks ( l, upd, newnode,
        nodeLevel, options );

    X_RETURN ( retstat );
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

MySKL_e MySKLinsertAD ( MySKL_t l, MySKL_n newnode )
{
    MySKL_e retstat;

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( newnode != NULL, MYSKL_ERROR );

    /* for thread mode */
    WRITER_ENTER( l );

    retstat = _insertPlain ( l, newnode,
        SZ_NULL, AD_MODE );

    WRITER_EXIT( l );
    X_RETURN ( retstat );
}



MySKL_e MySKLinsertND ( MySKL_t l, MySKL_n newnode )
{
    MySKL_e retstat;

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( newnode != NULL, MYSKL_ERROR );

    WRITER_ENTER( l );

    retstat = _insertI ( l, newnode, NULL,
        SZ_NULL, ND_MODE );

    WRITER_EXIT( l );
    X_RETURN ( retstat );
}



MySKL_e MySKLinsertUPD ( MySKL_t l, MySKL_n newnode,
    void updfunc ( const void * n, const void * newnode ))
{
    MySKL_e retstat;

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_X_RETV ( updfunc != NULL, MYSKL_ERROR );
    EXP_BC_X_RETV ( newnode != NULL, MYSKL_ERROR );

    /* for thread mode */
    WRITER_ENTER( l );

    retstat = _insertI ( l, newnode, updfunc,
        SZ_NULL, UP_ONE_MODE );

    WRITER_EXIT( l );
    X_RETURN ( retstat );
}



MySKL_e MySKLinsertUPD_ALL( MySKL_t l, MySKL_n newnode,
    void updfunc ( const void * n, const void * newnode ))
{
    MySKL_e retstat;

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, MYSKL_ERROR );
    EXP_BC_RETV(updfunc != NULL, MYSKL_ERROR);
    EXP_BC_RETV(newnode != NULL, MYSKL_ERROR);

    /* for thread mode */
    WRITER_ENTER( l );

    retstat = _insertI ( l, newnode, updfunc,
        SZ_NULL, UP_ALL_MODE );

    WRITER_EXIT( l );
    X_RETURN ( retstat );
}



MySKL_e MySKLcopy ( MySKL_t source, MySKL_t target,
    MySKL_n cpyfunc ( const void * n ) )
{
    MySKL_n x, newnode;
    MySKL_e ret;

    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV( source, DSKIP, NSKIP, MYSKL_ERROR );
    LDN_BC_X_RETV( target, DSKIP, NSKIP, MYSKL_ERROR);
    EXP_BC_X_RETV( cpyfunc != NULL, MYSKL_ERROR );

    READER_ENTER( source );
    WRITER_ENTER( target );

    x = source->header->next[ 0 ];
    while ( x != NIL ( source ) )
    {
        /* copy node x to newnode */
        newnode = cpyfunc(x);

        ret = _insertPlain ( target, newnode,
            x->sz, AD_MODE );

        if ( ret == MYSKL_MEM_EXHAUSTED )
        {
            WRITER_EXIT( target );
            READER_EXIT( source );

            X_RETURN ( ret );
        }

        /* move to the next node */
        x = next_node( x );
    }

    WRITER_EXIT( target );
    READER_EXIT( source );

    X_RETURN ( MYSKL_STATUS_OK );
}
