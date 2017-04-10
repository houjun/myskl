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

/*=============================================================*/
/*                                                             */
/*            Public header file for the library               */
/*                        <myskl.h>                            */
/*                                                             */
/*=============================================================*/

# ifndef __MYSKL__
# define __MYSKL__

# ifdef __cplusplus
extern "C" {
# endif

/* some data types and definitions */
# include "skl_defs.h"

# include <stdlib.h>
# include <stdio.h>


/*=============================================*/
/*                                             */
/* returnable error codes                      */
/*                                             */
/*=============================================*/

typedef enum
{
   MYSKL_NODE_NOT_DELETED,
   MYSKL_MEM_EXHAUSTED,
   MYSKL_RANDGEN_ERROR,
   MYSKL_DUPLICATE_KEY,
   MYSKL_KEY_NOT_FOUND,
   MYSKL_KEY_FOUND,
   MYSKL_STATUS_OK,
   MYSKL_ERROR
}  MySKL_e;



/*=============================================*/
/*                                             */
/* typedefs to describe the library data types */
/*                                             */
/*=============================================*/

typedef void * MySKL_t;
typedef void * MySKL_n;
typedef void * MySKL_i;
typedef void * MySKL_d;

typedef struct MySKLnode
{
   MySKL_n next;
   # ifdef MYSKL_DOUBLE_LL_TYPE
   MySKL_n prev;
   # endif
   uint sz;
}  MySKL_ns;



/*====================================================================
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +                                                                   +
 +                                                                   +
 +                      PUBLIC FUNCTIONS                             +
 +                      ----------------                             +
 +                                                                   +
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ====================================================================*/


/*****************************************************************
 *                                                               *
 * Given a <ptr> pointer to a <MySKL_ns> type field declared as  *
 * <member> and contained into a structure of type <type>, this  *
 * macro returns a reference to such a structure.                *
 *                                                               *
 * ***************************************************************/

# define MySKLgetEntry(ptr, type, member) \
    ((type *)((char *)(ptr)-(ulong)(&((type *)0)->member)))



/*******************************************************************
 *                                                                 *
 *  =============================================================  *
 *                                                                 *
 *         Some notes about thread safety in this library          *
 *                                                                 *
 *  =============================================================  *
 *                                                                 *
 *   Concurrent operations (destroy1, destroy2, ...) on the same   *
 *   list:                                                         *
 *   {UNDEFINED} :                                                 *
 *                                                                 *
 *      MySKLdestroy( L, ... )                                     *
 *      MySKLdestroy( L, ... )                                     *
 *                                                                 *
 *   list L may have already been destroyed by a previous call     *
 *   to MySKLdestroy(), thus there is the risk to operate on a     *
 *   no more existent list.                                        *
 *                                                                 *
 *   Concurrent operations (searchAll1, searchAll2, ...) that      *
 *   use the same iterator:                                        *
 *   list:                                                         *
 *   {UNSAFE} :                                                    *
 *                                                                 *
 *      MySKLsearchALL( L1, &iter1 )                               *
 *      MySKLsearchALL( L2, &iter1 )                               *
 *                                                                 *
 *   Internally they use two different iterators but they are      *
 *   assigned to the same variable, thus it is no possible to      *
 *   know which iterator the variable refers to.                   *
 *                                                                 *
 *******************************************************************/


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                         CORE OPERATIONS
                             SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*****************************************************************
 *                                                               *
 * Initializes and returns an empty list with at most <k> levels *
 * provided that <k> is not greater than <MYSKL_MAXLEVELS> value *
 * If <err> is not NULL, it will contain information whether any *
 * error occured. Upon error, function returns NULL.             *
 *                                                               *
 * PARAMETERS:                                                   *
 *                                                               *
 * ( client defined - required ) : compare :                     *
 *   generic comparison for items. To work                       *
 *   properly should return these values :                       *
 *                                                               *
 *     (-1) if x < y,                                            *
 *     (0)  if x = y,                                            *
 *     (1)  if x > y                                             *
 *                                                               *
 * ( client defined - not required ) : dstfunc :                 *
 *   destroyer of a node                                         *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_RANDGEN_ERROR                                           *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_t MS_API( MySKLinit )
 (
    uint k,
    int (*compare )( const void * x, const void * y ),
    void (*dstfunc)( MySKL_n n ),
    MySKL_e * err
 );



/* ***************************************************************
 *                                                               *
 * Inserts the node <node> into the list <list>. 'MySKLinsertAD' *
 * allows inserting duplicate nodes (AD = allow duplicates); on  *
 * the contrary, 'MySKLinsertND' does not ( ND = no duplicates ).*
 *                                                               *
 * PARAMETERS:                                                   *
 *                                                               *
 * node: node to store into the list                             *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_DUPLICATE_KEY -- only insertND()                        *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API( MySKLinsertAD )
 (
    MySKL_t list,
    MySKL_n node
 );

MS_EXTERN MySKL_e MS_API( MySKLinsertND )
 (
    MySKL_t list,
    MySKL_n node
 );



/* ***************************************************************
 *                                                               *
 * Updates a node corresponding to the node <node> into the list *
 * <l>. In case of more than 1 node updateable, 'MySKLinsertUPD' *
 * replaces the 1° node found: differently, 'MySKLinsertUPD_ALL' *
 * replaces all nodes. If an updateable node was not found, both *
 * 'MySKLinsertUPD' and 'MySKLinsertUPD_ALL' insert <node> as a  *
 * new element into the list <l>.                                *
 *                                                               *
 * PARAMETERS:                                                   *
 *                                                               *
 * node: node to update (or store) into the list                 *
 *                                                               *
 * ( client defined - required ) : updfunc :                     *
 *   update node <n> by replacing it with <newnode> content      *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API ( MySKLinsertUPD )
 (
    MySKL_t l,
    MySKL_n node,
    void updfunc ( const void * n, const void * newnode )
 );

MS_EXTERN MySKL_e MS_API ( MySKLinsertUPD_ALL )
 (
    MySKL_t l,
    MySKL_n node,
    void updfunc ( const void * n, const void * newnode )
 );



/* ****************************************************************
 *                                                                *
 * Deletes node <n>. 'MySKLdeleteNF' ( NF = no forcing ) prevents *
 * a node from being deleted if <dstdatafunc> returns false (0).  *
 * Instead, 'MySKLdeleteAF' ( AF = allow forcing ) deletes a node *
 * regardeless of what <dstdatafunc> may return.                  *
 *                                                                *
 * PARAMETERS:                                                    *
 *                                                                *
 * ( client defined - not required ) : dstdatafunc :              *
 *   destroyer of a NODE'S DATA (NOT to be confused               *
 *   with the destroyer of a NODE, which was set by               *
 *   a previously call to MySKLinit). Must return a               *
 *   boolean value                                                *
 *                                                                *
 * STATUS CODES:                                                  *
 *                                                                *
 * MYSKL_MEM_EXHAUSTED                                            *
 * MYSKL_KEY_NOT_FOUND                                            *
 * MYSKL_NODE_NOT_DELETED -- only MySKLdeleteNF()                 *
 * MYSKL_STATUS_OK                                                *
 * MYSKL_ERROR                                                    *
 *                                                                *
 ******************************************************************/

MS_EXTERN MySKL_e MS_API( MySKLdeleteNF )
 (
    MySKL_t l,
    MySKL_n n,
    bool ( *dstdatafunc )( MySKL_n )
 );

MS_EXTERN MySKL_e MS_API( MySKLdeleteAF )
 (
    MySKL_t l,
    MySKL_n n,
    bool ( *dstdatafunc )( MySKL_n )
 );



/* ***************************************************************
 *                                                               *
 * Joins <source> list with <target> list. This operation does   *
 * not modify list <source>. <cpyfunc> copies the contents of a  *
 * node belonging to the list <source> to the new node is being  *
 * inserted into the list <target>. While 'MySKLjoinAD' allows   *
 * the presence of duplicate nodes, 'MySKLjoinND' does not.      *
 *                                                               *
 * ( client defined - required ) : cpyfunc :                     *
 *   makes and returns a copy of the node <n>                    *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API( MySKLjoinAD )
 (
    MySKL_t target,
    MySKL_t source,
    MySKL_n cpyfunc ( const void * n )
 );

MS_EXTERN MySKL_e MS_API( MySKLjoinND )
 (
    MySKL_t target,
    MySKL_t source,
    MySKL_n cpyfunc ( const void * n )
 );



/* ***************************************************************
 *                                                               *
 * Given a <source1> and <source2> list, yields a <target> list  *
 * containing only those nodes that are both in <source1> and in *
 * <source2>. The nodes to insert in the <target> list are taken *
 * from the <source1> list.                                      *
 *                                                               *
 * ( client defined - required ) : cpyfunc :                     *
 *   makes and returns a copy of the node <n>                    *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API( MySKLintersect )
 (
    MySKL_t source1,
    MySKL_t source2,
    MySKL_t target,
    MySKL_n cpyfunc ( const void * n )
 );



/* ***************************************************************
 *                                                               *
 * Copies <source> list into a <target> list.                    *
 *                                                               *
 * ( client defined - required ) : cpyfunc :                     *
 *   makes and returns a copy of the node <n>                    *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_MEM_EXHAUSTED                                           *
 * MYSKL_STATUS_OK                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API ( MySKLcopy )
 (
    MySKL_t source,
    MySKL_t target,
    MySKL_n cpyfunc ( const void * n )
 );



/* ***************************************************************
 *                                                               *
 * Locates the node <n> in the list <l>. If <found_node> is not  *
 * NULL, a pointer to the found node ( or NULL if not any ) is   *
 * assigned to the variable pointed to by <found_node>.          *
 * MySKLsearchALL instead locates all the keys <k> in the list   *
 * <l> and returns an iterator that may be used to list those    *
 * keys.                                                         *
 *                                                               *
 * STATUS CODES:                                                 *
 *                                                               *
 * MYSKL_KEY_NOT_FOUND                                           *
 * MYSKL_KEY_FOUND                                               *
 * MYSKL_ERROR                                                   *
 *                                                               *
 *****************************************************************/

MS_EXTERN MySKL_e MS_API( MySKLsearch )
 (
    MySKL_t l,
    MySKL_n nodetofound,
    MySKL_n * found_node
 );

MS_EXTERN MySKL_e MS_API ( MySKLsearchALL )
 (
    MySKL_t l,
    MySKL_n nodetofound,
    MySKL_i * iter
 );



/* ***************************************************************
 *                                                               *
 * Releases memory for the list <l> by using <destroy> function. *
 *                                                               *
 * NOTE: this function, in a case of concurrent runs on the same *
 * list <l>, is UNSAFE, as there is no guarantee that a previous *
 * operation has not already destroyed the list. For this reason *
 * this function should never be used in such a fashion.         *
 *                                                               *
 * PARAMETERS:                                                   *
 *                                                               *
 * ( client defined - not required ) : dstdatafunc :             *
 *   destroyer of a NODE'S DATA (NOT to be confused with the     *
 *   destroyer of a NODE, which was set by a previously call     *
 *   to MySKLinit). Must return a boolean value                  *
 *                                                               *
 *****************************************************************/

MS_EXTERN void MS_API( MySKLdestroy )
 (
    MySKL_t * l,
    void ( *destroy )( MySKL_n n )
 );



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                     ITERATOR OPERATIONS
                           SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* ***********************************************************
 *                                                           *
 * Constructs an iterator for the list <l>. The current iter *
 * is set to point on the first node in the list if <n> is a *
 * NULL value or to point to the node <n> otherwise.         *
 * MySKLdestroyIterator does the opposite since it destroys  *
 * the iterator <i>. Finally, MySKLresetIterator resets the  *
 * iterator <i>. The <n> value behaves just like described   *
 * above for the MySKLsetIterator function.                  *
 *                                                           *
 *************************************************************/

MS_EXTERN void MS_API( MySKLsetIterator )
 (
    MySKL_t l,
    MySKL_i * i,
    MySKL_n n
 );

MS_EXTERN void MS_API( MySKLresetIterator )
 (
    MySKL_i * i,
    MySKL_n n
 );

MS_EXTERN void MS_API( MySKLdestroyIterator )
 (
    MySKL_i * i
 );



/* ***********************************************************
 *                                                           *
 * Returns a pointer to the 'forward/backward' node pointed  *
 * to by the iterator at the level <k>. 1 <= k <= max_level  *
 * Notice that the second function is available only if the  *
 * macro 'MYSKL_DOUBLE_LL_TYPE' is defined (<skl_defs.h>)    *
 *                                                           *
 *************************************************************/

MS_EXTERN MySKL_n MS_API( MySKLgetNextNode )
 (
    MySKL_i * iter,
    uint k
 );

MS_EXTERN MySKL_n MS_API( MySKLgetPrevNode )
 (
    MySKL_i * iter,
    uint lev
 );



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                       MEMORY OPERATIONS
                            SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* ***********************************************************
 *                                                           *
 * Creates-destroies a sort of container that keeps track of *
 * all the initialized lists, so as to automatically dealloc *
 * them.                                                     *
 *                                                           *
 *************************************************************/

MS_EXTERN void MS_API( MySKLaddtoListsContainer)( MySKL_t l );
MS_EXTERN bool MS_API( MySKLinitListsContainer )( void );
MS_EXTERN void MS_API( MySKLfreeListsContainer )( void );



/* ***********************************************************
 *                                                           *
 * Returns TRUE if, the whole memory allocated for all the   *
 * lists created, was freed. FALSE is returned otherwise.    *
 *                                                           *
 *************************************************************/

MS_EXTERN bool MS_API( MySKLmemoryCleaned ) ( void );



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                     AUXILIARY OPERATIONS
                           SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* ***********************************************************
 *                                                           *
 * If ( k == 0 ) checks whether the whole list <l> is empty; *
 * otherwise ( k > 0 ) checks if list has any node at level  *
 * <k>, where 1 <= k <= max_level                            *
 *                                                           *
 *************************************************************/

MS_EXTERN bool MS_API( MySKLisEmpty )
 (
    MySKL_t,
    uint k
 );



/* ***********************************************************
 *                                                           *
 * Set of functions that grab informations from a list <l>.  *
 *                                                           *
 *************************************************************/

MS_EXTERN uint MS_API( MySKLgetMaxLevel ) ( MySKL_t l );
MS_EXTERN uint MS_API( MySKLgetCurLevel ) ( MySKL_t l );
MS_EXTERN uint MS_API( MySKLgetNodeNum )  ( MySKL_t l );



/* ***********************************************************
 *                                                           *
 * Gets respectively the head and the tail of a list <l>     *
 *                                                           *
 *************************************************************/

MS_EXTERN MySKL_n MS_API( MySKLgetHead )
 (
    MySKL_t l
 );

MS_EXTERN MySKL_n MS_API( MySKLgetTail )
 (
    MySKL_t l
 );



/*************************************************************
 *                                                           *
 * Returns a version string of the form <x.y.z>, where       *
 * x=major_version, y=minor_version, z=micro version         *
 *                                                           *
 *************************************************************/

MS_EXTERN const char * MS_API( MySKLversion )( void );



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                    SHOWING STRUCT OPERATIONS
                            SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*************************************************************
 *                                                           *
 *  Draws the list <l>, using a level-based representation   *
 *  Needs two client defined functions to do accomplish it.  *
 *  Notice that the output of this func straightly depends   *
 *  on the rightness of these implementation.                *
 *                                                           *
 *  PARAMETERS:                                              *
 *                                                           *
 *   ( client defined - required ) : sizeofData :            *
 *   returns length of the data contained in the node <n>    *
 *   Notice that a data is intended as a pair 'item-key'     *
 *                                                           *
 *   ( client defined - required ) : printData :             *
 *   prints the data contained in the node <n>               *
 *                                                           *
 *************************************************************/

MS_EXTERN void MS_API( MySKLdrawList )
 (
    MySKL_t l,
    int ( *sizeofData )( MySKL_n n ),
    void ( *printData )( MySKL_n n )
 );



/*************************************************************
 *                                                           *
 * Displayes some information about the structure of <l>,    *
 * i.e. node and link addresses. It's not to be intended     *
 * as a clone of MySKLdrawList().                            *
 *                                                           *
 *************************************************************/

MS_EXTERN void MS_API( MySKLdisplayListStruct )
 (
    MySKL_t l
 );



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                      VERBOSITY OPERATIONS
                            SECTION

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

/*************************************************************
 *                                                           *
 * Enable/Disable and Suspend/Resume verbosity: the first    *
 * function accepts a custom control string, that's to say   *
 * a set of commands (flags) that specify the features to    *
 * enable. For a list of options, you should refer to line   *
 * 413 of the file <src/extra/dbug/dbug.c>                   *
 *                                                           *
 * Both MySKLCustomVerboseEnable() and MySKLverboseEnable()  *
 * return a TRUE value if verbosity wasn't already enabled,  *
 * FALSE otherwise. MySKLverboseResume() acts in the same    *
 * way.                                                      *
 *                                                           *
 * Both MySKLverboseDisable() & MySKLverboseSuspend() return *
 * a TRUE value if the verbosity was already enabled, FALSE  *
 * otherwise.                                                *
 *                                                           *
 *************************************************************/

MS_EXTERN bool MS_API( MySKLCustomVerboseEnable )
 (
    const char * ctr
 );

MS_EXTERN bool MS_API( MySKLverboseDisable )( void );
MS_EXTERN bool MS_API( MySKLverboseEnable ) ( void );

MS_EXTERN bool MS_API( MySKLverboseSuspend )( void );
MS_EXTERN bool MS_API( MySKLverboseResume ) ( void );

# ifdef __cplusplus
}
# endif  /* __cplusplus */
# endif  /* __MYSKL__ */
