/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

log_t* archLog;

void imprimirPrompt(void);

int main (int argc, char **argv){

	off_t fileSize=0;
	char* buffer = leerScript(&fileSize,argv[1]);
	archLog=crear_log(PATHLOG);


	return 0;
}

void imprimirPrompt(void) {
	printf("commiteando-por-un-suenio:~$")
}



