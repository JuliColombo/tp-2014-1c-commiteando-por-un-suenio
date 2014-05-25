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

	diccionario = dictionary_create();
	pila = CREATE_STACK(1000);

	definirVariable('a');
	definirVariable('b');
	definirVariable('c');

	t_puntero a = obtenerPosicionVariable('a')+1;
	t_puntero b = obtenerPosicionVariable('b')+1;
	t_puntero c = obtenerPosicionVariable('c')+1;

	asignar(a, 10);
	asignar(b,-15);
	asignar(c,9);

	printf("para la variable a, tenemos el valor %d\n",dereferenciar(obtenerPosicionVariable('a')));
	printf("para la variable b, tenemos el valor %d\n",dereferenciar(obtenerPosicionVariable('b')));
	printf("para la variable c, tenemos el valor %d\n",dereferenciar(obtenerPosicionVariable('c')));


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
