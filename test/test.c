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
# include <string.h>
# include <stdio.h>

# include "func.h"

# define NOFORCEDEL 0
# define FORCEDEL   1

# define NODUPINS   0
# define DUPINS     1

# define NPRET 0
# define PRET  1

# define FALSE 0
# define TRUE  1

# define RANDKEY 0


int main( int argc,char *argv[] )
{
    MySKL_t l1, l2;
    MySKL_t l3, l4;
    MySKL_t l5;
    MySKL_t l_copy;

    if ( !storeInput ( argc, argv ) ) exit ( EXIT_FAILURE );

    printf ( "\n%sSTARTING TESTS (NO THREADS)...%s",
            s_test_colors[RED],
            s_test_colors[NONE]
    );

    /* ++++++++++++++++++++++++ INIT TEST +++++++++++++++++++++++++++ */

    printtext( "INITIALIZATING ALL LISTS", NPRET );

    MySKLverboseEnable();
    if ( ! initLc () ) exit ( EXIT_FAILURE );

    if ( ! ( l1 = inittest ( inputargs.maxLev1 ) ) ) exit ( 1 );
    if ( ! ( l2 = inittest ( inputargs.maxLev2 ) ) ) exit ( 1 );
    if ( ! ( l3 = inittest ( inputargs.maxLev1 ) ) ) exit ( 1 );
    if ( ! ( l4 = inittest ( inputargs.maxLev2 ) ) ) exit ( 1 );
    if ( ! ( l5 = inittest ( inputargs.maxLev2 ) ) ) exit ( 1 );
    if ( ! ( l_copy = inittest ( inputargs.maxLev2 ) ) ) exit ( 1 );


    /* ++++++++++++++++++++++ COPY TEST 1 +++++++++++++++++++++++++++ */

    printtext( "COPYING THE LIST 'l1'", PRET );

    copytest( l1, l_copy );
    drawlist ( l_copy );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l1 );


    /* ++++++++++++++++++++++ INSERT TEST 1 +++++++++++++++++++++++++ */

    printtext( "INSERTING INTO THE LIST 'l1' - DUPS ALLOWED", PRET );
    inserttest ( l1, inputargs.elemNu1, DUPINS );

    printtext( "DRAWING THE LIST 'l1'", PRET );
    drawlist ( l1 );


    /* ++++++++++++++++++++++ COPY TEST 2 +++++++++++++++++++++++++++ */

    printtext( "REPEATING COPY OF THE LIST 'l1'", PRET );

    copytest(l1, l_copy );
    drawlist ( l_copy );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l1 );


    /* ++++++++++++++++++++++ INSERT TEST 2 +++++++++++++++++++++++++ */

    printtext( "INSERTING INTO THE LIST 'l2' - DUPS ALLOWED", PRET );
    inserttest ( l2, inputargs.elemNu2, DUPINS );

    printtext( "DRAWING THE LIST 'l2'", PRET );
    drawlist ( l2 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l2 );


   /* ++++++++++++++++++++++ INTERSECT TEST +++++++++++++++++++++++++ */

    printtext( "INTERSECTING THE LIST 'l1' WITH 'l2' - NO DUPS ALLOWED", PRET );
    intersecttest(l1, l2, l5, NODUPINS );
    drawlist ( l5 );


   /* ++++++++++++++++++++++ ITERATOR TEST ++++++++++++++++++++++++++ */

    printtext ( "ITERATING THROUGH THE LIST 'l1': FORWARD THEN BACKWARD", PRET );
    itertest ( l1 );


    /* +++++++++++++++++++++++ JOIN TEST 1 ++++++++++++++++++++++++++ */

    printtext ( "JOINING THE LIST 'l2' WITH 'l1' - DUPS ALLOWED", PRET );
    jointest ( l1, l2, DUPINS );

    printtext( "DRAWING THE NEW LIST 'l1'", PRET );
    drawlist ( l1 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l1 );


    /* ++++++++++++++++++++++ SEARCH TEST 1 +++++++++++++++++++++++++ */

    printtext( "SEARCHING KEY IN THE LIST 'l1': SEARCH ALL", PRET );
    searchtestall ( l1, 7 );


    /* ++++++++++++++++++++++ INSERT TEST 2 +++++++++++++++++++++++++ */

    printtext( "INSERTING INTO THE LIST 'l3' - NO DUPS ALLOWED", PRET );
    inserttest ( l3, inputargs.elemNu1, NODUPINS );

    printtext( "DRAWING THE LIST 'l3'", PRET );
    drawlist ( l3 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l3 );


    /* +++++++++++++++++++++++ JOIN TEST 2 ++++++++++++++++++++++++++ */

    printtext ( "JOINING THE LIST 'l2' WITH 'l3' - NO DUPS ALLOWED", PRET );
    jointest ( l3, l2, NODUPINS );

    printtext( "DRAWING THE NEW LIST 'l3'", PRET );
    drawlist ( l3 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l3 );


    /* ++++++++++++++++++++++ DELETE TEST 1 +++++++++++++++++++++++++ */

    printtext( "DELETING KEY FROM THE LIST 'l3' - DO NOT FORCE THE DELETION", PRET );
    deletetest ( l3, NOFORCEDEL );

    printtext( "DRAWING THE NEW LIST 'l3'", PRET );
    drawlist( l3 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l3 );


    /* ++++++++++++++++++++++ DELETE TEST 2 +++++++++++++++++++++++++ */

    printtext( "DELETING KEY FROM THE LIST 'l3' - FORCE THE DELETION", PRET );
    deletetest ( l3, FORCEDEL );

    printtext( "DRAWING THE NEW LIST 'l3'", PRET );
    drawlist( l3 );


    /* +++++++++++++++++++++++ DRAW STRUCT  +++++++++++++++++++++++++ */

    printtext( "DRAWING THE NEW LIST 'l3' - STRUCT", PRET );
    MySKLdisplayListStruct ( l3 );

    printtext ( "Summary:", NPRET );
    printlistinfo ( l3 );


    /* ++++++++++++++++++++++ SEARCH TEST 2 +++++++++++++++++++++++++ */

    printtext( "SEARCHING KEY IN THE LIST 'l3: SEARCH FIRST'", PRET );
    searchtest ( l3 );


    /* ++++++++++++++++++++++ SEARCH TEST 3 +++++++++++++++++++++++++ */

    printtext( "SEARCHING KEY IN THE LIST 'l3': SEARCH ALL", PRET );
    searchtestall ( l3 /*l3*/, RANDKEY );


    /* ++++++++++++++++++++++  MEMORY TEST ++++++++++++++++++++++++++ */

    printtext( "FREEING ALL MEMORY", PRET );
    memorytest ();  /* all mem. should now be free'd */


    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    printf ( "\nLibrary version: [ %s ]. ", MySKLversion() );
    printf ( "Bye!\n" );

    exit ( EXIT_SUCCESS );
}
