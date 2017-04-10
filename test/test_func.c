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

# include <stdlib.h>
# include <stdio.h>
# include <string.h>

/* for malloc() and free() even though they should
 * be declared in <stdlib.h>
 */

# include <malloc.h>
# include "func.h"


void insertion_sort( void *a, size_t n,
    size_t es,  int (*cmp) (const void *, const void *));

# define sort insertion_sort


/* ************************************************************ */
/*                                                              */
/*                   MACRO AND DATA TYPES                       */
/*                          begin                               */
/*                                                              */
/* ************************************************************ */

/* store input data - from command line or default values
 * <maxLev1>,<maxLev2>,<elemNu1>,<elemNu2>,<keythreshold>
 */
struct input inputargs = { 5, 3, 7, 3, 5 };




/* ************************************************************ */
/*                                                              */
/*                  CLIENT DEFINED FUNCTION                     */
/*                           begin                              */
/*                                                              */
/* ************************************************************ */

/* get the sizeof of data contained in the node <n> */

int getSizeofData ( MySKL_n n )
{
    int  base = 10, j = 1;
    ITYPE item; IKEY key;

   item = MySKLgetEntry ( n, LN_s, node )->item;

   while ( item  >= base )
   { base *= 10; j++; }

   base = 10; j++;

   key = MySKLgetEntry ( n, LN_s, node )->key;

   while ( key >= base )
   { base *= 10; j++; }

   /* +3 is to also count  '[', ']', '|' */
   return j + 3;
}


/* make a copy of the node n */

MySKL_n copyfunc ( const void * n )
{
   LN_p newobj= malloc( sizeof( LN_s) );
   LN_p obj = MySKLgetEntry( n, LN_s, node );

   newobj->item = obj->item;
   newobj->key  = obj->key;

   return &(newobj->node);
}


/* compare generic item x and y */

int cmpfunc ( const void * x, const void * y )
{
    LN_p a = MySKLgetEntry ( x, LN_s, node );
    LN_p b = MySKLgetEntry ( y, LN_s, node );

     return (( a->key > b->key )-( a->key < b->key ));
}


/* print the data of the node <n> */

void printData ( MySKL_n n )
{
   printf ( "[%d|%d]", MySKLgetEntry ( n, LN_s, node )->key,
          MySKLgetEntry ( n, LN_s, node )->item );
}


/* requested when sorting */

int cmpitem(const void *p1, const void *p2)
{
    if ( ((DS_p) p1)->key == ((DS_p) p2)->key ) return 0;
    if ( ((DS_p) p1)->key < ((DS_p) p2)->key  ) return 1;

    return -1;
}


/* destroy data of the node <n> */

void dstnode ( MySKL_n n )
{
     LN_p item = MySKLgetEntry ( n, LN_s, node );
     free( item);
}



/* ************************************************************ */
/*                                                              */
/*                  AUXLIARY FUNCTION SET                       */
/*                          begin                               */
/*                                                              */
/* **************************************************************/

/* store input from cmdline in global struct <i> */

bool storeInput ( int argc, char *argv[] )
{
    int k = 1;
    int * p;

    /* use default values as an input */
    if ( argc == 1 ) return 1;

    if ( argc != ARGNUM+1 )
    {
       printf ( ERRARG, *argv );
       return 0;
    }

    for ( ; k <= ARGNUM; k++ )
    {
        /* address of var. number <k>
         * inside global struct i
         */

        p = ( ITYPE * )&inputargs + ( k-1 );

        *p = atoi( argv[ k ] );
        if ( !*p ) return 0;
    }

    return 1;
}


/* generate a random item from 1 to 1024*/

int irand( void )
{
    return 1+rand()%1024;
}


/* generate a random key from 1 to keythreshold */

int krand( void )
{
    return 1+rand()%inputargs.keythreshold;
}


/* initialize a container having <numelem> cells */

static DS_p container_init( int numelem )
{
    DS_p container;

    container = (DS_p) malloc ( numelem * sizeof(DS_t) );
    return container;
}


/* add data to container: data may be duplicatet or not */

static bool container_adddata( DS_p container,
    DS_t data, int pos, char mode )
{
    int tmp = 0;

    if ( mode == DUPINS )
    {
        container[pos]= data;
        return TRUE;
    }

    /* no dups wanted */
    while (tmp < pos)
    {
        if (container[tmp].key == data.key)
            break;
        tmp++;
    }

    if ( tmp == pos )
    {
        container[pos]= data;
        return TRUE;
    }

    /* duplicates found */
    return FALSE;
}


/* test library function: insert */

void inserttest ( MySKL_t l, int N, bool insmode )
{
    MySKL_i it1;
    DS_p expected;
    MySKL_e err;
    MySKL_n n;
    int pos;

    if ( (expected = container_init( N )) == NULL ) return;
    pos = 0;

    while( N-- > 0 )
    {
        DS_t data = { krand(), irand(), UNMARKED_DATA };
        printf ( WHATINS, data.key, data.item );

        LN_p item = malloc( sizeof( LN_s) );
        item->key= data.key;
        item->item= data.item;

        if ( container_adddata( expected, data, pos,
             insmode ) ) pos++;

        err = ( insmode ?
            MySKLinsertAD( l, &(item->node) ):
            MySKLinsertND( l, &(item->node) )
        );

        if ( err != MYSKL_STATUS_OK )
        {
            free (item);
            printf ( INSRES1 );
        }
    }

    sort( expected, pos, sizeof(DS_t), cmpitem );
    MySKLsetIterator ( l, &it1, NULL );

    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
    {
        if ( MySKLgetEntry ( n, LN_s, node )->key !=
             expected[pos-1].key ||
             MySKLgetEntry ( n, LN_s, node )->item !=
             expected[pos-1].item
        ) break;
        pos--;
    }

    MySKLdestroyIterator( &it1 );
    free ( expected );

    if (n != NULL)
        printf ("%sInsert test failed%s: allow duplicate? %d\n",
            s_test_colors[RED], s_test_colors[NONE], insmode);
    else
        printf ( "%sInsert test ok%s\n", s_test_colors[GREEN],
            s_test_colors[NONE] );
}


/* test library function: delete */

void deletetest ( MySKL_t l, bool force )
{
    LN_s tofound;
    MySKL_e err;

    /* NOTE: +1 generates float exeption at runtime */
    tofound.key = krand() % MySKLgetNodeNum( l );

    err = ( !force ?
       MySKLdeleteNF( l, &tofound.node, NULL ) :
       MySKLdeleteAF( l, &tofound.node, NULL )
    );

    if ( err != MYSKL_NODE_NOT_DELETED )
    {
       err == MYSKL_KEY_NOT_FOUND ?
         printf ( DELRES2, tofound.key ):
         printf ( DELRES1, tofound.key );
    }
    else printf ( DELRES3 );
}


/* test library function: copy */

void copytest( MySKL_t l1, MySKL_t l_copy )
{
    MySKL_i it1 = NULL;
    MySKL_i it2 = NULL;
    MySKL_n n;
    int pos, N;
    MySKL_e err;
    DS_p expected;
    int insmode = 1;

    MySKLsetIterator ( l1, &it1, NULL );

    N = MySKLgetNodeNum(l1);
    if ( (expected = container_init( N )) == NULL ) return;

    pos = 0;
    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
    {
        DS_t data = { MySKLgetEntry ( n, LN_s, node )->key,
                      MySKLgetEntry ( n, LN_s, node )->item,
                      UNMARKED_DATA
        };
        expected[pos]= data;
        pos++;
    }

    sort(expected, pos, sizeof(DS_t), cmpitem);

    err = MySKLcopy( l1, l_copy, copyfunc );
    MySKLsetIterator( l_copy, &it2, NULL );

    while ( ( n = MySKLgetNextNode ( &it2, 1 ) ) )
    {
        if ( MySKLgetEntry ( n, LN_s, node )->key !=
             expected[pos-1].key ||
             MySKLgetEntry ( n, LN_s, node )->item !=
             expected[pos-1].item
        ) break;
        pos--;
    }

    if ( n != NULL )
        printf ("%sCopy test failed%s: allow duplicate: %d\n",
            s_test_colors[RED], s_test_colors[NONE], insmode);
    else
        printf ("%sCopy test ok%s\n", s_test_colors[GREEN],
            s_test_colors[NONE]);

    free ( expected );
    MySKLdestroyIterator( &it1 );
    MySKLdestroyIterator( &it2 );
}


/* test library function: join */

void jointest ( MySKL_t l1, MySKL_t l2,
   bool insmode )
{
    MySKL_i it1 = NULL;
    MySKL_i it2 = NULL;
    MySKL_e err;
    MySKL_n n;
    int numel, pos, N, bak, testfailed = 0;
    DS_p expected;

    MySKLsetIterator ( l1, &it1, NULL );
    MySKLsetIterator ( l2, &it2, NULL );

    N = MySKLgetNodeNum(l1) + MySKLgetNodeNum(l2);
    expected = malloc ( N * sizeof(DS_t) );
    if ( expected == NULL ) return;

    numel = N;
    pos = 0;

    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
    {

        DS_t data = { MySKLgetEntry ( n, LN_s, node )->key,
                      MySKLgetEntry ( n, LN_s, node )->item,
                      MARKED_DATA
        };

        if ( container_adddata( expected,
             data, pos, insmode ) ) pos++;
    }

    while ( ( n = MySKLgetNextNode ( &it2, 1 ) ) )
    {
        DS_t data = { MySKLgetEntry ( n, LN_s, node )->key,
                      MySKLgetEntry ( n, LN_s, node )->item,
                      UNMARKED_DATA
        };

        if ( container_adddata( expected,
             data, pos, insmode ) ) pos++;
    }

    sort(expected, pos, sizeof(DS_t), cmpitem);

    err = ( insmode ?
      MySKLjoinAD ( l1, l2, copyfunc ):
      MySKLjoinND ( l1, l2, copyfunc ) );

    if ( err == MYSKL_MEM_EXHAUSTED )
    {  printf ( JOINRES1 ); return; }

    MySKLresetIterator( &it1, NULL );
    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
    {
        if (expected[pos-1].marked)
        {
            int tmp = pos-1;
            int val = expected[pos-1].key;

            while ( tmp > 0 && expected[tmp-1].marked &&
                expected[tmp-1].key == val )
            {
                tmp--;
            }

            bak = tmp;
            while ( tmp <= pos-1 )
            {
                if ( MySKLgetEntry ( n, LN_s, node )->key !=
                    expected[tmp].key ||
                    MySKLgetEntry ( n, LN_s, node )->item !=
                    expected[tmp].item
                )
                {  testfailed=1; break; }

                if ( tmp == pos-1 ) break;
                n = MySKLgetNextNode ( &it1, 1 );
                tmp++;
            }

            pos = bak;
            if ( testfailed == 1 ) break;
        }
        else
        {
            if ( MySKLgetEntry ( n, LN_s, node )->key !=
                expected[pos-1].key ||
                MySKLgetEntry ( n, LN_s, node )->item !=
                expected[pos-1].item
             )
            {  testfailed=1; break; }

            pos--;
        }
    }

    if (testfailed== 1)
        printf ("%sJoin test failed%s: allow duplicate? %d\n",
            s_test_colors[RED], s_test_colors[NONE], insmode);
    else
        printf ("%sJoin test ok%s\n", s_test_colors[GREEN],
            s_test_colors[NONE]);

    free ( expected );
    MySKLdestroyIterator( &it1 );
    MySKLdestroyIterator( &it2 );

    printf ( JOINRES2 );
    return;
}


/* test library function: search */

void searchtest ( MySKL_t l )
{
    LN_s tofound;
    MySKL_e err;

    tofound.key = krand() % (MySKLgetNodeNum( l )+1);
    err = MySKLsearch( l, &tofound.node, NULL );

    err == MYSKL_KEY_NOT_FOUND ?
        printf ( SEARCHRES1, tofound.key ) :
        printf ( SEARCHRES2, tofound.key );
}


/* test library function: search all */

void searchtestall ( MySKL_t l, int key )
{
    LN_s tofound;
    MySKL_i it = NULL;
    MySKL_e err;
    MySKL_n n;

    if ( key == 0 )
        tofound.key = krand() % MySKLgetNodeNum( l );

    tofound.key = 3;
    err = MySKLsearchALL( l, &tofound.node , &it );

    err == MYSKL_KEY_NOT_FOUND ?
      printf ( SEARCHRES1, tofound.key ) :
      printf ( SEARCHRES2, tofound.key );

    while ( ( n = MySKLgetNextNode ( &it, 1 ) ) )
        ( *printData )( n ), printf ( "  " );

    MySKLdestroyIterator( &it );
}


/* test library function: init */

MySKL_t inittest ( int maxlev )
{
    MySKL_e err;
    MySKL_t l;

    l = MySKLinit( maxlev, cmpfunc, dstnode, &err );
    if ( err == MYSKL_STATUS_OK )
    {
        MySKLaddtoListsContainer(l);
        printf ( INITRES2 );
    }
    else printf ( INITRES1 );
    return l;
}


/* test library function: iterators */

void itertest ( MySKL_t l )
{
    MySKL_i it1 = NULL;
    MySKL_n n;
    char dbwason = 0;

    if ( MySKLverboseSuspend() ) dbwason = 1;
    MySKLsetIterator ( l, &it1, NULL );

    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
      ( *printData )( n ), printf ( "  " );

    printf ( "\n" );

    while ( ( n = MySKLgetPrevNode ( &it1, 1 ) ) )
     ( *printData )( n ), printf ( "  " );

    printf ( "\n" );

    MySKLdestroyIterator( &it1 );
    if ( dbwason ) MySKLverboseResume();
}


/* test library function: memory status */

void memorytest ( void )
{
    MySKLfreeListsContainer();
    MySKLverboseDisable();

    MySKLmemoryCleaned () ?
        printf ( MEMSTAT1, s_test_colors[GREEN], s_test_colors[NONE] ) :
        printf ( MEMSTAT2, s_test_colors[RED], s_test_colors[NONE] );
}


/* print a double delimiter */

void wrapper ( void )
{
    const int delimN = 156;
    int j = 0;

    printf ( "\n\n" );
    while ( j < delimN )
    {
        if ( j++ == delimN/2 )
          printf ( "\n" );

        printf ( "#" );
    }
}


/* print the given string s */

void printtext( const char *s,
   char pressret )
{
    size_t len = strlen(s);
    size_t lcp = len;

    printf ( "\n\n\n%s", s_test_colors[LIGHT_RED] );
    while ( len > 0 )
        if ( s[ --len ] != '\n' )
            printf ( "*" );

    len = lcp;
    printf ( "\n%s\n", s );
    while ( len > 0 )
        if ( s[ --len ] != '\n' )
            printf ( "*" );

    printf ("%s", s_test_colors[NONE]);
    pressret = pressret; /* avoid gcc warning */

# if INTERACTIVE_MODE
    if ( pressret == TRUE )
    {
        printf ( PRET_MSG );
        while ( getchar() != '\n' )
            ;
    }
    else printf ( "\n" );
# endif

   printf ( "\n\n" );
}


/* print info about list l */

void printlistinfo( MySKL_t l )
{
   char dbwason = 0;

   if ( MySKLverboseSuspend() ) dbwason = 1;

   printf ( LISTINFO,
     MySKLgetCurLevel ( l ),
     MySKLgetMaxLevel ( l ),
     MySKLgetNodeNum ( l )
   );

   if ( dbwason ) MySKLverboseResume();
}


/* use list container */

bool initLc( void )
{
    return (
      ( MySKLinitListsContainer () ) ?
      1 : ( printf ( LCERROR ), 0 )
    );
}


/* draw list l */

void drawlist ( MySKL_t l )
{
    char dbwason = 0;

    if ( MySKLverboseSuspend() ) dbwason = 1;

    MySKLdrawList ( l, getSizeofData, printData );

    if ( dbwason ) MySKLverboseResume();
}



void intersecttest ( MySKL_t l1, MySKL_t l2, MySKL_t l3,
   bool insmode )
{
    int tmpdata1_len;
    int tmpdata2_len;

    DS_p expected;
    DS_p tmpdata1;
    DS_p tmpdata2;

    MySKL_i it1 = NULL;
    MySKL_i it2 = NULL;
    MySKL_i it3 = NULL;
    MySKL_e err;
    MySKL_n n;
    int N, i, testfailed = 0, pos;

    MySKLsetIterator ( l1, &it1, NULL );
    MySKLsetIterator ( l2, &it2, NULL );

    N = MySKLgetNodeNum(l1);
    if ( (tmpdata1 = container_init( N )) == NULL ) return;

    pos = 0;

    while ( ( n = MySKLgetNextNode ( &it1, 1 ) ) )
    {
        DS_t data = { MySKLgetEntry ( n, LN_s, node )->key,
                     MySKLgetEntry ( n, LN_s, node )->item,
                     MARKED_DATA };

        if ( container_adddata( tmpdata1, data, pos,
             insmode ) ) pos++;
    }

    tmpdata1_len = pos;

    N = MySKLgetNodeNum(l2);
    if ( (tmpdata2 = container_init( N )) == NULL ) return;


    pos = 0;
    while ( ( n = MySKLgetNextNode ( &it2, 1 ) ) )
    {
        DS_t data = { MySKLgetEntry ( n, LN_s, node )->key,
                      MySKLgetEntry ( n, LN_s, node )->item,
                      UNMARKED_DATA };

        if ( container_adddata( tmpdata2, data, pos,
             insmode ) ) pos++;

    }

    tmpdata2_len = pos;
    if ( (expected = container_init(tmpdata1_len+tmpdata2_len )) == NULL ) return;

    i = pos = 0;
    while ( i < tmpdata1_len )
    {
        int j = 0;
        while ( j < tmpdata2_len )
        {
            if (tmpdata2[j].key == tmpdata1[i].key)
                break;
            j++;
        }

        if ( j != tmpdata2_len ) { expected[pos]= tmpdata1[i]; pos++; }
        i++;
    }

    sort(expected, pos, sizeof(DS_t), cmpitem);
    err = MySKLintersect(l1, l2, l3, copyfunc );


    MySKLsetIterator ( l3, &it3, NULL );
    while ( ( n = MySKLgetNextNode ( &it3, 1 ) ) )
    {
        if ( MySKLgetEntry ( n, LN_s, node )->key !=
             expected[pos-1].key ||
             MySKLgetEntry ( n, LN_s, node )->item !=
             expected[pos-1].item )
        {  testfailed=1; break; }

        pos--;
    }

    if (testfailed== 1)
        printf ("%sIntersect test failed%s: allow duplicate? %d\n",
            s_test_colors[RED], s_test_colors[NONE], insmode);
    else
        printf ("%sIntersect test ok%s\n",
            s_test_colors[GREEN], s_test_colors[NONE] );

    free ( expected );
    free ( tmpdata1 );
    free ( tmpdata2 );
    MySKLdestroyIterator( &it1 );
    MySKLdestroyIterator( &it2 );
    MySKLdestroyIterator( &it3 );
}



void insertion_sort(void *a, size_t n, size_t es,
    int (*cmp) (const void *, const void *))
{
    size_t slices = es / sizeof (char), i;
    char * pm, *pl, t;

    /* move from a[1] to a[n] */
    for (pm = (char *)a + es; pm < (char *)a + n * es; pm += es)
        for (pl = pm; pl > (char *)a && cmp(pl - es, pl) > 0;
             pl -= (es*2))
        {
            /* swap pl[x], pl[x-1] by copying every single slice
               from pl[x-1] to pl[x] and vice versa */
            i = slices;
            do {
                t = *pl;
                *pl = *(pl-es);
                *(pl-es) = t;
                pl++;
            } while (--i > 0);

            /* swap(pl, pl - es, es); */
        }
}
