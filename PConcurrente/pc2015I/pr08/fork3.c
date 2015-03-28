#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// Creación de una tarea en forma de preceso (programa secuencial)

void main(){
  int i,nh,status;
  pid_t pidh; // Variable para guardar el PID del hijo
  printf("Cuantos Procesos se crean?: \n");
  scanf("%d",&nh);
  pidh=fork();
  for(i=0;i<nh;i++){
    if(pidh==0){
      if(pidh==-1){
        perror("fork failed\n");
        exit(EXIT_FAILURE);
      }  else if(pidh==0){
        pidh=fork();
        printf("Hola soy proceso pid: %d, mi padre %d \n", getpid(),getppid());
      }
      if(pidh!=0){
      (void)waitpid(pidh,&status,0);
      exit(status);
      }
    }
  }
}
