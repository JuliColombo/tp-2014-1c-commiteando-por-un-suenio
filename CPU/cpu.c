/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include "cpu.h"

//t_dictionary* diccionario;
int top_index = -1;
config_cpu configuracion_cpu;
char* PATH;
pthread_t conexion_kernel,conexion_umv;
log_t* archLog;
t_quantum quantum;
t_retardo_quantum retardo;
t_pcb* pcb;
t_config* config;
sem_t sem_kernel;

int main (int argc, char **argv){
	if((sem_init(&sem_kernel,1,0))==-1){
		perror("No se puede crear el semaforo");
	}
	PATH=argv[1];
	inicializarConfiguracion();

	printf("\n\n EL PID DE ESTA CPU ES: %d\n\n",(int)getpid());

	pthread_create(&conexion_umv, NULL, (void*) &core_conexion_umv, NULL);
	pthread_create(&conexion_kernel, NULL, (void*) &core_conexion_kernel, NULL);





	pthread_join(conexion_umv, NULL);
	pthread_join(conexion_kernel,NULL);
	if(socket_cerrarConexion(sockUMV)==-1){
		log_escribir(archLog,"Cerrar Conexion",ERROR,"No se pudo conectar a la UMV");
	}
	free(config->path);
	dictionary_destroy(config->properties);
	free(config);

	return 0;
}


