#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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
  return myproc()->pid;
}

int
sys_getppid(void)
{
  return myproc()->parent->pid;
}

int
sys_getChildren(void)
{
  int ppid;
  argint(0, &ppid);
  return getChildrenpids(ppid);
}

int
sys_getCount(void)
{
  int syscall_number;
  argint(0, &syscall_number);
  if(syscall_number < 1 || syscall_number > 24)
	return -1;
  return myproc()->number_syscall_invokes[syscall_number - 1];
}

int
sys_changePriority(void)
{
  int new_priority;
  argint(0, &new_priority);
  if(new_priority < 1 || new_priority > 5)
	return -1;
  myproc()->priority = new_priority;
  return 1;
}

int
sys_changePolicy(void)
{
  int algorithm_number;
  argint(0, &algorithm_number);
  if(algorithm_number < 0 || algorithm_number > 2)
	return -1;
  changeAlgorithm(algorithm_number);
  return 1;
}

int
sys_waitForChild(void)
{
  struct timeVariables *time_variables;
  argptr (0 , (void*)&time_variables ,sizeof(*time_variables));
  return getTimeVariables(time_variables);
}

int
sys_ticketlockInit(void)
{
  initTicketlock(2);
  return 1;
}

int
sys_ticketlockTest(void)
{
  aquireTicketlock(2);
  for(int i=0;i<10;i++)
	cprintf("%d\n",myproc()->pid);
  return releaseTicketlock(2);
}

int
sys_rwinit(void)
{
  initTicketlock(0);
  initTicketlock(1);
  return 1;
}

int
sys_rwtest(void)
{
  int lock_num, res = 0;
  argint(0, &lock_num);
  if(lock_num == 0){
	aquireTicketlock(lock_num);
	reader_count++;
	if(reader_count == 1)
		aquireTicketlock(1);
	releaseTicketlock(lock_num);
	
	res = shared_data;
	
	aquireTicketlock(lock_num);
	reader_count--;
	if(reader_count == 0)
		releaseTicketlock(1);
	releaseTicketlock(lock_num);
  }
  else if( lock_num == 1){
	aquireTicketlock(lock_num);
	shared_data++;
	releaseTicketlock(lock_num);
  }
  return res;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
