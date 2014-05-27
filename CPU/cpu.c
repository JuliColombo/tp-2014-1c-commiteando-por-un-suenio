/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include "cpu.h"

t_dictionary* diccionario;
int top_index = -1;


config_cpu configuracion_cpu;
char* PATH;


int main (int argc, char **argv){
	PATH=argv[1];
	leerConfiguracion(PATH);

	//Cuando me conecto a UMV, que me diga si la pila esta vacia y me guardo su valor de top_index. Si esta vacia, es -1, sino lo que corresponda.
	//Con el top_index voy a poder calcular la posicion a pushear


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
