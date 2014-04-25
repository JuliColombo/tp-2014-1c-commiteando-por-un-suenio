/*
 * UMV.c
 *
 *  Created on: 21/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "commons/config.h"
#include "commons/collections/dictionary.h"

//Al iniciar, la UMV crea la MP de un tamaño configurable por un archivo de configuracion, que supuse era
//el tamaño maximo que puede tener el diccionario de ese archivo de configuracion
int* crearMP(t_config archConfig) {
	int tamanio = (archConfig.properties)->table_max_size;
	int* MP = malloc(tamanio);

	return MP;
}


//Funcion que recive el programa del PLP y le reserva memoria (si puede)
void solicitarMemoria(t_programa prgrama){
}
