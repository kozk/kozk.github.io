// Por iniciar 
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define NSALIDAS 10
#define NENTRADAS 10
#define NLUGARES 24
#define NAUTOSXVEZ 8 // NUMERO DE AUTOS QUE ENTRARAN POR VEZ
#define NEJECUCIONES 5 //numero de veces que se ejecutara las entradas

sem_t SEntrada;//semaforo para tener control de las entradas
pthread_mutex_t C1;//candado para las zonas criticas
char est[NLUGARES];//el estacionamiento
pthread_t th_auto[NAUTOSXVEZ];//numero de autos que se estacionaran
int lugar;//el lugar que ocupara en el estacionamiento

void automovil( void *id );
int entrada( void *id ,int puerta);
void salida( void *id ,int salida,int lug);
void tiempo(int time);
void interfaz();

int main()
{
	int i;
	for(i=0;i<24;i++)
	{
		est[i]=' ';
	}
	sem_init( &SEntrada,0,24);
	pthread_mutex_init(&C1,NULL);

	int res;
	
	for(i=0;i<NAUTOSXVEZ;i++){
 	   pthread_create( &th_auto[i], NULL,(void *) &automovil, (void *) i);
	}

	for(i=0;i<NAUTOSXVEZ;i++){
		pthread_join(th_auto[i],(void *)&res);
	}
	return 0;
}

void automovil( void *id ){
	int ent,tie,sal,i;
	for(i=0;i<NEJECUCIONES;i++)
	{
		ent = rand()%NENTRADAS;
		tie = rand()%5;
		sal = rand()%NSALIDAS;

		int id2 = (int) id;
		//printf("id2: %d",id2);

		int lug = entrada(id,ent);
		interfaz();
		tiempo(tie);
		salida(id,sal,lug);
		interfaz();
	}
}


int entrada( void *id ,int puerta)
{
	int id2 = (int) id;
	char id3 = (char) (id2+48);//cambiamos el id a caracter

	sem_wait(&SEntrada);//checa que existan lugares en el estacionamiento

	printf("%d Entro por la puerta: %d\n",id2,puerta);

	pthread_mutex_lock(&C1);
	lugar = rand()%NLUGARES;
	while(est[lugar] != ' ')//para asegurar que ocupe un lugar vacio
	{
		lugar = (lugar+1)%24;
	}
	est[lugar] = id3;
	pthread_mutex_unlock(&C1);
	return lugar;

}


void salida( void *id ,int salida,int lug)
{
	int id2 = (int) id;
	char id3 = (char) (id2+48);//cambiamos el id a caracter

	printf("%d Salio por la puerta: %d\n",id2,salida);
	pthread_mutex_lock(&C1);
	est[lug] = ' ';
	pthread_mutex_unlock(&C1);
	sem_post(&SEntrada);
}

void tiempo(int time){
	sleep(time);
}

void interfaz()
{
	printf("|%c|%c|%c|%c|%c|%c|%c|%c|\n|%c|%c|%c|%c|%c|%c|%c|%c|\n|%c|%c|%c|%c|%c|%c|%c|%c|\n#################\n"
		    ,est[0],est[1],est[2],est[3],est[4],est[5],est[6],est[7]
		    ,est[8],est[9],est[10],est[11],est[12],est[13],est[14],est[15]
		    ,est[16],est[17],est[18],est[19],est[20],est[21],est[22],est[23]);
}


// En la wiki podemos agregar la info para el reporte
