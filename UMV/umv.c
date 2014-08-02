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
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, CONEXIONES, CPU;
log_t* archLog;
int sock_servidor;
int sock_cpu;
int procesoActivo;
int AlgoritmoActual, Puerto;
int FinPrograma = 0;

//Listas
t_list * Segmentos_UMV;
t_list * Rangos_Libres;
t_list * List_Base_ID;

// Semaforos
pthread_mutex_t sem_crear_segmento = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sem_destruir_Segmento = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sem_enviarBytes = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sem_solicitarBytes = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t* mutex_log;
pthread_mutex_t* mutex_pid;


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




