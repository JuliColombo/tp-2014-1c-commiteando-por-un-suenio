/*
 * funciones_CPU.c
 *
 *  Created on: 13/06/2014
 *      Author: utnso
 */
#include "funciones_CPU.h"

config_cpu configuracion_cpu;


void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1){
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
	}
	else{
	leerConfiguracion(PATH);
	imprimirConfiguracion();
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_cpu.ip_kernel=config_get_string_value(config,"Direccion IP para conectarse al Kernel");
	configuracion_cpu.puerto_kernel=config_get_int_value(config,"Puerto TCP para conectarse al Kernel");
	configuracion_cpu.ip_umv=config_get_string_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_cpu.puerto_umv=config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
}

void imprimirConfiguracion(void){
	printf("Direccion IP para conectarse al Kernel: %s\n", configuracion_cpu.ip_kernel);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_cpu.puerto_kernel);
	printf("Direccion IP para conectarse a la UMV: %s\n", configuracion_cpu.ip_umv);
	printf("Puerto para conexiones con UMV: %d\n", configuracion_cpu.puerto_umv);
}

void log_error_socket(void){
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
}

/******************************** CONEXION KERNEL ***************************************************/
void* core_conexion_kernel(void){
	int sock;
	if((sock=socket_crearYConectarCliente(configuracion_cpu.ip_kernel,configuracion_cpu.puerto_kernel))==-1){
		log_error_socket();
		printf("se conecto al kernel\n");
	}

	//CODEO ACA
	recibir_quantum(sock);
	recibir_retardo_quantum(sock);

	if(socket_cerrarConexion(sock)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return NULL;
}

void recibir_quantum(int sockKernel) {
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockKernel);
	quantum = estructura->numero;
}

void recibir_retardo_quantum(int sockKernel) {
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockKernel);
	retardo = estructura->numero;
}

/******************************** CONEXION UMV ***************************************************/

void* core_conexion_umv(void){
	int sock;
	if ((sock=socket_crearYConectarCliente(configuracion_cpu.ip_umv, configuracion_cpu.puerto_umv))>0){
			log_error_socket();
			printf("Conectado a la UMV\n");
		}

	//CODEO ACA


	if(socket_cerrarConexion(sock)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar a la UMV");
	}
	return NULL;
}

