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
#define STRCUT_POP 2
#define STRUCT_POP_DESREFERENCIAR 3
#define STRUCT_POP_RETORNAR 4
#define STRUCT_SIGNAL 5

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
t_stream* paquetePop(struct_pop* estructura);
t_stream* paquetePush(struct_push* estructura);

struct_push* sacarPaquetePush(char* data, uint32_t length);
struct_push* sacarPaquetePop(char* data, uint32_t length);
struct_push* sacarPaquetePopDesreferenciar(char* data, uint32_t length);
struct_push* sacarPaquetePopRetornar(char* data, uint32_t length);
void *deserializar(int type, char* data, uint32_t length);
t_header despaquetizarHeader(char * header);

#endif /* PAQUETE_H_ */
