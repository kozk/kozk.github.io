#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<mqueue.h>

#define MAX_TIME_PENSAR 7
#define MAX_TIME_COMER 5

void filosofo(char *filosofo,char *buzon_mesa,
	      char *buzon_palillo_izq,char *buzon_palillo_der);
void controlador(int senhal);

int main(int argc,char *argv[]){
  filosofo(argv[1],argv[2],argv[3],argv[4]);
  return(0);
}

void filosofo(char *filosofo,char *buzon_mesa,
	      char *buzon_palillo_izq,char *buzon_palillo_der){
  mqd_t qHandlerMesa,qHandlerIzq,qHandlerDer;
  int n_filosofo;
  char buffer[64];
  
  if(signal(SIGINT,controlador)==SIG_ERR){
    fprintf(stderr,"Abrupt termination.\n");  exit(EXIT_FAILURE);
  }
  n_filosofo=atoi(filosofo);
  
  qHandlerMesa=mq_open(buzon_mesa,O_RDWR);
  qHandlerIzq=mq_open(buzon_palillo_izq,O_RDWR);
  qHandlerIzq=mq_open(buzon_palillo_der,O_RDWR);
  
  srand((int)getpid());
  while(1){
    printf("[Filosofo %d] pensando...\n",n_filosofo);
    sleep(rand()%MAX_TIME_PENSAR);
    mq_receive(qHandlerMesa,buffer,sizeof(buffer),NULL);
    //coge palillos
    mq_receive(qHandlerIzq,buffer,sizeof(buffer),NULL);
    mq_receive(qHandlerDer,buffer,sizeof(buffer),NULL);
//COME
    printf("[Filosofo %d] comiendo...\n",n_filosofo);
    sleep(rand()%MAX_TIME_COMER);
    //suelta palillos
    mq_send(qHandlerIzq,buffer,sizeof(buffer),0);
    mq_send(qHandlerDer,buffer,sizeof(buffer),0);

    mq_send(qHandlerMesa,buffer,sizeof(buffer),0);
  }
}

void controlador(int senhal){
  printf("[Filosofo %d] Finlizado (SIGINT)\n",getpid());
  exit(EXIT_SUCCESS);
}

    




