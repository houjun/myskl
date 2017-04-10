
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
# define _PROTO_PUBLIC_DRAW

# include "skl_priv.h"


/* **********************************************************
# define _set_iterator(l,iter,n)                            \
{                                                           \
    size_t dim = X_ALLOC ( sizeof( MySKL_iter ), *(iter) ); \
    MEM_BC_X_VOID ( *(iter), (dim) );                       \
    setIterator ( l, *(iter), n, NULL );                    \
}
* ***********************************************************/

# define _set_iterator(l,iter,n)                            \
{                                                           \
   if ( !( iter = malloc( sizeof( MySKL_is ) ) ) ) return;  \
   setIteratorNoMutex( l, iter, n, NULL );                  \
}

# define _reset_iterator(l,iter,n)          \
{                                           \
    setIteratorNoMutex( l, iter, n, NULL ); \
}



/** ****************************************************************
 *                                                                 *
 * Fa in modo che il disegno del link che precede i dati del nodo  *
 * successivo al nodo corrente (quest'ultimo è puntato da <iter1>  *
 * e da <iter2>) del livello <L+1>, sia della lunghezza corretta.  *
 * In pratica usando il nodo corrente come partenza, si prendono   *
 * i nodi successivi al liv <L+1> e al livello 1 e finche' questi  *
 * non corrispondono si allunga il link stampando "====". Questo   *
 * pero' non basta difatti e' necessario tenere conto anche dello  *
 * spazio occupato dai dati che verranno saltati. Questo viene     *
 * fatto utilizzando il puntatore a funzione <getSizeofData> che   *
 * prende come parametro un nodo e ritorna la lunghezza dei suoi   *
 * dati, in modo da aggiungere il giusto numero di '=' a quanto    *
 * stampato prima. I dati del nodo successivo a quello corrente    *
 * verranno poi stampati dalla funzione drawList()                 *
 *                                                                 *
 ** ****************************************************************/

static void adjustLink ( MySKL_i * iter1, MySKL_i * iter2,
    uint L, int ( * getSizeofData )( MySKL_n ) )
{
    MySKL_n ptrref, tmpptr;
    uint i;

    ptrref = getNextNoMutex ( *iter1, L+1 );
    tmpptr = getNextNoMutex ( *iter2, 1 );

    while ( tmpptr != ptrref )
    {
        fprintf ( stdout, "====" );
        i = ( *getSizeofData )( tmpptr );

        /* also counts char '<' */
        IFDOUBLE ( i++; )

        for ( ; i > 0; i-- ) fprintf ( stdout, "=" );
        tmpptr = getNextNoMutex ( *iter2, 1 );
    }
}



/** *****************************************************************
 *                                                                  *
 * Disegna la lista l mediante l'utilizzo di puntatori a funzione:  *
 * il puntatore <sizeofData> (utilizzato dalla funzione adjustLink) *
 * e il puntatore <printData> ( utilizzato per stampare item e key  *
 * dei nodi )                                                       *
 *                                                                  *
 ** *****************************************************************/

static void drawList ( MySKL_t l, int ( * sizeofData )( MySKL_n ),
    void ( * printData )( MySKL_n ) )
{
    MySKL_i iter1, iter2;
    MySKL_n x;
    uint L;

    _set_iterator( l, iter1, NULL );
    _set_iterator( l, iter2, NULL );

    L = l->maxlevel;
    while ( L )
    {
        fprintf ( stdout, "Level # %d :\t[H]", L );
        L--;

        if ( l->header->next[ L ] == NIL ( l ) )
        {
            fprintf ( stdout, "====|\n" );
            continue;
        }

        _reset_iterator ( ( iter1 )->list, iter1, NULL );
        _reset_iterator ( ( iter2 )->list, iter2, NULL );

        x = l->header;
        while ( x->next[ L ] != NIL ( l ) )
        {
            IFDOUBLE ( fprintf ( stdout, "<" ); )

            if ( L ) adjustLink ( &iter1, &iter2,
                    L, sizeofData );

            x = next_node_level( x, L );
            fprintf ( stdout, "===>" );

            /* print data handler */
            ( *printData )( x );
        }

        /* go down a level */
        fprintf ( stdout, "\n" );
    }

    free ( iter1 );
    free ( iter2 );
}



/** ****************************************************************
 *                                                                 *
 * Stampa informazioni riguardanti la struttura della lista <l>,   *
 * indirizzo dei nodi e dei dati che la compongono compresi        *
 *                                                                 *
 ** ****************************************************************/

static void printListInfo ( MySKL_t l )
{
    MySKL_n node;
    uint i, cnt;

    fprintf ( stdout,
        "Header:\n\tList size: %d\n"
        "\tNIL: %p\n\tMax height: %d\n",
        l->nodenum, (void *) NIL ( l ),
        l->maxlevel
        );

    node = l->header->next[ 0 ];
    cnt  = 0;

    while ( node != NIL ( l ) )
    {
        fprintf ( stdout,
            "\nNode # %d:\n\tAddress: %p\n"
            /*"\tKey: %p\n\tItem: %p\n"*/
            "\tHeight: %d\n\tNext: ",
            ++cnt, node, /*node->data->key,
            node->data->item,*/ node->sz
            );

        for ( i = 0; i < node->sz; i++ )
            fprintf ( stdout, "[%d]: %p ", i, node->next[ i ] );

        IFDOUBLE (
            fprintf ( stdout, "\n\tPrev: " );

            for ( i = 0; i < node->sz; i++ )
                fprintf ( stdout, "[%d]: %p ", i, node->prev[ i ] );
        )

        node = node->next[ 0 ];
        fprintf ( stdout, "\n" );
    }
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

void MySKLdrawList ( MySKL_t l, int ( * sizeofData )( MySKL_n ),
    void ( * printData )( MySKL_n ) )
{
    LDN_BC_VOID ( l, DSKIP, NSKIP );

    EXP_BC_VOID ( sizeofData != NULL )
    EXP_BC_VOID ( printData != NULL );

    READER_ENTER( l );
    drawList ( l, sizeofData, printData );

    READER_EXIT( l );
    return;
}



void MySKLdisplayListStruct ( MySKL_t l )
{
    LDN_BC_VOID ( l, DSKIP, NSKIP );
    READER_ENTER( l );

    printListInfo ( l );
    READER_EXIT( l );
}
