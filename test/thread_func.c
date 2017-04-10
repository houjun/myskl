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

# include <string.h>
# include <stdio.h>
# include "func.h"
# include "thread_func.h"


/* workerArg is a pointer to something (a structure) that contains
 * the parameters useful to this function
 */

void * worker( void * workerArg )
{
    struct threadset * thinf = ( struct threadset * )workerArg;
    int id = thinf->id;
    MySKL_e err;
    int N;

    /* valgrind complains about printf(s) but, nevertheless,
     * we leave it
     */

    fprintf( stdout, "\n%sStarting worker: %d%s\n",
        s_test_colors[RED], id, s_test_colors[NONE] );

    /* MySKLverboseEnable(); */

    N = thinf->elem;

    while ( N-- > 0 )
    {
        DS_t data = { krand( ), irand( ), UNMARKED_DATA };
        LN_p item = malloc( sizeof ( LN_s ) );

        item->key = data.key;
        item->item = data.item;

        err = ( thinf->oper ?
            MySKLinsertAD( thinf->l, &(item->node) ):
            MySKLinsertND( thinf->l, &(item->node) )
        );

        ( thinf->oper ?
            MySKLdeleteAF( thinf->l, &(item->node), NULL ):
            MySKLsearch( thinf->l,  &(item->node), NULL )
        );

        if ( err != MYSKL_STATUS_OK ) free( item );
    }

    drawlist ( thinf->l );
    /* MySKLverboseDisable(); */

    fprintf( stderr, "%sWorker %d finished!%s\n",
        s_test_colors[RED], id, s_test_colors[NONE] );

    return ( void * )0;
}
