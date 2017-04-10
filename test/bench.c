

/*
 * This file was taken from the Transcendental Technical Travails
 * blog (http://www.canonware.com/rb/) and slightly modified to
 * fulfill this library needs. The main one was to have a way to
 * test the performance of this library. Since the same kind of
 * code is also used to gauge the performance of a good redblack
 * tree implementation, automatically it offers a way to make an
 * empirical performance evaluation of this library.
 */

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>

# define NDEBUG
# include <assert.h>
# include <myskl.h>

# define NNODES 1500
# define NSETS 25
# define NSEARCH 0
# define NITER 0
# define MAX_LEVEL 20


int cmpfunc ( const void * x, const void * y );
int getSizeofData ( MySKL_n n );
void printData ( MySKL_n n );

/* #define VERBOSE */

typedef struct node_s node_t;

struct node_s {
#define NODE_MAGIC 0x9823af7e
    uint32_t magic;
    MySKL_ns node;
    long key;
};


int cmpfunc ( const void * x, const void * y )
{
    node_t * a = MySKLgetEntry ( x, node_t, node );
    node_t * b = MySKLgetEntry ( y, node_t, node );

    return (( a->key > b->key )-( a->key < b->key ));
}


int getSizeofData ( MySKL_n n )
{
    long base = 10, j = 1;
    long key;

   key = MySKLgetEntry ( n, node_t, node )->key;

   while ( key >= base )
   { base *= 10; j++; }

   /* +3 is to also count  '[', ']', '|' */
   return j + 2;
}


void printData ( MySKL_n n )
{
   printf ( "[%ld]", MySKLgetEntry ( n, node_t, node )->key );
}



int main(void) {

    MySKL_t list;
    long set[NNODES];
    node_t nodes[NNODES];
    unsigned i, j, k;

    /* MySKLverboseEnable(); */
    srand(42);

    for (i = 0; i < NSETS; i++) {
	for (j = 0; j < NNODES; j++) {
	    set[j] = (long) (((double) NNODES)
	      * ((double) rand() / ((double)RAND_MAX)));
	}

	for (j = 1; j <= NNODES; j++) {
#ifdef VERBOSE
	    fprintf(stderr, "Tree %u, %u node%s\n", i, j, j != 1 ? "s" : "");
#endif

	    /* Initialize list and nodes. */
            list = MySKLinit( MAX_LEVEL, cmpfunc, NULL, NULL );

	    for (k = 0; k < j; k++) {
		nodes[k].magic = NODE_MAGIC;
		nodes[k].key = set[k];
	    }

	    /* Insert nodes. */
	    for (k = 0; k < j; k++) {
                MySKLinsertND(list, &(nodes[k].node));
                /* MySKLdrawList ( list, getSizeofData, printData );*/
	    }

	    /* Remove nodes. */
	    for (k = 0; k < j; k++) {
              # if NSEARCH > 0
                unsigned l, m;
		for (l = 0; l < NSEARCH; l++) {
                    MySKL_n nfound;
		    for (m = 0; m <= k; m++) {
                        MySKLsearch(list, &(nodes[m].node), &nfound );
		    }
		}
              # endif

                MySKLdeleteNF( list, &nodes[k].node, NULL );
		nodes[k].magic = 0;
	    }
	}
    }

    return 0;
}
