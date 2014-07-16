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
pthread_t pcp, plp, io;
char* PATH_config;
cola_procesos cola;
pthread_mutex_t* mutex_cola_ready;
pthread_mutex_t* mutex_cola_new;
pthread_mutex_t* mutex_cola_exec;
pthread_mutex_t* mutex_cola_block;
pthread_mutex_t* mutex_cola_exit;
pthread_mutex_t* mutex_pid;
int sock_programas, sock_umv, sock_cpu;
int* fds_conectados_cpu;
int* estado_cpu;
t_programa* programas;
int program_pid;
sem_t sem_multiProg, sem_pcp, sem_new, sem_cpu;




int main(int argc, char **argv) { //Recibe la ruta del archivo de configuracion del Kernel o el nombre del archivo si esta en la misma carpeta
	program_pid=0;
	PATH_config = argv[1];
	inicializarMutex();

	inicializarConfiguracion(); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel
	inicializarColas();
	crearSemaforos();

	pthread_create(&plp, NULL, (void*) &core_plp, NULL);
	pthread_create(&pcp, NULL, (void*) &core_pcp, NULL);


	esperarYCerrarConexiones();
	pthread_join(pcp, NULL);
	pthread_join(plp, NULL);

	return EXIT_SUCCESS;
}
