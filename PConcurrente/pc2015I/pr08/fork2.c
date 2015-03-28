#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// Creación de una tarea en forma de preceso (programa secuencial)

void main(){
  pid_t pidh; // Variable para guardar el PID del hijo
  pidh=fork();
  printf("Soy una tarea del programa, mi PID: %d, valor de pidh = %d\n", getpid(), pidh);
}
