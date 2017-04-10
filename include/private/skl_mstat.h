
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

# ifndef __SKL_MSTAT_H__
# define __SKL_MSTAT_H__

# ifdef THREAD
#   include <pthread.h>
# endif

# include "skl_defs.h"


/* This structure is used either with DBUG macro defined or not
 * and will contain poor statistics about memory allocation and
 * deallocation
 */

struct _skl_mstat_st
{
    ulong alloc_total_size,
          alloc_count,
          freed_count;

    # ifdef THREAD
    pthread_mutex_t mlock;
    # endif
};

/* skl_priv.c */
extern struct _skl_mstat_st _skl_mstat;

# endif	/* __SKL_MSTAT_H__ */
