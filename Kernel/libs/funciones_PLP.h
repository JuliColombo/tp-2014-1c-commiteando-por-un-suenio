/*
 * FuncionesPLP.h
 *
 *  Created on: 25/04/2014
 *      Author: utnso
 */

#ifndef FUNCIONESPLP_H_
#define FUNCIONESPLP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones_PLP.h"
#include "estructuras_kernel.h"
#include "parser/metadata_program.h"
#include "commons/config.h"
#include "commons/collections/list.h"
#include "log.h"

extern t_config_kernel configuracion_kernel;
extern log_t* archLog;
extern char* PATH;

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

void calcularPeso (t_programa programa);

void agregarAListaSegunPeso(t_programa programa, t_list* lista);

void mostrarNodosPorPantalla(t_list* lista);
void inicializarConfiguracion(char*);
void leerConfiguracion(char*);

void imprimirConfiguracion(t_config_kernel configuracion);

#endif /* FUNCIONESPLP_H_ */
