/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "libs/funciones_UMV.h"

void* MP;
int tamanioMP;
int tamanioMaxStack;
char* PATH=PATHCONFIG;
int retardo=0;
int procesoEnUso;
int cant_tablas=0;
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, CONEXIONES, CPU;
log_t* archLog;
pthread_mutex_t* mutex_log;
pthread_mutex_t* mutex_pid;
int sock_servidor;
int sock_cpu;
int procesoActivo;
int gradoDeMultiprogramacion;

t_list * Segmentos_UMV;
t_list * Rangos_Libres;
t_list * List_Base_ID;
int AlgoritmoActual, Puerto;
int FinPrograma = 0;
int Retardo = 0;

// Semaforos
pthread_mutex_t Sem_Graba_Segmento = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Sem_Elimina_Segmento = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Sem_GrabaBytes = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Sem_DevuelveBytes = PTHREAD_MUTEX_INITIALIZER;



int main (int argc, char **argv){
	//Inicializamos las listas
	Segmentos_UMV = list_create();
	Rangos_Libres = list_create();
	List_Base_ID = list_create();
	srand(time(NULL));

	inicializarConfiguracion();
	//Acceder a archConfig y obtener datos
	MP=crearMP();

	//Guardamos el primer rango de memoria libre
	RangoMemoria * RangoInicial = create_rango_mem(0,tamanioMP);
	list_add(Rangos_Libres,RangoInicial);


	inicializarMutex();
	inicializarHilos();



	esperarHilos();

	return EXIT_SUCCESS;
}




