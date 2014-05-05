/*
 * estructuras_umv.h
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_UMV_H_
#define ESTRUCTURAS_UMV_H_

#include "parser/metadata_program.h"

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

typedef struct t_config_UMV {
uint32_t MemSize;
} t_config_UMV;

enum tipoAlgoritmo{
					firstfit,worstfit
};



#endif /* ESTRUCTURAS_UMV_H_ */
