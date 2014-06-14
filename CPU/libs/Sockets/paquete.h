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
#define STRUCT_POP_POSITION 2
#define STRUCT_SIGNAL 3
#define STRUCT_NUMERO 4
#define STRUCT_CHAR 5
#define STRUCT_STRING 6
#define STRUCT_ASIGNAR_COMPARTIDA 7
#define STRUCT_OBTENER_COMPARTIDA 8
#define STRUCT_TIPO_INSTRUCCION 9
#define STRUCT_SEMAFORO 10
#define STRUCT_MODIFICAR_TOP_INDEX 11

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

void destruirPaquete(t_stream* paquete);
t_stream* serializar(int type, void* estructura);
t_header cargarData(t_estructura estructura, uint32_t length);
char* crearData(t_estructura estructura, uint32_t length);
t_stream* paquetePopPosition(struct_pop_position* estructura);
t_stream* paquetePush(struct_push* estructura);
t_stream* paqueteSignal(struct_signal* estructura);
t_stream* paqueteNumero(struct_numero* estructura);
t_stream* paqueteChar(struct_char* estructura);
t_stream* paqueteString(struct_string* estructura);
t_stream* paqueteAsignarCompartida(struct_asignar_compartida* estructura);
t_stream* paqueteTipoInstruccion(struct_tipo_instruccion * estructura);
t_stream* paqueteSemaforo(struct_semaforo* estructura);
t_stream* paqueteObtenerCompartida(struct_string* estructura);
t_stream* paqueteModificarTopIndex(struct_modificar_top_index* estructura);


struct_push* sacarPaquetePush(char* data, uint32_t length);
struct_pop_position* sacarPaquetePopPosition(char* dataPaquete, uint32_t length);
struct_signal* sacarPaqueteSignal(char* data, uint32_t length);
struct_numero* sacarPaqueteNumero(char* data, uint32_t length);
struct_char* sacarPaqueteChar(char* data, uint32_t length);
struct_string* sacarPaqueteString(char* data, uint32_t length);
struct_asignar_compartida* sacarPaqueteAsignarCompartida(char* data, uint32_t length);
struct_tipo_instruccion * sacarPaqueteTipoInstruccion(char * dataPaquete, uint32_t length);
struct_semaforo* sacarPaqueteSemaforo(char* dataPaquete, uint32_t length);
struct_modificar_top_index* sacarPaqueteModificarTopIndex(char* dataPaquete, uint32_t length);
void *deserializar(int type, char* data, uint32_t length);
t_header despaquetizarHeader(char * header);

#endif /* PAQUETE_H_ */
