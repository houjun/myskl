
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

# define _PROTO_PRIVATE_VERSION
# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_DBG
# define _PROTO_PRIVATE_LC

# include "skl_priv.h"
# include <stdarg.h>

# define ASSERT_FAIL  "Exiting (due to %s ASSERTION FAIL)...\n"
# define FATAL_ERROR  "Exiting (due to %s FATAL ERROR)...\n"
# define WARN_ERROR   "\nReturning...."
# define BEFORE_EXIT  "Cleaning before exiting...\n"

# ifdef THREAD
    struct _skl_mstat_st _skl_mstat = { 0, 0, 0,
        PTHREAD_MUTEX_INITIALIZER };
# else
    struct _skl_mstat_st _skl_mstat = { 0, 0, 0 };
# endif



/** *************************************************************
 *                                                              *
 * stampa su stderr una lista variabile di argomenti secondo un *
 * formato predefinito                                          *
 *                                                              *
 ** *************************************************************/

static void _printErr ( const char * progname, const char
    * routine, const char * format, va_list ap )
{
    fflush ( stdout );
    fflush ( stderr );

    if ( progname )
    {
        routine ? fprintf ( stderr, "%s (%s) : ", progname,
            routine ) : fprintf ( stderr, "%s : ", progname );
    }
    else
    if ( routine ) fprintf ( stderr, "(%s) : ", routine );

    vfprintf ( stderr, format, ap );
    fflush ( stderr );
}



/** *************************************************************
 **************************************************************
 +                                                            +
 +                                                            +
 +               FUNZIONE/PROCEDURE PRIVATE                   +
 +                  usabili in altri file                     +
 +                                                            +
 +                                                            +
 **************************************************************
 * **************************************************************/

/* ++++++ */

# define _CONST_CH_NUM  1 /* const chars number */
# define _BUFFSIZE     32

const char * _sklVersion ( void )
{
    static const char * version = NULL;
    static char buf[ _BUFFSIZE ];

    if ( version == NULL )
    {
        # ifndef HAVE_SNPRINTF
        memset ( buf, 0, sizeof( buf ) );

        if (
            strlen ( PACKAGE_NAME ) + _CONST_CH_NUM
            + strlen ( PACKAGE_VERSION ) < _BUFFSIZE
            ) sprintf (
                buf, "%s-%s", PACKAGE_NAME,
                PACKAGE_VERSION
                );
        # else

        snprintf (
            buf, sizeof( buf ), "%s-%s",
            PACKAGE_NAME, PACKAGE_VERSION
            );
        # endif
        version = buf;
    }

    return version;
}



/* ++++++ */

void _sklAssertFail ( const char * fname,
    const char * func, uint line,
    const char * expr )
{
    fprintf ( stderr,
        "%s : "
        "file  <%s> : function  <%s()> : "
        "line %u>\n"
        "\tAssertion ( %s ) failed!\n",
        PACKAGE_NAME, fname,
        func, line, expr
        );

    fprintf ( stderr, BEFORE_EXIT );
    _destroyListsContainer ();

    fprintf ( stderr, ASSERT_FAIL, PACKAGE_NAME );
    abort ();
}



/* ++++++ */

void _sklFatal ( const char * routine,
    const char * format, ... )
{
    va_list ap;

    va_start ( ap, format );

    _printErr ( PACKAGE_NAME " :", routine, format, ap );
    fprintf ( stderr, BEFORE_EXIT );

    _destroyListsContainer ();
    va_end ( ap );

    fprintf ( stderr, FATAL_ERROR, PACKAGE_NAME );
    exit ( EXIT_FAILURE );
}



/* ++++++ */

# ifndef NWARNING
void _sklWarning ( const char * routine,
    const char * format, ... )
{
    va_list ap;

    va_start ( ap, format );

    fprintf ( stderr, "\n#############\n" );
    _printErr ( NULL, routine, format, ap );

    va_end ( ap );
    fprintf ( stderr, WARN_ERROR );

    fprintf ( stderr, "\n#############\n" );
    return;
}
# endif



/* ++++++ */

void _sklFreeMemBlocks ( void * p, ... )
{
    void * argl;
    va_list ap;

    va_start ( ap, p );
    argl = va_arg ( ap, void * );

    if ( p != NULL ) { X_FREE ( p ); }
    while ( argl != NULL )
    {
        X_FREE ( argl );
        argl = va_arg ( ap, void * );
    }
}



/* ++++++ */

void * _no_db_alloc( size_t size )
{
    void * ptrmem;

    if ( ( ptrmem = malloc ( size ) ) == NULL )
        return NULL;

    mutex_lock ( _skl_mstat.mlock );

    _skl_mstat.alloc_count++;
    _skl_mstat.alloc_total_size += size;

    mutex_unlock ( _skl_mstat.mlock );
    return ptrmem;
}



/* ++++++ */

void _no_db_free( void * x )
{
    mutex_lock ( _skl_mstat.mlock );

    _skl_mstat.freed_count++;

    mutex_unlock( _skl_mstat.mlock );
    free ( x );
}
