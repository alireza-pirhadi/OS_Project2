#include "types.h"
#include "stat.h"
#include "user.h"

struct timeVariables {
  int creationTime;
  int terminationTime;
  int sleepingTime;
  int readyTime;
  int runningTime;
};

int main(){
  int a[10][4] = {0};
  changePolicy(1);
  int pid = fork();
  for(int i=0;i<10;i++){
	if(pid == 0){
	    for(int i=0;i<1000;i++)
  		printf(1, "|%d| : |%d|\n",getpid(),i);
	    exit();
	}
	if(pid > 0 && i != 9){
		pid = fork();
	}
  }
   for(int i=0;i<10;i++){
      struct timeVariables *t;
      t = malloc(sizeof(struct timeVariables));
      int childpid = waitForChild(t);
      if(childpid != (-1)){
	  a[i][0] = childpid;
	  a[i][1] = t->terminationTime - t->creationTime;
	  a[i][2] = t->runningTime;
	  a[i][3] = t->readyTime;
      }
  }
  int average_turnaroundTime = 0;
  int average_cbt = 0;
  int average_waitingTime = 0;
  for(int i=0;i<10;i++){
	average_turnaroundTime += a[i][1];
	average_cbt += a[i][2];
	average_waitingTime += a[i][3];
	printf(1, "pid : %d, Turnaround time = %d, CBT = %d, Waiting time = %d\n", a[i][0], a[i][1],a[i][2],a[i][3]);
  }
  average_turnaroundTime /= 10;
  average_cbt /= 10;
  average_waitingTime /= 10;
  printf(1, "\naverage Turnaround time = %d, average CBT = %d, average Waiting time = %d\n", average_turnaroundTime, average_cbt,average_waitingTime);
  exit();
  return 0;
}
