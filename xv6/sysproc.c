#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "sem.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}


// Semaphores

//TODO Make thread safe!
int
sys_sem_init(void)
{
	
	int sem, value;

	if(argint(0,&sem) < -1)
		return -1;

	if(argint(1,&value) < -1)
		return -1;

	if(value < 1)
		return -1;

	if(sem >= SEM_VALUE_MAX)
		return -1;

	if(semtable[sem].active)
		return -1;

	semtable[sem].active = 1;
	semtable[sem].value = value;
	//TODO finish this next line (if need be)
	//semtable[sem].spinlock = ??


	return 0;
}

int
sys_sem_destroy(void)
{
	
	int sem;

	if(argint(0,&sem) < -1)
		return -1;

	//No point in checking if it wasn't active already
	semtable[sem].active = 0;

	return 0;
}

int
sys_sem_wait(void)
{
	int sem, count;

	//check for errors
	if(argint(0,&sem) < -1)
		return -1;
	
	if(argint(0,&count) < -1)
		return -1;

	if(count < 1)
		return -1;

	if(sem >= SEM_VALUE_MAX)
		return -1;

	if(semtable[sem].active)
		return -1;
	//end of error checks

	if(semtable[sem].value >= count) {
		semtable[sem].value -= count;
	} else {
		return sem_enqueue(sem, count); //TODO Define this method elsewhere
		//TODO spinlock
	}

	return 0;
}

int
sys_sem_signal(void)
{
	
	int sem, count;

	//check for errors
	if(argint(0,&sem) < -1)
		return -1;
	
	if(argint(0,&count) < -1)
		return -1;

	if(count < 1)
		return -1;

	if(sem >= SEM_VALUE_MAX)
		return -1;

	if(semtable[sem].active)
		return -1;
	//end of error checks

	semtable[sem].value += count;

	if(!semtable[sem].wait) { //TODO check if queue is not empty
		//TODO remove processes from queue
		//TODO place this process on `waiting list`
	}

	return 0;
}
