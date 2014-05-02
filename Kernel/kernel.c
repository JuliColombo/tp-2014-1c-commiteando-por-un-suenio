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
	char* PATH;

int main(int argc, char **argv){ // Recibe la ruta del archivo de configuracion del Kernel
	PATH=argv[1];

    inicializarConfiguracion(PATH); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel


    return 0;
}
