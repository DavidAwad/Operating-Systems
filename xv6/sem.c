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

int getOldest(int sem) {
	int i;
	
	if(semtable[sem].waitlist.waiting_count == 0) {
		return 0;
	}

	for(i=semtable[sem].waitlist.oldest+1;;i++) {
		if(i>=SEM_WAITLIST_MAX) {
			i = 0;
		}
		if(i = semtable[sem].waitlist.oldest) {
			return 0; //This shouldn't ever happen
		}
		if(semtable[sem].waitlist.value[i] > 0) {
			return i;
		}
	}

	return 0; //This should't ever happen and I don't want to do any extra error checking
}

int sem_dequeue(int sem) {
	int i,pid;

	for(i=semtable[sem].waitlist.oldest;;i++) {
		if(i>=SEM_WAITLIST_MAX) {
			i = 0;
		}

		if(semtable[sem].waitlist.value[i] != 0) {
			if(semtable[sem].waitlist.value[i] <= semtable[sem].value) {
				semtable[sem].value -= semtable[sem].waitlist.value[i];
				semtable[sem].waitlist.value[i] = 0;
				pid = semtable[sem].waitlist.pid[0];
				semtable[sem].waitlist.pid[0] = 0;
				semtable[sem].waitlist.waiting_count--;
				semtable[sem].waitlist.oldest = getOldest(sem);
				return pid;
			}
		}

		if(i==semtable[sem].waitlist.oldest-1 || (semtable[sem].waitlist.oldest==0 && i==SEM_WAITLIST_MAX)) {
			//Couldn't find something to remove
			return -1;
		}
	}

	return -1;
}

