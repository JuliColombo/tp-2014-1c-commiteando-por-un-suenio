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

extern int stack;
extern int cursor;

t_struct_pcb* pcb;

t_puntero calcularPosicionAsignacionCPU(int top_index);

t_puntero calcularPosicionAsignacion(int top_index);

void insertarEnDiccionario(t_nombre_variable identificador_variable,t_puntero posicion);

t_puntero_instruccion irAIntruccionLabel(t_nombre_etiqueta etiqueta);

void recibirProximaInstruccion(int sockUMV);

int esPrimerContexto();

t_puntero calcularPosicionAsignacionCPU(int top_index);

int esPar(int numero);

t_puntero calcularPosicionAsignacion(int top_index);

char* convertirAString(t_nombre_variable c);

t_elemento* elemento_create(const char* name, t_puntero pos);

void elemento_delete(t_elemento* elemento);

void reservarContextoSinRetorno();

void reservarContextoConRetorno(t_puntero donde_retornar);

void recuperarPosicionDeDirecciones();

void recuperarProgramCounter(t_puntero* program_counter);

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo);

void volverAContextoAnterior(t_puntero* c_stack_viejo);

void guardarAlternado ();

void regenerarDiccionario(int tamanio_contexto);

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_viejo);

void recuperarDireccionRetorno(t_puntero* direccion_retorno);

t_intructions instruccionParaBuscarEnIndiceCodigo(t_puntero_instruccion instruccion);


#endif /* PRIMITIVASAUX_H_ */
