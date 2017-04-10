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

# ifndef __MYSKL_TYPES_H__
# define __MYSKL_TYPES_H__

# ifdef __GNUC__
#   ifndef const
#     define const  __const
#   endif
#   ifndef signed
#     define signed __signed
#   endif
#   ifndef volatile
#     define volatile __volatile
#   endif
# else   /* Not GCC */
#   ifdef __STDC__
#     undef  signed
#     define signed
#     undef  volatile
#     define volatile
#   endif
# endif

/* The AC_CHECK_SIZEOF() in configure fails for some machines
 * These macros provide some fallback values: also useful for
 * win32 system
 */

# if !SIZEOF_UNSIGNED_SHORT
#   undef SIZEOF_UNSIGNED_SHORT
#   define SIZEOF_UNSIGNED_SHORT 2
# endif

# if !SIZEOF_SIGNED_SHORT
#   undef SIZEOF_SIGNED_SHORT
#   define SIZEOF_SIGNED_SHORT 2
# endif

# if !SIZEOF_UNSIGNED_INT
#   undef SIZEOF_UNSIGNED_INT
#   define SIZEOF_UNSIGNED_INT 4
# endif

# if !SIZEOF_SIGNED_INT
#   undef SIZEOF_SIGNED_INT
#   define SIZEOF_SIGNED_INT 4
# endif

# if !SIZEOF_UNSIGNED_LONG
#   undef SIZEOF_UNSIGNED_LONG
#   define SIZEOF_UNSIGNED_LONG 4
# endif

# if !SIZEOF_LONG_LONG
#   undef SIZEOF_LONG_LONG
#   define SIZEOF_LONG_LONG 8
# endif

# if !SIZEOF_LONG
#   undef SIZEOF_LONG
#   define SIZEOF_LONG 4
# endif

# if !SIZEOF_UNSIGNED_CHAR
#   undef SIZEOF_UNSIGNED_CHAR
#   define SIZEOF_UNSIGNED_CHAR 1
# endif

# if !SIZEOF_SIGNED_CHAR
#   undef SIZEOF_SIGNED_CHAR
#   define SIZEOF_SIGNED_CHAR 1
# endif

# if !SIZEOF_CHAR
#   undef SIZEOF_CHAR
#   define SIZEOF_CHAR 1
# endif

# include <sys/types.h>


/* ==================================================
 *
 * Portable type declarations
 * (Typdefs for easier portability)
 *
 * ==================================================
 */

# ifndef HAVE_UINT32
#   undef uint32
#   ifdef HAVE_UINT32_T
typedef uint32_t uint32;
#   elif SIZEOF_INT == 4
typedef unsigned int uint32;
#   elif SIZEOF_LONG == 4
typedef unsigned long uint32;
#   else
#     error No typedef for uint32
#   endif
# define HAVE_UINT32
# endif /* HAVE_UINT32 */

# ifndef HAVE_INT32
#   undef int32
#   ifdef HAVE_INT32_T
typedef int32_t int32;
#   elif SIZEOF_INT == 4
typedef signed int int32;
#   elif SIZEOF_LONG == 4
typedef signed long int32;
#   else
#     error No typedef for int32
#   endif
# define HAVE_INT32
# endif /* HAVE_INT32 */

# ifndef HAVE_INT64
#   undef int64
#   ifdef HAVE_INT64_T
typedef int64_t int64;
#   elif SIZEOF_INT == 8
typedef signed int int64;
#   elif SIZEOF_LONG == 8
typedef signed long int64;
#   elif SIZEOF_LONG_LONG == 8
typedef signed long long int64;
#   else
#     error No typedef for int64
#   endif
# define HAVE_INT64
# endif /* HAVE_INT64 */

# ifndef HAVE_UINT64
#   undef uint64
#   ifdef HAVE_UINT64_T
typedef uint64_t uint64;
#   elif SIZEOF_INT == 8
typedef unsigned int uint64;
#   elif SIZEOF_LONG == 8
typedef unsigned long uint64;
#   elif SIZEOF_LONG_LONG == 8
typedef unsigned long long uint64;
#   else
#     error No typedef for uint64
#   endif
# define HAVE_UINT64
# endif /* HAVE_UINT64 */

# ifndef HAVE_INT16
#   undef int16
#   ifdef HAVE_INT16_T
typedef int16_t int16;
#   elif SIZEOF_SIGNED_INT == 2
typedef signed int int16;
#   elif SIZEOF_SIGNED_SHORT == 2
typedef signed short int16;
#   else
#     error No typedef for int16
#   endif
# define HAVE_INT16
# endif /* HAVE_INT16 */

# ifndef HAVE_UINT16
#   undef uint16
#   ifdef HAVE_UINT16_T
typedef uint16_t uint16;
#   elif SIZEOF_UNSIGNED_INT == 2
typedef unsigned int uint16;
#   elif SIZEOF_UNSIGNED_SHORT == 2
typedef unsigned short uint16;
#   else
#     error No typedef for uint16
#   endif
# define HAVE_UINT16
# endif /* HAVE_UINT16 */

# ifndef HAVE_INT8
#   undef int8
#   ifdef HAVE_INT8_T
typedef int8_t int8;
#   elif SIZEOF_SIGNED_CHAR == 1
typedef signed char int8;
#   else
#     error No typedef for uint16
#   endif
# define HAVE_INT8
# endif /* HAVE_INT8 */

# ifndef HAVE_UINT8
#   undef uint8
#   ifndef HAVE_UINT8_T
typedef unsigned char uint8;
#   else
typedef uint8_t uint8;
#   endif
# define HAVE_UINT8
# endif /* HAVE_UINT8 */


/* Enable uint on systems that don't have it */
/* (1) <warning: typedef redeclared: uint> in sunstudio */

# if !defined (HAVE_UINT) && !defined(__SUNPRO_C)   /* avoid (1) */
#   undef HAVE_UINT
typedef unsigned int uint;
#   define HAVE_UINT
# endif

/* Enable ulong on systems that don't have it */
/* (1) <warning: typedef redeclared: uint> in sunstudio */

# if ! defined (HAVE_ULONG) && !defined(__SUNPRO_C) /* avoid (1) */
#   undef ulong
typedef unsigned long ulong;
#   define HAVE_ULONG
# endif

/* Other helper macros */

# define DIM(a) (sizeof(a)/sizeof((a)[0]))
# define IMPLEMENT_BOOL_AS_ENUM

# if defined (IMPLEMENT_BOOL_AS_CHAR) || defined(_WIN32)
#   ifndef bool
#     define bool char
#    endif
#   ifndef FALSE
#     define FALSE 0
#   endif
#   ifndef TRUE
#     define TRUE 1
#   endif
# endif

# if defined (IMPLEMENT_BOOL_AS_ENUM) && !defined(_WIN32)
#   ifndef bool
      enum _bool { FALSE, TRUE };
#     define bool enum _bool
#   endif
# endif

/* import|export win dll dynamically */

# define MS_API(func) func
# ifdef _WIN32
#  ifdef DLL_EXPORT
#    define MS_EXTERN __declspec(dllexport)
#  else
#    ifdef MYSKL_DLL_IMPORT
#      define MS_EXTERN extern __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef MS_EXTERN
#  define MS_EXTERN extern
# endif

# ifndef HAVE_LIBGC
#   define ALLOC   malloc
#   define REALLOC realloc
#   define DEALLOC free
# else
 #  define DEALLOC GC_FREE
 #  define REALLOC GC_REALLOC
 #  define ALLOC   GC_MALLOC
#   include <gc.h>
# endif


/* ============================================== */
/* Some configuration macros                      */
/* ============================================== */

# ifndef PACKAGE_STRING
#   define PACKAGE_STRING "myskl 0.2.1"
# endif

# ifndef PACKAGE_VERSION
#   define PACKAGE_VERSION "0.2.1"
# endif

# ifndef PACKAGE_NAME
#   define PACKAGE_NAME "MySKL"
# endif


/* NOTE: whether to use warnings or assertions  */
/* when a kind of error occurs                  */

# define CHECK_DATA_WARN_ON_ERROR
# define CHECK_MEMO_EXIT_ON_ERROR


/* NOTE: choose the readers/writers implementation */
/* THREAD_READERS_WRITERS_V2                       */

# define THREAD_READERS_WRITERS_V1


/* NOTE: decomment to disable DBUG tracing   */
/* (if enabled by configure)                 */
/* # define DBUG_OFF                         */


/* set the skiplist max height (level)       */
/* a height too high is not advisable        */

# define MYSKL_MAXLEVELS 20


/* NOTE: decomment to use double linked list */
/* # define MYSKL_DOUBLE_LL_TYPE             */


/* NOTE: use a pseudorandom number generator */
/* for assigning the height of a node        */
/* NOTE: assign the height of a node without */
/* using a pseudorandom number generator     */

# define MYSKL_USE_RAND
/* # define MYSKL_NO_RAND */

# endif /* __MYSKL_TYPES_H__ */
