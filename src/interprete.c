/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "funcionesInterprete.h"

log_t* archLog;

int main (int argc, char **argv){

	log_t* archLog;
	off_t fileSize=0;
	char* buffer = leerScript(&fileSize,argv[1]);
	archLog=log_crear(PATHLOG);

	if (buffer == NULL) {

			log_escribir(archLog,"Nucleo PI",ERROR,"Error en la lectura del script");
			return EXIT_FAILURE;
	}
	return 0;
}
