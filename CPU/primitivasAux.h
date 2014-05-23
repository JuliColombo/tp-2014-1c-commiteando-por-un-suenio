/*
 * primitivasAux.h
 *
 *  Created on: 22/05/2014
 *      Author: utnso
 */

#ifndef PRIMITIVASAUX_H_
#define PRIMITIVASAUX_H_

#include <parser/metadata_program.h>
#include "stack.h"
#include <commons/collections/dictionary.h>

int comprendidoEntre(int m, int n, int i);

t_valor_variable nombreParametro(int i);

t_puntero calcularPosicionAsignacion(t_stack* P);

void reservarContextoSinRetorno();

void reservarContextoConRetorno();


#endif /* PRIMITIVASAUX_H_ */
