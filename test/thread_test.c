/***************************************************************************
 *   Copyright (C) 2010 by Andrea Ghersi                                   *
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "func.h"
#include "thread_func.h"

#define NOFORCEDEL 0
#define FORCEDEL   1

#define NODUPINS   0
#define DUPINS     1

#define NPRET 0
#define PRET  1

#define FALSE 0
#define TRUE  1

#define RANDKEY 0
#define THD_NUM 5  /* number of thread per run */
#define RUN_NUM 25 /* number of runs */


int main( int argc, char *argv[] )
{
    struct threadset thset[THD_NUM];
    unsigned int runs = RUN_NUM, j;
    MySKL_t l1, l2, l3;

    if ( !storeInput( argc, argv )             ||
         !initLc( )                            ||
         !(l1 = inittest( inputargs.maxLev1 )) ||
         !(l2 = inittest( inputargs.maxLev2 )) ||
         !(l3 = inittest( inputargs.maxLev2 ))
    ) exit( EXIT_FAILURE );


    printf ( "%sSTARTING THREAD TESTS...%s\n", s_test_colors[RED],
            s_test_colors[NONE] );

    while ( runs-- > 0 )
    {
        for ( j = 0; j < THD_NUM; j++ )
        {
            thset[j].l = l1;
            thset[j].id = j;

            if ( j % 2 )
            {
                thset[j].elem = inputargs.elemNu1;
                thset[j].oper = 1;
            }
            else
            {
                thset[j].elem = inputargs.elemNu2;
                thset[j].oper = 0;
            }
        }

        /* launch the threads */

        for ( j = 0; j < THD_NUM; j++ )
            pthread_create(
                &thset[j].aThread, NULL, worker, ( void* ) & thset[j]
            );

        MySKLcopy( l1, l2, copyfunc );
        /* MySKLjoinAD(l3, l2, copyfunc ); */

        /* the main thread waits until other threads finished */
        for ( j = 0; j < THD_NUM; j++ )
            pthread_join( thset[j].aThread, NULL );
    }

    /* all mem. should now be free'd */
    memorytest( );

    printf ( "\n%sTHREAD TESTS COMPLETED ...%s", s_test_colors[RED],
            s_test_colors[NONE] );

    printf ( "Library version: [ %s ]. ", MySKLversion() );
    printf ( "Bye!\n" );

    exit( EXIT_SUCCESS );
}
