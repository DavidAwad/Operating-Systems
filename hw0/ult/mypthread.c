#include"mypthread.h"


static mypthread_real threadPool[THREAD_COUNT];
static mypthread_real *currThread;
static int countThreads;

//Internal function declaration
mypthread_real *getThreadWithStatus(mypthread_status status);
#define getUnusedThread() getThreadWithStatus(UNUSED);
#define getActiveThread() getThreadWithStatus(ACTIVE);
#define getPausedThread() getThreadWithStatus(PAUSED);

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg) {

	if(countThreads == THREAD_COUNT) {
		//panic!
		fprintf(stderr, "Too many thread\n");
		exit(EXIT_FAILURE);
	}

	mypthread_real *ret;

	//Find UNUSED thread
	ret = getUnusedThread();
	if(ret == NULL) {
		//panic!
		fprintf(stderr, "Couldn't find UNUSED thread\n");
		exit(EXIT_FAILURE);
	}

	countThreads++;

	if(getcontext(&ret->ctx) != 0) {
		//panic!
		fprintf(stderr, "Unable to get context\n");
		exit(EXIT_FAILURE);
	}
	ret->ctx.uc_stack.ss_sp = ret->stk;
	ret->ctx.uc_stack.ss_size = STACK_SIZE;
	//ret->ctx.ul_link = ;
	
	makecontext(&(ret->ctx), (void (*)(void))start_routine, 1, arg);

	ret->status = PAUSED;

	*thread = ret; //This line is important, I think it needs to be fixed

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
	mypthread_real *tempThread;

	currThread->status = UNUSED;
	countThreads--;

	if(countThreads == 0) {
		exit(0);
	}

	tempThread = currThread;
	currThread = getPausedThread();
	if(!currThread) {
		//panic!
		fprintf(stderr, "Unable to find paused thread!\n");
		exit(EXIT_FAILURE);
	}

	currThread->status = ACTIVE;

	if(setcontext(&(currThread->ctx)) == -1) {
		//panic!
		fprintf(stderr, "Unable to set context!\n");
		exit(EXIT_FAILURE);
	}

}

int mypthread_yield(void) {
	return mypthread_join(getPausedThread(), NULL);
}

int mypthread_join(mypthread_t thread, void **retval) {
	mypthread_real *tempThread;
	mypthread_real *argThread = thread;

	if(argThread == NULL) {
		//error!
		fprintf(stderr, "Null ptr recived by mypthread_join\n");
		return -1;
	}
	if(argThread->status == UNUSED) {
		//error!
		fprintf(stderr, "Recived an uninitialized thread\n");
		return -1;
	}

	currThread->status = PAUSED;

	currThread = argThread;
	currThread->status = ACTIVE;

	if(setcontext(&(currThread->ctx)) == -1) {
		//panic!
		fprintf(stderr, "Unable to set context!\n");
		return -1;
	}

	return 0;
	
}

//HELPER FUNCTIONS
mypthread_real *getThreadWithStatus(mypthread_status status) {
	int i=0;
	mypthread_real *currThread = threadPool;
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
