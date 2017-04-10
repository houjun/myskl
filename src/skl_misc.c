
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
# define _PROTO_PUBLIC_MISC

# include <stdlib.h>
# include "skl_priv.h"

# define FUNC_NOT_AVAIL                                       \
    "Warning: %s(): DBUG not available here: macro DBUG_OFF " \
    "defined.\nYou can undefine it in 'skl.defs.h', provided "\
    "that DBUG support was enabled by <configure>.\n"



/** *************************************************************
 ** *************************************************************
 +                                                              +
 +                                                              +
 +              FUNZIONI/PROCEDURE PUBBLICHE                    +
 +                                                              +
 +                                                              +
 ** *************************************************************
 ** *************************************************************/

bool MySKLisEmpty ( MySKL_t l, uint k )
{
    bool ret;
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, TRUE );
    EXP_BC_RETV ( k <= l->maxlevel, TRUE );

    READER_ENTER( l );
    if ( k > 0 )
    {
        /* controllo su liv. k */

        ret = ( l->header->next[ --k ] == NIL( l ) );

        READER_EXIT( l );
        X_RETURN ( ret );
    }

    /* controllo sull'intera lista l */

    ret = ( l->nodenum == 0 );

    READER_EXIT( l );
    X_RETURN ( ret );
}



MySKL_n MySKLgetHead ( MySKL_t l )
{
    MySKL_n node;
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, NULL );
    READER_ENTER( l );

    node = (
        l->header->next[ 0 ] != NIL ( l ) ?
        l->header->next[ 0 ] : NULL
    );

    READER_EXIT( l );
    X_RETURN( node );
}



MySKL_n MySKLgetTail ( MySKL_t l )
{
    MySKL_n node;
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, NULL );
    READER_ENTER( l );

    node = ( l->tail != NIL ( l ) ? l->tail : NULL );

    READER_EXIT( l );
    X_RETURN( node );
}



uint MySKLgetMaxLevel ( MySKL_t l )
{
    uint ret;
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, 0 );
    READER_ENTER( l );

    ret = l->maxlevel;

    READER_EXIT( l );
    X_RETURN ( ret );
}



uint MySKLgetCurLevel ( MySKL_t l )
{
    uint ret;
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, 0 );
    READER_ENTER( l );

    ret = l->curlevel;

    READER_EXIT( l );
    X_RETURN ( ret );
}



uint MySKLgetNodeNum ( MySKL_t l )
{
    uint ret;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    LDN_BC_X_RETV ( l, DSKIP, NSKIP, 0 );
    READER_ENTER( l );

    ret = l->nodenum;

    READER_EXIT( l );
    X_RETURN ( ret );
}



bool MySKLCustomVerboseEnable ( const char * control )
{
# ifndef DBUG_OFF
    if ( !X_DEBUG_IS_ON )
    {
        if ( ( control == NULL ) || ( *control == '\0' ) )
            DBUG_PUSH ( "d:t:L:F" );
        else
            DBUG_PUSH ( control );

        X_DEBUG_ON;
        return TRUE;
    }
# else
    control = NULL;
    warnfna ( FUNC_NOT_AVAIL );
# endif

    return FALSE;
}



bool MySKLverboseEnable ( void )
{
# ifndef DBUG_OFF
    return MySKLCustomVerboseEnable ( NULL );
# else
    warnfna ( FUNC_NOT_AVAIL );
    return TRUE;
# endif
}



bool MySKLverboseDisable ( void )
{
# ifndef DBUG_OFF

    if ( X_DEBUG_IS_ON )
    {
        X_DEBUG_OFF; X_DEBUG_END();
        return TRUE;
    }
    else return FALSE;
# else

    warnfna ( FUNC_NOT_AVAIL );
    return FALSE;

# endif
}



bool MySKLverboseSuspend ( void )
{
# ifndef DBUG_OFF

    if ( X_DEBUG_IS_ON )
    {
        X_DEBUG_OFF;
        return TRUE;
    }
    else return FALSE;
# else

    warnfna ( FUNC_NOT_AVAIL );
    return FALSE;

# endif
}



bool MySKLverboseResume ( void )
{
# ifndef DBUG_OFF

    if ( !X_DEBUG_IS_ON )
    {
        X_DEBUG_ON;
        return TRUE;
    }
    else return FALSE;
# else
    warnfna ( FUNC_NOT_AVAIL );
    return TRUE;
# endif
}



bool MySKLmemoryCleaned ( void )
{
    X_ENTER ( THIS_IS_ME );

# ifndef HAVE_LIBGC
    X_RETURN (
       _skl_mstat.alloc_count ==
       _skl_mstat.freed_count
    );
# else
    X_RETURN( TRUE );

# endif
}



const char * MySKLversion ()
{
    const char * version;
    X_ENTER ( THIS_IS_ME );

    version = _sklVersion();
    X_RETURN ( version );
}

/* TODO: add functions MySKLVerboseAddCtrl and MySKLVerboseRemoveCtrl
 * they will use push() and pop()
 */
