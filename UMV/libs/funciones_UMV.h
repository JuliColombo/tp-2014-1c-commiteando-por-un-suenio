/*
 * UMV.h
 *
 *  Created on: 25/04/2014
 *      Author: utnso
 */

#ifndef UMV_H_
#define UMV_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "commons/collections/dictionary.h"
#include "estructuras_umv.h"
#include <string.h>
#include "parser/metadata_program.h"
#include "estructuras_umv.h"
#include "funciones_UMV.h"
#include "commons/config.h"
#include "commons/collections/list.h"

/*Macros del archivo config*/

#define PATHCONFIG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio/UMV/configuracion_UMV.cfg"

/*Variables globales*/

extern t_config_UMV configuracion_UMV;

/*Prototipos de fuciones*/

_Bool solicitarMemoria(t_programa prgrama);

int estaEnDicOP(char palabra[]);
int estaEnDicTOP(char palabra[]);
void algoritmo(int* algor);
void leerConfiguracion(void);				  				 			 // Lee en la macro del archivo de configuracion ubicado en la ruta PATH
void imprimirConfiguracion(void);			  				 			 // Muestra por pantalla todos los campos de la macro del archivo de configuracion



#endif /* UMV_H_ */

