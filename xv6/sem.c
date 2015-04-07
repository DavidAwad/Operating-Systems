#include"sem.h"
#include"proc.h"

int sem_enqueue(int sem, int count){
	int i;
	if(semtable[sem].waitlist.waiting_count >= SEM_WAITLIST_MAX) {
		return -1;
	}
	for(i=semtable[sem].waitlist.oldest+1;;i++) {
		if(i==semtable[sem].waitlist.oldest) {
			//Waitlist full, this shouldn't happen
			return -1;
		}
		if(i>=SEM_WAITLIST_MAX) {
			i=0;
		}
		if(semtable[sem].waitlist.value[i]==0) {
			semtable[sem].waitlist.value[i] = count;
			semtable[sem].waitlist.pid[i] = 0; //TODO write this line
			semtable[sem].waitlist.waiting_count++;
			break;
		}
	}

	return 0;
}

int sem_take();
