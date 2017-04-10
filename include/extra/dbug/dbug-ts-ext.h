
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

# ifndef _DBUG_TS_EXT_H
# define _DBUG_TS_EXT_H

# include <pthread.h>
# include "dbug.h"

# define THREAD_NAME_SIZE 100

extern void set_thread_var( const void *, pthread_key_t );
extern void * get_thread_var( pthread_key_t );
extern void del_thread_key( pthread_key_t );
extern const char * thread_name( void );

extern pthread_mutex_t THR_my_pthread_mutex_lock_dbug;
extern pthread_key_t THR_KEY_code_state;
extern pthread_key_t THR_KEY_dbug_state;

# endif	/* _DBUG_TS_EXT_H */
