/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include "cpu.h"

t_dictionary* diccionario;
t_stack* pila;


config_cpu configuracion_cpu;
char* PATH;


int main (int argc, char **argv){
	PATH=argv[1];
	leerConfiguracion(PATH);


	return 0;
}

/***************************************  FUNCIONES  ***************************************/

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_cpu.ip_kernel=config_get_int_value(config,"Direccion IP para conectarse al Kernel");
	configuracion_cpu.puerto_kernel=config_get_int_value(config,"Puerto TCP para conectarse al Kernel");
	configuracion_cpu.ip_umv=config_get_int_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_cpu.puerto_umv=config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
}
