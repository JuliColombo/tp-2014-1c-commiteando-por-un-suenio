/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include "interprete.h"

log_t* archLog;
t_config_programa configuracion_programa;
char* PATH_config=pathconfig;
char* archivo_ansisop;
int sock_kernel_servidor;
FILE* script;


int main (int argc, char **argv){
	//PATH_config =getenv("ANSISOP_CONFIG");
	printf("%s\n",PATH_config);
	inicializarConfiguracion();
	off_t fileSize=0;
	archivo_ansisop = leerScript(&fileSize,argv[1]);
	if (archivo_ansisop == NULL) {
			log_escribir(archLog, "Programa", ERROR, "Error en la lectura del Script");
			return EXIT_FAILURE;
	}

	int i=0;
	if((i=validarScript())==0){
		log_escribir(archLog, "Programa",INFO,"La script es de tipo ANSISOP");
	}else{
		log_escribir(archLog, "Script",ERROR,"La script no puede ser interpretada");
		abort();
	}



	int sock_kernel_servidor=abrirSocket();
	int j=socket_enviar(sock_kernel_servidor, D_STRUCT_STRING, archivo_ansisop);
	if(j==1){
		log_escribir(archLog, "Programa", INFO,"La script se envió correctamente");
	}else{
		printf("ERROR: no se pudo enviar el paquete\n");
	}


	while(1){

		}




	cerrarSocket(sock_kernel_servidor);



	return 0;
}
