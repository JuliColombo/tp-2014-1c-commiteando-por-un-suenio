/*
 * kernel.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "kernel.h"

int main(int argc, char **argv){ //el kernel recive el archivo de configuracion como primer parametro
	t_config_kernel configuracion_kernel;
	char *cwd = (char *)malloc(sizeof(char)*1000);
    getcwd(cwd, sizeof(cwd));

    printf("%s", cwd);

    leerConfiguracion(configuracion_kernel, cwd);
    imprimirConfiguracion(configuracion_kernel);

    return 0;
}
