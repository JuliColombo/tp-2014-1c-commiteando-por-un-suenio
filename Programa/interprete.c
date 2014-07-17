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
//	int j = socket_enviar(sock_kernel_servidor, D_STRUCT_STRING, codigo_ansisop);
//	if(j==1){
//		log_escribir(archLog, "Programa", INFO,"La script se envió correctamente");
//	}else{
//		printf("ERROR: no se pudo enviar el paquete\n");
//	}
	t_struct_string* paquete = malloc(sizeof(t_struct_string));
	paquete->string=codigo_ansisop;
	printf("el valor que se manda es \n%s\n", paquete->string);
	int j=socket_enviar(sock_kernel_servidor, D_STRUCT_STRING, paquete);
	if(j==1){
		printf("Se envio bien el paquete\n");
		free(paquete);
	}


//	t_tipoEstructura tipoRecibido;
//	void* structRecibida;
	while(1){
		/*socket_recibir(sock_kernel_servidor, &tipoRecibido, &structRecibida);
		char* cadenaAImprimir = (char*) structRecibida;
		if(cadenaAImprimir==0){
			cerrarSocket(sock_kernel_servidor);
			return EXIT_SUCCESS;
		}
		if(cadenaAImprimir==1){
			printf("No hay memoria suficiente para el programa\n");
			cerrarSocket(sock_kernel_servidor);
			return EXIT_FAILURE;
		}

		for(i=0; cadenaAImprimir[i]!=NULL;i++){
			printf("%c", cadenaAImprimir[i]);
		}*/
	}








	return 0;
}
