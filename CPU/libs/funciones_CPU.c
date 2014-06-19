/*
 * funciones_CPU.c
 *
 *  Created on: 13/06/2014
 *      Author: utnso
 */
#include "funciones_CPU.h"
#include "Sockets/estructuras_socket.h"
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


void* core_conexion_kernel(void){
	int sock;
	if((sock=socket_crearYConectarCliente(configuracion_cpu.ip_kernel,configuracion_cpu.puerto_kernel))==-1){
		log_error_socket();
	}
	while(1){

	}
	if(socket_cerrarConexion(sock)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return NULL;
}

void* core_conexion_umv(void){
	int sock;
	if ((sock=socket_crearYConectarCliente(configuracion_cpu.ip_umv, configuracion_cpu.puerto_umv))>0){
			printf("Conectado a la UMV\n");
		}

	/*t_struct_numero* i = malloc(sizeof(t_struct_numero));
	 uint32_t k=5;
	 i->numero=k;
	 printf("el valor que se manda es %d\n", k);
	 int j=socket_enviar(sock, D_STRUCT_NUMERO, i);
	 if(j==1){
	 printf("Se envio bien el paquete\n");
	 free(i);*/

	t_struct_string* i = malloc(sizeof(t_struct_string));
		 char* k = "argentina";
		 i->string=k;
		 printf("el valor que se manda es %s\n", i->string);
		 int j=socket_enviar(sock, D_STRUCT_STRING, i);
		 if(j==1){
		 printf("Se envio bien el paquete\n");
		 free(i);

	/*t_struct_char* i = malloc(sizeof(t_struct_char));
		 char k = 'j';
		 i->letra=k;
		 printf("el valor que se manda es %c\n", k);
		 int j=socket_enviar(sock, D_STRUCT_CHAR, i);
		 if(j==1){
		 printf("Se envio bien el paquete\n");
		 free(i);*/


}
	if(socket_cerrarConexion(sock)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return NULL;
}

