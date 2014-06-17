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
int sock_kernel_servidor;


int main (int argc, char **argv){
	Archivo_Ansisop = argv[1];
	PATH =getenv("ANSISOP_CONFIG");
	printf("%s\n",PATH);
	inicializarConfiguracion();
	off_t fileSize=0;
	char* buffer = leerScript(&fileSize,argv[1]);
	if (buffer == NULL) {
			abort();
	}
	while(1){

	}
	int sock=abrirSocket();





	cerrarSocket(sock);



	return 0;
}
