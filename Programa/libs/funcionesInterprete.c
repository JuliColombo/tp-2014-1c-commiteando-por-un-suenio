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
	}

	else {

		*fileSize = file_info.st_size;
		FILE* script = fopen(path, "r");
		buffer = malloc(*fileSize);
		fread(buffer, *fileSize, sizeof(char), script);
		buffer[*fileSize - 1] = '\0';
		fclose(script);

	}

	return buffer;
}

void inicializarConfiguracion(void) {
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(Archivo_Ansisop, &file_info);
	if (control == -1) {
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		}
	else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_programa.ip_kernel = config_get_int_value(config,"Puerto TCP para recibir conexiones del Kernel");
	configuracion_programa.puerto_kernel = config_get_int_value(config,"Direccion IP para conectarse al Kernel");

	}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("IP del Kernel: %d\n", configuracion_programa.ip_kernel);
	printf("Puerto Kernel: %d\n", configuracion_programa.puerto_kernel);

}

