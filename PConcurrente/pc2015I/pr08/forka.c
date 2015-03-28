#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// Creación de una tarea en forma de preceso (programa secuencial)

int main(void){
  int i,nh,status,a[3]={1,2,1};
  pid_t pidh,wpid,pid;

  for(i=0;i<2;i++){
    printf("i = %d \n",i); 
    pid=getpid();
    if((pidh=fork())==0){
      printf("In child process\n");
      pid=getpid();
      printf("pid in child process = %d\n",pid);
      if(a[i]<2){
	printf("Should be accept\n");
	_exit(1);
      }  else{
        printf("Shoul be reject\n", getpid(), getppid());
	_exit(0);
      }
    }
  }
  if(pidh>0){
    (void)waitpid(pidh,&status,0);
    pid=getpid();
    printf("Parent pid: %d\n", pid);
    wpid=wait(&status);
    if(wpid!=-1){
      printf("Child's exit status was %d\n",status);
      if(status>0){
	printf("Accept\n");
      }  else{
	printf("Reject\n");
      }
    }
  }
  return(0);
}
//       exit(EXIT_FAILURE);
