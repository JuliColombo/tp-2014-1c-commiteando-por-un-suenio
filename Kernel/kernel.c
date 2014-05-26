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


int main(int argc, char **argv) { //Recibe la ruta del archivo de configuracion del Kernel o el nombre del archivo si esta en la misma carpeta
	PATH = argv[1];
	cola.new,cola.ready,cola.exec,cola.block,cola.exit=NULL;

	inicializarConfiguracion(); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel

	int thread_plp = pthread_create (&plp, NULL, core_plp(), NULL);
	int thread_pcp = pthread_create (&pcp, NULL, core_pcp(), NULL);


	int thread_io[cant_identificadores(configuracion_kernel.id_hio)];

	int i;
	for(i=0;i < cant_identificadores(configuracion_kernel.id_hio); i++){
		// ******  HAY QUE MODIFICAR LOS HILOS ACÁ ADENTRO ******

		thread_io[i] = pthread_create(&io.thread[i].tid, NULL, core_io(configuracion_kernel.retardo_hio[i]), NULL); //La estructura io.thread[i].tid no la entendi, hice lo que me parecio para que funcione pero hay que cambiarla
		printf("Thread %d para IO\n", i);
		/*pthread_mutex_lock(io.mutex);
		agregarHilo(io.thread[i].tid);
		pthread_mutex_unlock(io.mutex);*/
	}



	pthread_join(thread_pcp, NULL);
	pthread_join(thread_plp, NULL);

	return EXIT_SUCCESS;
}
