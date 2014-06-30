/*
 * funcionesInterprete.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */


#include "funcionesInterprete.h"


char* leerScript(off_t* fileSize,char* path) {

	struct stat file_info;
	int control = lstat(path, &file_info);
	char* buffer = NULL;

	if (control == -1) {
		log_escribir(archLog, "Leer Script", ERROR, "El archivo no existe");
		*fileSize = 0;

	}else{
		*fileSize = file_info.st_size;
		FILE* script = fopen(path, "r");
		buffer = malloc(*fileSize);
		fread(buffer, *fileSize, sizeof(char), script);
		fclose(script);

	}

	return buffer;
}


int validarScript(void){
	int i=0;
	char* aux = malloc(18*sizeof(char));

	while(archivo_ansisop[i]!='\n'){
		aux[i]=archivo_ansisop[i];
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


