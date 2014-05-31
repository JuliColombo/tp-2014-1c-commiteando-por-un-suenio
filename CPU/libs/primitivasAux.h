/*
 * primitivasAux.h
 *
 *  Created on: 22/05/2014
 *      Author: utnso
 */

#ifndef PRIMITIVASAUX_H_
#define PRIMITIVASAUX_H_

#include <parser/metadata_program.h>
#include <commons/collections/dictionary.h>
#include "Sockets/socket.h"

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;


t_puntero calcularPosicionAsignacionCPU(int top_index);

void reservarContextoSinRetorno();

void reservarContextoConRetorno();

char* convertirAString(t_nombre_variable c);

t_elemento* elemento_create(const char* name, t_puntero pos);

void elemento_delete(t_elemento* elemento);

void regenerarDiccionario(int tamanio_contexto);

void guardarAlternado ();

void volverAContextoAnterior();


#endif /* PRIMITIVASAUX_H_ */
