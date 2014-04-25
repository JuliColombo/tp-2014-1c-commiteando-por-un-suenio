/*
 * FuncionesPLP.h
 *
 *  Created on: 25/04/2014
 *      Author: utnso
 */

#ifndef FUNCIONESPLP_H_
#define FUNCIONESPLP_H_

#include "commons/collections/list.h"
#include "parser/metadata_program.h"

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

void calcularPeso (t_programa programa);

void ordenarPorPeso (int p[], int n);

#endif /* FUNCIONESPLP_H_ */
