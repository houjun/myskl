
/* source public library header */

#include <myskl.h>

/* auxliary test functions -- test_func.c */

extern void inserttest( MySKL_t, int, bool );
extern void jointest( MySKL_t, MySKL_t, bool );
extern void deletetest( MySKL_t, bool );
extern void destroytest( MySKL_t * );
extern void searchtest( MySKL_t );
extern MySKL_t inittest( int );
extern void memorytest( void );

extern int cmpfunc( const void *, const void * );
extern int cmpitem( const void *, const void * );
extern MySKL_n copyfunc ( const void * );
extern void dstnode ( MySKL_n );

extern void intersecttest( MySKL_t, MySKL_t, MySKL_t, bool );
extern void copytest( MySKL_t l1, MySKL_t );
extern void searchtestall( MySKL_t, int );

extern void printlistinfo( MySKL_t );
extern int getSizeofData( MySKL_n );
extern void destroyData( MySKL_d );
extern void printData( MySKL_n );
extern void drawlist( MySKL_t );
extern void itertest( MySKL_t );
extern bool initLc( void );

extern void printtext( const char *, char );
extern bool storeInput( int, char ** );
extern void wrapper( void );
extern int irand( void );
extern int krand( void );


#define LISTINFO "Current level: %d\nMax level: %d\nNode num: %d\n"
#define LCERROR "Failed initializing lists container, exiting...\n"

#define ERRARG "%s: error: five digits requested: " \
  "<maxlev1> <maxlev2> <#elem1> <#elem2> <keythreshold>\n"

#define SEARCHRES1 "Search test: key <%d> not found!\n"
#define SEARCHRES2 "Search test: key <%d> found!\n"

#define DELRES1 "Delete test: Key <%d> was deleted!\n"
#define DELRES2 "Delete test: Key <%d> was not found!\n"
#define DELRES3 "Delete test: Shared data! Can't delete!\n"

#define INITRES1 "Init test: ERROR OCCURED!\n\n"
#define INITRES2 "Init test: done!\n\n"

#define INSRES1 "Insert test: ERROR OCCURED!\n"
#define WHATINS "\nkey: \t<%d>\t item: \t<%d>\n\n"

#define JOINRES1 "Join test: ERROR OCCURED!\n"
#define JOINRES2 "Join test: done!\n"

#define MEMSTAT1 "Memory status: OK, %sfreed%s!\n"
#define MEMSTAT2 "Memory status: %sleak%s!\n"

#define PRET_MSG "\n\nPRESS RETURN TO CONTINUE: "
#define NODUPINS  0
#define DUPINS    1

#define FALSE 0
#define TRUE  1

#define INTERACTIVE_MODE FALSE
#define ITEMSIZE sizeof( int )
#define KEYSIZE  sizeof( int )
#define UNMARKED_DATA  0
#define MARKED_DATA    1
#define ITYPE int
#define IKEY  int
#define ARGNUM 5

#define LIGHT_RED  0
#define RED        1
#define MAGENTA    2
#define GREEN      3
#define BLUE       4
#define NONE       5

static const char * s_test_colors[] = {
    "\033[01;31m", /* ligh red */
    "\033[22;31m", /* red */
    "\033[22;35m", /* magenta */
    "\033[22;32m", /* green */
    "\033[01;34m", /* blue */
    "\033[0;0m"
};

typedef struct data_struct
{
    int key;
    int item;
    int marked;
} DS_t, * DS_p;

struct input
{
    int maxLev1, maxLev2;
    int elemNu1, elemNu2;
    int keythreshold;
};

typedef struct list_node
{
    /* may be whatever you want */
    MySKL_ns node;
    int item;
    int key;
} LN_s, * LN_p;

/* where input will be stored  -- test_func.c */
extern struct input inputargs;
