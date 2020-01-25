#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]){
  int syscall_number = atoi(argv[1]);
  printf(1,"%d\n",getCount(syscall_number));
  exit();
  return 0;
}
