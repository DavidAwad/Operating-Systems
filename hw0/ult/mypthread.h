#ifndef H_MYPTHREAD
#define H_MYPTHREAD

#define STACK_SIZE 4096
#define THREAD_COUNT 512

#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

#include"util.h"

//Status Enum
typedef enum {
	UNUSED,
	ACTIVE,
	PAUSED
} mypthread_status;

// Types
typedef struct {
	// Define any fields you might need inside here.
} mypthread_attr_t;

typedef struct {
	mypthread_status status;
	mypthread_attr_t attr;
	ucontext_t ctx;
	char stk[STACK_SIZE];
} mypthread_real;

typedef mypthread_real* mypthread_t;

#define mypthread_create( x , y , z , a) mypthread_create_( x , y , z , a , AT )
#define mypthread_exit( x ) mypthread_exit_( x , AT )
#define mypthread_yield( ) mypthread_yield_( AT )
#define mypthread_join( x , y) mypthread_join_( x , y , AT )

// Functions
int mypthread_create_(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg, char *location);

void mypthread_exit_(void *retval, char *location);

int mypthread_yield_(char *location);

int mypthread_join_(mypthread_t thread, void **retval, char *location);


/* Don't touch anything after this line.
 *
 * This is included just to make the mtsort.c program compatible
 * with both your ULT implementation as well as the system pthreads
 * implementation. The key idea is that mutexes are essentially
 * useless in a cooperative implementation, but are necessary in
 * a preemptive implementation.
 */

typedef int mypthread_mutex_t;
typedef int mypthread_mutexattr_t;

inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
			const mypthread_mutexattr_t *attr);

inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex);

inline int mypthread_mutex_lock(mypthread_mutex_t *mutex);

inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex);

inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

#endif
