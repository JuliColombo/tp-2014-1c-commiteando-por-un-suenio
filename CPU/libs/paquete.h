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

#define STRUCT_PUSH 0
#define STRCUT_POP 1
#define STRUCT_POP_DESREFERENCIAR 2
#define STRUCT_POP_RETORNAR 3

typedef struct {
	uint8_t Type;
	uint32_t Lenght;

}__attribute__((packed)) t_nipc;

typedef struct {

	uint32_t length;
	char* buffer;
}__attribute__((packed)) t_stream ;

typedef struct {
	uint32_t posicion;
	char id;
}__attribute__((packed)) struct_push;

typedef struct {

} __attribute__((packed)) struct_pop;

typedef struct {
	uint32_t posicion;
} __attribute__((packed)) struct_pop_desreferenciar;

typedef struct {
	uint32_t posicion;
} __attribute__((packed)) struct_pop_retornar;

typedef enum {
	PUSH_SIZE_CHECK,
	POP,
	POP_DESREFERENCIAR,
	POP_RETORNAR,
}tipo_operacion;

t_stream* nipc_paquetePopRetornar(struct_pop_retornar* estructura);
t_stream* nipc_paquetePopDesreferenciar(struct_pop_desreferenciar* estructura);
t_stream* nipc_paquetePop(struct_pop* estructura);
t_stream* nipc_paquetePush(struct_push* estructura);
t_stream* nipc_crearPaquete(char type,int length, char* payload);
//void nipc_destruirPaquete(t_nipc* paquete);
t_stream* nipc_serializar2(int type, void* estructura);
#endif /* PAQUETE_H_ */
