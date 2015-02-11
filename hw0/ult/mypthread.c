#include"mypthread.h"


static mypthread_t threadPool[THREAD_COUNT];
static mypthread_t *currThread;
static int countThreads;

//Internal function declaration
mypthread_t *getThreadWithStatus(mypthread_status status);

/*
typedef struct {
	mypthread_status status;
	mypthread_attr_t attr;
	void *(*func) (void *);
	void *argv;
	ucontext_t ctx;
	char stk[STACK_SIZE];
} mypthread_t;
*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg) {
	
	if(countThreads == THREAD_COUNT) {
		//panic!
		fprintf(stderr, "Too many thread");
		exit(EXIT_FAILURE);
	}

	//Find UNUSED thread
	thread = getThreadWithStatus(UNUSED);
	if(thread == NULL) {
		//panic!
		fprintf(stderr, "Couldn't find UNUSED thread");
		exit(EXIT_FAILURE);
	}

	countThreads++;

	if(getcontext(&thread->ctx) != 0) {
		//panic!
		fprintf(stderr, "Unable to get context");
		exit(EXIT_FAILURE);
	}
	thread->ctx.uc_stack.ss_sp = thread->stk;
	thread->ctx.uc_stack.ss_size = STACK_SIZE;

	thread->func = start_routine;
	thread->argv = arg;

	thread->status = PAUSED;

	return 0;
}

/*
 * mypthread_exit has 3 responsibilities
 * 1) sets its own status to UNUSED
 * 2) decrements the thread count
 *
 * 3) If the thread count is 0 (meaning this is the last thread)
 * 		-call exit(3), terminating the program
 * 	otherwise switch to a context of a PAUSED thread
 */

void mypthread_exit(void *retval) {	
	mypthread_t *tempThread;

	currThread->status = UNUSED;
	countThreads--;

	if(countThreads == 0) {
		exit(0);
	}

	tempThread = currThread;
	currThread = getThreadWithStatus(PAUSED);
	if(!currThread) {
		//panic!
		fprintf(stderr, "Unable to find paused thread!");
		exit(EXIT_FAILURE);
	}

	currThread->status = ACTIVE;

	if(setcontext(&(currThread->ctx)) == -1) {
		//panic!
		fprintf(stderr, "Unable to set context!");
		exit(EXIT_FAILURE);
	}

}

int mypthread_yield(void) {
	return -1;
}

int mypthread_join(mypthread_t thread, void **retval) {
	
}

//HELPER FUNCTIONS
mypthread_t *getThreadWithStatus(mypthread_status status) {
	int i=0;
	mypthread_t *currThread = threadPool;
	for(;i<THREAD_COUNT;i++) {
		if((currThread+i)->status == status) {
			return currThread+i;
		}
	}

	return NULL; //error, handled by caller
}




//

inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
			const mypthread_mutexattr_t *attr) { return 0; }

inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_lock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex) { return 0; }
