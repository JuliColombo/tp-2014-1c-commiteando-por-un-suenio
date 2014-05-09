/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include "interprete.h"

log_t* archLog;
t_config_programa configuracion_programa;
char* ANSISOP_CONFIG;

int main (int argc, char **argv){
	ANSISOP_CONFIG = argv[1];
	inicializarConfiguracion(ANSISOP_CONFIG);
	off_t fileSize=0;
	archLog = log_crear(PATHLOG);
	char* buffer = leerScript(&fileSize,argv[1]);
	if (buffer == NULL) {
			log_escribir(archLog,"Nucleo PI",ERROR,"Error en la lectura del script");
			return EXIT_FAILURE;
	}
	return 0;
}
