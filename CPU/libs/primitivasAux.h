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

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;



int comprendidoEntre(int m, int n, int i);

t_valor_variable nombreParametro(int i);

t_puntero calcularPosicionAsignacion(t_stack* P);

void reservarContextoSinRetorno();

void reservarContextoConRetorno();

char* convertirAString(t_nombre_variable c);

t_elemento* elemento_create(const char* name, t_puntero pos);

void elemento_delete(t_elemento* elemento);

void regenerarDiccionario(t_stack* pila, int tamanio_contexto);

void guardarAlternado (t_stack* pila);


#endif /* PRIMITIVASAUX_H_ */
