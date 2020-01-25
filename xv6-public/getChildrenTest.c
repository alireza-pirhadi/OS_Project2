#include "types.h"
#include "stat.h"
#include "user.h"

int main(){
	changePolicy(1);//for avoiding processes to print simultaneously
	int pid1 = fork();
	int pid2 = fork();
	int pid3 = fork();
	
	int pid = getpid();
	int ppid = getppid();
	int children = getChildren(getppid());
	printf(1,"pid : %d, ppid : %d, parent's children : %d\n", pid, ppid, children);
	if(pid1 == 0 && pid2 == 0 && pid3 > 0){
	    	wait();
	}
	if(pid1 == 0 && pid2 > 0 && pid3 > 0){
	    	wait();
		wait();
	}
	if(pid1 > 0 && pid2 == 0 && pid3 > 0){
	    	wait();
	}
	if(pid1 > 0 && pid2 > 0 && pid3 > 0){
	    	wait();
		wait();
		wait();
	}
	exit();
}
