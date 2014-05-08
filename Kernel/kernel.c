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
char* PATH;

int main(int argc, char **argv) { // Recibe la ruta del archivo de configuracion del Kernel
	PATH = argv[1];

	inicializarConfiguracion(PATH); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel

	int thread_plp = pthread_create (&plp, NULL, core_plp(), NULL);
	int thread_pcp = pthread_create (&pcp, NULL, core_pcp(), NULL);

	pthread_join(thread_pcp, NULL);
	pthread_join(thread_plp, NULL);

	t_pcb* proceso = NULL;
	int i;
	for(i=0;i < cant_hilos(*configuracion_kernel.id_hio); i++){
		// ******  HAY QUE MODIFICAR LOS HILOS ACÁ ADENTRO ******

		//pthread_create(&IO.thread[i].tid, NULL, (void*) &core_io, NULL);

		/*pthread_mutex_lock(IO.mutex);
		agregarHilo(IO.thread[i].tid);
		pthread_mutex_unlock(IO.mutex);*/
	}


	return EXIT_SUCCESS;
}
