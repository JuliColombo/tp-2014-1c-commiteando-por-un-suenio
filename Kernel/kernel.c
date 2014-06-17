/*
 * kernel.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "kernel.h"

/* Variables Globales */

t_config_kernel configuracion_kernel;
log_t* archLog;
pthread_t pcp, plp;
t_thread_io io;
char* PATH;
cola_procesos cola;
pthread_mutex_t* mutex_cola_ready;
pthread_mutex_t* mutex_cola_new;
pthread_mutex_t* mutex_cola_exec;
pthread_mutex_t* mutex_cola_block;
pthread_mutex_t* mutex_cola_exit;
int sock_programas, sock_umv, sock_cpu;
int* fds_conectados_programas;
int* fds_conectados_cpu;



int main(int argc, char **argv) { //Recibe la ruta del archivo de configuracion del Kernel o el nombre del archivo si esta en la misma carpeta
	PATH = argv[1];
	inicializarColas();

	inicializarConfiguracion(); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel
	pthread_create(&plp, NULL, (void*) &core_plp, NULL);
	//pthread_create(&pcp, NULL, (void*) &core_pcp, NULL);


	esperarYCerrarConexiones();
	pthread_join(plp, NULL);
	//pthread_join(pcp, NULL);

	return EXIT_SUCCESS;
}
