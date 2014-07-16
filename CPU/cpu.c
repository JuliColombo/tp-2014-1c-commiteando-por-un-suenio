/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include "cpu.h"

t_dictionary* diccionario;
int top_index = -1;
config_cpu configuracion_cpu;
char* PATH;
pthread_t conexion_kernel,conexion_umv;
log_t* archLog;
t_quantum quantum;
t_retardo_quantum retardo;

int main (int argc, char **argv){
	PATH=argv[1];
	inicializarConfiguracion();

	//pthread_create(&conexion_umv, NULL, (void*) &core_conexion_umv, NULL);
	pthread_create(&conexion_kernel, NULL, (void*) &core_conexion_kernel, NULL);

	//pthread_join(conexion_umv, NULL);
	pthread_join(conexion_kernel,NULL);

	return 0;
}


