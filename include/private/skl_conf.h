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

# ifndef __SKL_CONFIG_H__
# define __SKL_CONFIG_H__

# if HAVE_CONFIG_H
#   include <config.h>
# endif

# include <stdio.h>

# ifdef __CYGWIN__
/*  we use a Unix API, so pretend it's not Windows */
#   undef WIN
#   undef WIN32
#   undef _WIN
#   undef _WIN32
#   undef _WIN64
#   undef __WIN__
#   undef __WIN32__
# else
#   ifdef _WIN32
#   include <windows.h>
/*  we supprime warning about sprintf unsafety
#   pragma warning(disable:4996)
#   pragma warning(disable:4311)
*/
#   endif
# endif

# if STDC_HEADERS || _LIBC
#   include <stdlib.h>
#   include <stddef.h>
#   include <string.h>
# else
#   ifdef HAVE_MALLOC_H
#     include <malloc.h>
#   endif
#   ifdef HAVE_STRINGS_H
#     include <strings.h>
#   endif
# endif

# if !HAVE_STRCHR && !_LIBC && !_WIN32
#   ifndef strchr
#     define strchr index
#   endif
#   ifndef strrchr
#     define strrchr rindex
#   endif
# endif

# if !HAVE_VSNPRINTF && HAVE___VSNPRINTF
#   undef vsnprintf
#   define vsnprintf __vsnprintf
#   define HAVE_VSNPRINTF 1
# endif

# if !HAVE_SNPRINTF && HAVE___SNPRINTF
#   undef snprintf
#   define snprintf  __snprintf
#   define HAVE_SNPRINTF
# endif

# if !HAVE_MEMCPY
#   define memcpy(d,s,n)  bcopy((s),(d),(n))
#   define memmove(d,s,n) bcopy((s),(d),(n))
# endif

# if !HAVE_BCOPY
#   define bcopy(s,d,n) memcpy((d),(s),(n))
#   define bzero(d,n)   memset((d),0,(n))
# endif

# if HAVE_UNISTD_H || _LIBC
#   include <unistd.h>
# endif

# if HAVE_ERRNO_H
#   include <errno.h>
# endif

# ifdef __STDC__
#   define STR(x)  #x
#   define CONC(x, y)  x##y
# else
#   define STR(x)     "x"
#   define CONC(x, y)  x/**/y
# endif

# endif /* __SKL_CONFIG_H__ */
