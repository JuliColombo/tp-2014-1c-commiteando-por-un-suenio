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
			log_escribir(archLog,"No se pudo leer el script",ERROR,"El path del script es inválido");
			abort();
	}

	if((socket_crearYConectarCliente(configuracion_programa.ip_kernel,configuracion_programa.puerto_kernel))==-1){
		log_escribir(archLog, "Conexion", ERROR, "No se pudo conectar al Kernel");
		abort();
	}




	return 0;
}
