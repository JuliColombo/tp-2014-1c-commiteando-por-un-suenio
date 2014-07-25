/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include "interprete.h"

log_t* archLog;
t_config_programa configuracion_programa;
char* PATH_config;
char* codigo_ansisop;
int sock_kernel_servidor;
FILE* script;

int main (int argc, char **argv){
	PATH_config =getenv("ANSISOP_CONFIG");
	inicializarConfiguracion();


	off_t fileSize=0;
	leerScript(&fileSize,argv[1]);
	if (codigo_ansisop == NULL) {
			log_escribir(archLog, "Programa", ERROR, "Error en la lectura del Script");
			return EXIT_FAILURE;
	}

//	int i;
//	if((i=validarScript())==0){
//		log_escribir(archLog, "Programa",INFO,"La script es de tipo ANSISOP");
//	}else{
//		log_escribir(archLog, "Script",ERROR,"La script no puede ser interpretada");
//	//	return EXIT_FAILURE;
//	}



	int sock_kernel_servidor=abrirSocket();
	t_struct_string* paquete = malloc(sizeof(t_struct_string));
	paquete->string=codigo_ansisop;
	int j=socket_enviar(sock_kernel_servidor, D_STRUCT_STRING, paquete);
	if(j==1){
		log_escribir(archLog, "Programa", INFO,"La script se envió correctamente");
		free(paquete);
	}else{
		log_escribir(archLog,"Envio paquete" ,ERROR,"no se pudo enviar");
	}
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int q=0;
	socket_recibir(sock_kernel_servidor, &tipoRecibido, &structRecibida);
	while(1){
		if(tipoRecibido==D_STRUCT_PROGFIN){
			t_struct_numero* num = ((t_struct_numero*)structRecibida);
			log_escribir(archLog, "Termino el programa", INFO, "Se finalizo correctamente el programa");
			free(structRecibida);
			break;
		}
		if(tipoRecibido==D_STRUCT_SF){
			log_escribir(archLog, "Termino el programa", INFO, "No hubo suficiente memoria para el programa");
			free(structRecibida);
			cerrarSocket(sock_kernel_servidor);
			break;
			return EXIT_FAILURE;
		}
		if(tipoRecibido==D_STRUCT_STRING){
			t_struct_string* cadenaAImprimir = ((t_struct_string*) structRecibida);
			printf("%s", cadenaAImprimir->string);
		}
		free(structRecibida);

		socket_recibir(sock_kernel_servidor, &tipoRecibido, &structRecibida);
	}

	cerrarSocket(sock_kernel_servidor);

	return EXIT_SUCCESS;
}
