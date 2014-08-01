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
#include "estructuras.h"
#include <stdint.h>
#include "Sockets/socket.h"
#include <signal.h>

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;

enum{
	CONTINUES = 0,
	DONE = 5,
	QUANTUM = 3,
	SEG_FAULT = 4,
	//IO = 6,
	BLOQUEADO = 7,
}numeros_utiles;

extern int stack;
extern int cursor;
extern int termino; //tiene que ser extern??

t_dictionary *diccionario;
int sockUMV;
int top_index;

char* proximaInstruccion;
char* indiceEtiquetas;
int esConRetorno;
int recienReserve;


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

void chequearSiHuboSF();

void controlarBloqueo(int sockKernel, int termino);


#endif /* PRIMITIVASAUX_H_ */
