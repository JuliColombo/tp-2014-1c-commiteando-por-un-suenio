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

int main(int argc, char **argv){ // No recibe nada ahora
	PATH=argv[1];


	/*char *cwd = (char *)malloc(sizeof(char)*1000);
    getcwd(cwd, sizeof(cwd));

    printf("%s", cwd);*/

    inicializarConfiguracion(PATH); //Lee el archivo de configuracion y asigna las configuraciones a configuracion_kernel
    imprimirConfiguracion(configuracion_kernel); //Imprime las configuraciones actuales por pantalla

    return 0;
}
