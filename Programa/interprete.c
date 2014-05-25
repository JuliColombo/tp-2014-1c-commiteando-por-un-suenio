/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include "interprete.h"

log_t* archLog;
t_config_programa configuracion_programa;
char* PATH=PATHCONFIG;
char* Archivo_Ansisop;


int main (int argc, char **argv){
	Archivo_Ansisop = argv[1];
	inicializarConfiguracion();
	off_t fileSize=0;
	char* buffer = leerScript(&fileSize,argv[1]);
	if (buffer == NULL) {
			log_escribir(archLog,"Nucleo PI",ERROR,"Error en la lectura del script");
			return EXIT_FAILURE;
	}
	return 0;
}
