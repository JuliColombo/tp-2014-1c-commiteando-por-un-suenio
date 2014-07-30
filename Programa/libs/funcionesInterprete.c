/*
 * funcionesInterprete.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */


#include "funcionesInterprete.h"


void leerScript(off_t* fileSize,char* path) {

	struct stat file_info;
	int control = lstat(path, &file_info);
	codigo_ansisop = NULL;

	if (control == -1) {
		log_escribir(archLog, "Leer Script", ERROR, "El archivo no existe");
		*fileSize = 0;

	}else{
		long f_size;
		int c;
		FILE* script = fopen(path, "r");
		fseek(script, 0L, SEEK_END);
		f_size = ftell(script);
		fseek(script, 0L, SEEK_SET);
		codigo_ansisop = malloc(sizeof(char)*f_size);
		fread(codigo_ansisop,1,f_size,script);
	    fclose(script);


	}

}


int validarScript(void){
	int i=0;
	char* aux = malloc(18*sizeof(char));
	while(codigo_ansisop[i]!='\n'){
		printf("%c",codigo_ansisop[i]);
		aux[i]=codigo_ansisop[i];
		i++;
	}
	int resultado = strcmp(aux,"#!/usr/bin/ansisop");
	free(aux);
	return resultado;
}



/************************* FUNCIONES GENERALES *************************/

void inicializarConfiguracion(void) {
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH_config, &file_info);
	if (control == -1) {
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		}
	else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH_config);

	configuracion_programa.ip_kernel = config_get_string_value(config,"Direccion IP para conectarse al Kernel");
	configuracion_programa.puerto_kernel = config_get_int_value(config,"Puerto TCP para recibir conexiones del Kernel");

	}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("IP del Kernel: %s\n", configuracion_programa.ip_kernel);
	printf("Puerto Kernel: %d\n", configuracion_programa.puerto_kernel);

}

int abrirSocket(void){
	int sock;
	if((sock=socket_crearYConectarCliente(configuracion_programa.ip_kernel,configuracion_programa.puerto_kernel))==-1){
			log_escribir(archLog, "Conexion", ERROR, "No se pudo conectar al Kernel");
			abort();
	}else{
		log_escribir(archLog, "Conexion", INFO, "Se conectó correctamente con el kernel");
	}
	return sock;
}

void cerrarSocket(int sock){
	int socket_cerrado;
	if((socket_cerrado=socket_cerrarConexion(sock))==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar la conexion con el Kernel");
	}else{
		log_escribir(archLog,"Conexión",INFO,"La conexión se cerró correctamente");
	}
}


