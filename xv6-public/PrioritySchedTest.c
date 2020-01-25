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
  int a[25][4]={0};
  changePolicy(2);
  int pid = fork();
  for(int i=1;i<=25;i++){
	a[i-1][0] = pid;
	if(pid == 0){
	    changePriority(5 - ((i - 1) / 5));
	    for(int j=0;j<500;j++)
  		    printf(1, "|%d| : |%d|\n",i,j);
	    exit();
	}
	if(pid > 0 && i != 25){
		pid = fork();
	}
  }
  for(int i=0;i<25;i++){
      struct timeVariables *t;
      t = malloc(sizeof(struct timeVariables));
      int childpid = waitForChild(t);
      if(childpid != (-1)){
          int p_num = 0;
          for(int j=0;j<25;j++)
	      if(a[j][0] == childpid)
		  p_num = j;
	  a[p_num][1] = t->terminationTime - t->creationTime;
	  a[p_num][2] = t->runningTime;
	  a[p_num][3] = t->readyTime;
      }
  }
  int groups_average[5][3] = {0}, total_average[3] = {0};
  for(int i=0;i<25;i++){
	groups_average[i/5][0] += a[i][1];
	groups_average[i/5][1] += a[i][2];
	groups_average[i/5][2] += a[i][3];
	total_average[0] += a[i][1];
	total_average[1] += a[i][2];
	total_average[2] += a[i][3];
	printf(1, "p_num : %d, pid : %d, Turnaround time = %d, CBT = %d, Waiting time = %d\n", i+1, a[i][0], a[i][1], a[i][2], a[i][3]);
  }
  printf(1,"\n");
  for(int i=0;i<5;i++){
	groups_average[i][0] /= 5;
	groups_average[i][1] /= 5;
	groups_average[i][2] /= 5;
	printf(1, "averages of group %d : Turnaround time = %d, CBT = %d, Waiting time = %d\n", i+1, groups_average[i][0], groups_average[i][1],groups_average[i][2]);
  }
  total_average[0] /= 25;
  total_average[1] /= 25;
  total_average[2] /= 25;
  printf(1, "\ntotal averages : Turnaround time = %d, CBT = %d, Waiting time = %d\n", total_average[0], total_average[1], total_average[2]);
  exit();
  return 0;
}
