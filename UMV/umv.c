/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "libs/funciones_UMV.h"

int* MP;
int tamanioMP;
int tamanioMaxStack;
char* PATH=PATHCONFIG;
int retardo=0;
int procesoEnUso;
tablaSeg* tablaDeSegmentos;
int cant_tablas=0;
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, CONEXIONES, CPU;
log_t* archLog;
pthread_mutex_t* mutex_MP;
pthread_mutex_t* mutex_log;
pthread_mutex_t* mutex_pid;
int sock_servidor;
int sock_cpu;
int procesoActivo;
int gradoDeMultiprogramacion;
lista_handshake lista_handshakes;


int main (int argc, char **argv){

	inicializarConfiguracion();
	//Acceder a archConfig y obtener datos
	MP=crearMP();

	//Crear estructuras administrativas

	inicializarListaHandshakes();
	inicializarMutex();
	inicializarHilos();

	esperarHilos();

	return EXIT_SUCCESS;
}




