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
char* PATH=PATHCONFIG;
pthread_t conexion_kernel,conexion_umv;
log_t* archLog;

int main (int argc, char **argv){

	inicializarConfiguracion();

	//Cuando me conecto a UMV, que me diga si la pila esta vacia y me guardo su valor de top_index. Si esta vacia, es -1, sino lo que corresponda.
	//Con el top_index voy a poder calcular la posicion a pushear



	int thread_umv = pthread_create(&conexion_umv, NULL, core_conexion_umv(), NULL);
	int thread_kernel = pthread_create(&conexion_kernel, NULL, core_conexion_kernel(), NULL);

	pthread_join(thread_umv, NULL);
	pthread_join(thread_kernel,NULL);

	return 0;
}


