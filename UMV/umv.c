/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "libs/funciones_UMV.h"


int* MP;
int algor=firstfit;
char* PATH;
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, KERNEL, CPU;

int main (int argc, char **argv){
	leerConfiguracion();
	imprimirConfiguracion();
	//Acceder a archConfig y obtener datos
	MP=crearMP(configuracion_UMV);
	//Crear estructuras administrativas

	pthread_create(&CONSOLA, NULL, (void*) &core_consola, NULL);
//	pthread_create(&KERNEL, NULL, (void*) &core_kernel, NULL);
/*	pthread_create(&CPU, NULL, (void*) &core_cpu, NULL);

	pthread_join(CONSOLA,NULL);
	pthread_join(KERNEL,NULL);
	pthread_join(CPU,NULL);
*/
	return 0;
}



//***********************************************Consola************************************

void *core_consola(void* parametro) {

	pthread_t inicio;
	pthread_create(&inicio, NULL, (void*) &consola, NULL);
}
