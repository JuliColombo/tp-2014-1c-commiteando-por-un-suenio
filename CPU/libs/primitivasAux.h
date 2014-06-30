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
#include "Sockets/crear_estructuras.h"

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;

t_struct_pcb pcb;

AnSISOP_kernel (*funciones_kernel) (const char* string);
AnSISOP_funciones (*funciones_parser) (const char* string);

t_puntero calcularPosicionAsignacionCPU(int top_index);

void reservarContextoSinRetorno();

void reservarContextoConRetorno();

char* convertirAString(t_nombre_variable c);

t_elemento* elemento_create(const char* name, t_puntero pos);

void elemento_delete(t_elemento* elemento);

void regenerarDiccionario(int tamanio_contexto);

void guardarAlternado ();

void volverAContextoAnterior();

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_actual);

t_puntero recuperarDireccionRetorno();

void recuperarPosicionDeDirecciones();

void recuperarProgramCounter(t_puntero* program_counter);

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo);


#endif /* PRIMITIVASAUX_H_ */
