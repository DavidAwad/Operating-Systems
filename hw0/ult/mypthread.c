#include"mypthread.h"


static mypthread_t threadPool[THREAD_COUNT];
static int currThread;
static int countThreads;


int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg) {
	if(getcontext(&thread->ctx) != 0) {
		//panic!
		fprintf(stderr, "Unable to get context");
		return 0;
	}
	thread->ctx.uc_stack.ss_sp = thread->stk;
	thread->ctx.uc_stack.ss_size = STACK_SIZE;

	thread->func = start_routine;
	thread->argv = arg;

	return 0;
}

void mypthread_exit(void *retval) {
	
}

int mypthread_yield(void) {
	return -1;
}

int mypthread_join(mypthread_t thread, void **retval) {
	
}

inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
			const mypthread_mutexattr_t *attr) { return 0; }

inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_lock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex) { return 0; }

inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex) { return 0; }
