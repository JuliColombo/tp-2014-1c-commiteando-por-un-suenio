/*
 * paquete.h
 *
 *  Created on: 29/05/2014
 *      Author: utnso
 */

#ifndef PAQUETE_H_
#define PAQUETE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "estructuras.h"

#define S_ERROR 0
#define STRUCT_PUSH 1
#define STRUCT_POP_DESREFERENCIAR 2
#define STRUCT_POP_RETORNAR 3
#define STRUCT_SIGNAL 4
#define STRUCT_NUMERO 5
#define STRUCT_CHAR 6
#define STRUCT_STRING 7
#define STRUCT_ASIGNAR_COMPARTIDA 8
#define STRUCT_TIPO_INSTRUCCION 9

typedef uint8_t t_estructura;

typedef struct {
	uint8_t Type;
	uint32_t Lenght;

}__attribute__((packed)) t_header;


typedef struct {

	uint32_t length;
	char* buffer;
}__attribute__((packed)) t_stream ;

/****************************************************FUNCIONES******************************************************/

void nipc_destruirPaquete(t_stream* paquete);
t_stream* serializar(int type, void* estructura);
t_header cargarData(t_estructura estructura, uint32_t length);
char* crearData(t_estructura estructura, uint32_t length);
t_stream* paquetePopRetornar(struct_pop_retornar* estructura);
t_stream* paquetePopDesreferenciar(struct_pop_desreferenciar* estructura);
t_stream* paquetePush(struct_push* estructura);
t_stream* paqueteSignal(struct_signal* estructura);
t_stream* paqueteNumero(struct_numero* estructura);
t_stream* paqueteChar(struct_char* estructura);
t_stream* paqueteString(struct_string* estructura);
t_stream* paqueteAsignarCompartida(struct_asignar_compartida* estructura);
t_stream * paqueteTipoInstruccion(struct_tipo_instruccion * estructuraOrigen);

struct_push* sacarPaquetePush(char* data, uint32_t length);
struct_push* sacarPaquetePop(char* data, uint32_t length);
struct_pop_desreferenciar* sacarPaquetePopDesreferenciar(char* data, uint32_t length);
struct_pop_retornar* sacarPaquetePopRetornar(char* data, uint32_t length);
struct_signal* sacarPaqueteSignal(char* data, uint32_t length);
struct_numero* sacarPaqueteNumero(char* data, uint32_t length);
struct_char* sacarPaqueteChar(char* data, uint32_t length);
struct_string* sacarPaqueteString(char* data, uint32_t length);
struct_asignar_compartida* sacarPaqueteAsignarCompartida(char* data, uint32_t length);
struct_tipo_instruccion * sacarPaqueteTipoInstruccion(char * dataPaquete, uint32_t length);
void *deserializar(int type, char* data, uint32_t length);
t_header despaquetizarHeader(char * header);

#endif /* PAQUETE_H_ */
