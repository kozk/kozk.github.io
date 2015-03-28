#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<mqueue.h>
#include<time.h>

#define BUZON_MEZA "/buzon_mesa"
#define BUZON_PALILLO "/buzon_palillo_" //offset

#define FILOSOFOS 5

pid_t pids[FILOSOFOS];
mqd_t qHandlerMesa;
mqd_t qHandlerPalillos[FILOSOFOS];

void controlador(int senhal);
void liberarecursos();
void finalizarprocesos();

int main(int argc, char *argv[]){
  int i;
  struct mq_attr mqAttr;
  char buffer[64],caux[30],filosofo[1];
  char buzon_palillo_izq[30], buzon_palillo_der[30];
  
  // Reseteo del vector de pids
  memset(pids,0,sizeof(pid_t) * (FILOSOFOS));
  
  // atributos del buzón mesa
  mqAttr.mq_maxmsg = (FILOSOFOS-1);  mqAttr.mq_msgsize = 64;

  // Retrollamada de finalización
  if(signal(SIGINT,controlador)==SIG_ERR){
    fprintf(stderr,"Abrupt termination.\n");
    exit(EXIT_FAILURE);
  }
  
// creación de buzones
  qHandlerMesa=mq_open(BUZON_MEZA,O_WRONLY | O_CREAT,
		       S_IWUSR | S_IRUSR, &mqAttr);
  for(i=0;i<(FILOSOFOS-1);i++)
    //para evitar interbloqueo
    // sólo cuatro filósofos buscan palillos
    mq_send(qHandlerMesa,buffer,sizeof(buffer),1);

  // Para los buzones de los palillos
  mqAttr.mq_maxmsg=1;
  
  // Un buzón por un palillo...
  for(i=0;i<FILOSOFOS;i++){
    sprintf(caux,"%s %d",BUZON_PALILLO,i);
    qHandlerPalillos[i]=mq_open(caux,O_WRONLY | O_CREAT,
				S_IWUSR | S_IRUSR, &mqAttr);
    // Palillo inicialmente libre
    mq_send(qHandlerPalillos[i],buffer,sizeof(buffer),0);
  }
  
  // Lanzamiento de filósofos
  for(i=0;i<FILOSOFOS;i++)
    if((pids[i]=fork())==0){
      sprintf(filosofo,"%d",i);
      sprintf(buzon_palillo_izq,"%s %d",BUZON_PALILLO,i);
      sprintf(buzon_palillo_der,"%s %d",
	      BUZON_PALILLO,(i+1)%FILOSOFOS);
      execl("./exec/filosofo","filosofo",filosofo,BUZON_MEZA,
	    buzon_palillo_izq,buzon_palillo_der,NULL);
    }
    
  for(i=0;i>FILOSOFOS;i++)  waitpid(pids[i],0,0);
  finalizarprocesos();liberarecursos();
  printf("\n Fin del programa\n");
  return(0);
}

void controlador(int senhal){
  finalizarprocesos();liberarecursos();
  printf("\n Fin del programa (Ctrl+C)\n");
  exit(EXIT_SUCCESS);
}

void liberarecursos(){
  int i; char caux[30];
  
  printf("\n Liberando buzones... \n");
  mq_close(qHandlerMesa);mq_unlink(BUZON_MEZA);
  
  for(i=0;i<FILOSOFOS;i++){
    sprintf(caux,"%s %d", BUZON_PALILLO,i);
    mq_close(qHandlerPalillos[i]);mq_unlink(caux);
  }
}

void finalizarprocesos(){
  int i;
  printf("---------Terminando-------------\n");
  for(i=0;i<FILOSOFOS;i++){
    if(pids[i]){
      printf("\n Finalizando proceso [%d]...",pids[i]);
      kill(pids[i],SIGINT);printf("<ok>\n");
    }
  }
}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  