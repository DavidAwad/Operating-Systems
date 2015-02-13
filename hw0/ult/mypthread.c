#include"mypthread.h"


static mypthread_real threadPool[THREAD_COUNT];
static mypthread_real *currThread;
static int countThreads = 1;

//Internal function declaration
//Don't want these functions to be exterally visible
mypthread_real *getThreadWithStatus(mypthread_status status);
#define getUnusedThread() getThreadWithStatus(UNUSED)
#define getActiveThread() getThreadWithStatus(ACTIVE)
#define getPausedThread() getThreadWithStatus(PAUSED)

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg) {

	if(countThreads == THREAD_COUNT-1) {
		panic("Too many threads");
	}

	mypthread_real *ret;
	//Equiv: mypthread_t ret;

	//Find UNUSED thread
	ret = getUnusedThread();
	if(ret == NULL) {
		panic("Couldn't find UNUSED thread");
	}

	countThreads++;

	if(getcontext(&(ret->ctx)) != 0) {
		panic("Unable to get context");
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

	if(!currThread && (countThreads == 1)) {
		//User calls exit before create
		exit(0);
	}

	if(!currThread) {
		//User calls exit before join or yield, but after create
		printf("Need to make this edge case");
	}

	currThread->status = UNUSED;
	countThreads--;

	if(countThreads == 0) {
		exit(0);
	}

	currThread = getPausedThread();
	if(!currThread) {
		panic("Unable to find paused thread!");
	}

	currThread->status = ACTIVE;

	if(setcontext(&(currThread->ctx)) == -1) {
		panic("Unable to set context!");
	}
}

int mypthread_yield(void) {
	return mypthread_join(getPausedThread(), NULL);
}

int mypthread_join(mypthread_t thread, void **retval) {
	//cry

	if(thread == 0) {
		//Error, either by user or yield getPausedThread() failed
		panic("Something bad happened");
	}

	if(currThread = NULL) {
		//Should only happen on very first attempted join/ yield
		//Means main thread needs to be set up
	}

	if(thread->status == UNUSED) {
		panic("Recived uninitialized thread");
	}

	/*
	mypthread_real *tempThread;
	mypthread_real *argThread = thread;

	currThread->status = PAUSED;

	currThread = argThread;
	currThread->status = ACTIVE;

	if(setcontext(&(currThread->ctx)) == -1) {
		//panic!
		fprintf(stderr, "Unable to set context!\n");
		return -1;
	}

	return 0;
	
	*/
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

void panic_(char *err, char* location) {
	fprintf(stderr, "Error %s: %s\n", err, location);
	exit(EXIT_FAILURE);
}


//

inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
			const mypthread_mutexattr_t *attr) { return 0; }

inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_lock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex) { return 0; }
