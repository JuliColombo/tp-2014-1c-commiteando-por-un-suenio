/*
 * paquete.c
 *
 *  Created on: 29/05/2014
 *      Author: utnso
 */

#include "paquete.h"

/***********************************************************************SERIALIZACIONES**************************************************/

void nipc_destruirPaquete(t_stream* paquete) {

	if(paquete->length > 0) {

		free(paquete->buffer);
		paquete->buffer=NULL;
	}

}

//DESTRUIR PAQUETE DESPUES DE SERIALIZAR

t_stream* serializar(int type, void* estructura) {
	t_stream* stream = NULL;
	switch(type) {
	case PUSH_SIZE_CHECK:
		stream = paquetePush((struct_push*) estructura);
		break;
	case POP:
		stream = paquetePop((struct_pop*) estructura);
		break;
	case POP_DESREFERENCIAR:
		stream = paquetePopDesreferenciar((struct_pop_desreferenciar*) estructura);
		break;
	case POP_RETORNAR:
		stream = paquetePopRetornar((struct_pop_retornar*) estructura);
		break;
	}
	return stream;
}

t_header cargarData(t_estructura estructura, uint32_t length) {
	t_header package;
	package.Type = estructura;
	package.Lenght = length;
	return package;

}

char* crearData(t_estructura estructura, uint32_t length) {
	char* data = malloc(length);
	uint32_t lengthDatos = length - sizeof(t_header);
	t_header package = cargarData(estructura, lengthDatos);

	uint32_t tamanioDato=0;

	memcpy(data,&package.Type,tamanioDato = sizeof(package.Type));
	memcpy(data+tamanioDato, &package.Lenght, sizeof(package.Lenght));

	return data;

}

t_stream* paquetePush(struct_push* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_header) + sizeof(estructura->id) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_PUSH,paquete->length);

	int tamanioDato = 0;

	memcpy(data, &estructura->id, tamanioDato = sizeof(estructura->id));
	memcpy(data + tamanioDato, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	free(data); //??
	return paquete;
}

t_stream* paquetePop(struct_pop* estructura) {
	t_stream* paquete =NULL;

	//Aca me las tengo que ingeniar para decirle a la UMV que popee, pero no le mando nada :(

	return paquete;
}

t_stream* paquetePopDesreferenciar(struct_pop_desreferenciar* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_header) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_POP_DESREFERENCIAR,paquete->length);

	memcpy(data, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	free(data); //??
	return paquete;
}

t_stream* paquetePopRetornar(struct_pop_retornar* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_header) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_POP_RETORNAR,paquete->length);

	memcpy(data, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	free(data); //??
	return paquete;
}

/***********************************************************************DESERIALIZACIONES**************************************************/


struct_push* sacarPaquetePush(char* data, uint32_t length) {
	struct_push* estructuraDestino = malloc(sizeof(struct_push));

	int tamanioDato = 0;
	memcpy(&estructuraDestino->id,data,tamanioDato = sizeof(estructuraDestino->id));
	memcpy(&(estructuraDestino->posicion) + tamanioDato, data+tamanioDato, sizeof(estructuraDestino->posicion));

	return estructuraDestino;

}

struct_push* sacarPaquetePop(char* data, uint32_t length) {
	struct_push* estructuraDestino = malloc(sizeof(struct_push));

	//Ver que pongo aca

	return estructuraDestino;

}

struct_push* sacarPaquetePopDesreferenciar(char* data, uint32_t length) {
	struct_push* estructuraDestino = malloc(sizeof(struct_push));

	memcpy(&estructuraDestino->posicion, data, sizeof(estructuraDestino->posicion));

	return estructuraDestino;

}

struct_push* sacarPaquetePopRetornar(char* data, uint32_t length) {
	struct_push* estructuraDestino = malloc(sizeof(struct_push));

	memcpy(&estructuraDestino->posicion, data, sizeof(estructuraDestino->posicion));

	return estructuraDestino;

}


//HACER FREE DE ESTRUCTURA DESTINO DESPUES DE USAR LA FUNCION
void *deserializar(int type, char* data, uint32_t length) {
	void* estructuraDestino = NULL;

	switch(type) {
	case PUSH_SIZE_CHECK:
		estructuraDestino = sacarPaquetePush(data, length);
		break;
	case POP:
		estructuraDestino = sacarPaquetePop(data,length);
		break;
	case POP_DESREFERENCIAR:
		estructuraDestino = sacarPaquetePopDesreferenciar(data,length);
		break;
	case POP_RETORNAR:
		estructuraDestino = sacarPaquetePopRetornar(data,length);
		break;
	}
return estructuraDestino;
}

t_header despaquetizarHeader(char * header){
	t_header estructuraHeader;

	int tamanoTotal = 0, tamanoDato = 0;
	memcpy(&estructuraHeader.Type, header + tamanoTotal, tamanoDato = sizeof(uint8_t));
	tamanoTotal = tamanoDato;
	memcpy(&estructuraHeader.Lenght, header + tamanoTotal, tamanoDato = sizeof(uint16_t));

	return estructuraHeader;
}
