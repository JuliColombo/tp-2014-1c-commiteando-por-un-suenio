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
	case STRUCT_PUSH:
		stream = paquetePush((struct_push*) estructura);
		break;
	case STRUCT_POP_DESREFERENCIAR:
		stream = paquetePopDesreferenciar((struct_pop_desreferenciar*) estructura);
		break;
	case STRUCT_POP_RETORNAR:
		stream = paquetePopRetornar((struct_pop_retornar*) estructura);
		break;
	case STRUCT_NUMERO:
		stream = paqueteNumero((struct_numero*) estructura);
		break;
	case STRUCT_SIGNAL:
		stream = paqueteSignal((struct_signal*) estructura);
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

t_stream * paqueteSignal(struct_signal * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(struct_signal);

	char * data = crearData(STRUCT_SIGNAL, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(struct_signal));	//copio a data la estructura.

	paquete->buffer = data;

	return paquete;
}

t_stream * paqueteNumero(struct_numero * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearData(STRUCT_NUMERO, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(struct_numero));		//copio a data el numero.

	paquete->buffer = data;

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


struct_pop_desreferenciar* sacarPaquetePopDesreferenciar(char* data, uint32_t length) {
	struct_pop_desreferenciar* estructuraDestino = malloc(sizeof(struct_pop_desreferenciar));

	memcpy(&estructuraDestino->posicion, data, sizeof(estructuraDestino->posicion));

	return estructuraDestino;

}

struct_pop_retornar* sacarPaquetePopRetornar(char* data, uint32_t length) {
	struct_pop_retornar* estructuraDestino = malloc(sizeof(struct_pop_retornar));

	memcpy(&estructuraDestino->posicion, data, sizeof(estructuraDestino->posicion));

	return estructuraDestino;

}

struct_signal * sacarPaqueteSignal(char * dataPaquete, uint32_t length){
	struct_signal * estructuraDestino = malloc(sizeof(struct_signal));

	memcpy(estructuraDestino, dataPaquete, sizeof(struct_signal)); //copio el data del paquete a la estructura

	return estructuraDestino;
}

struct_numero * sacarPaqueteNumero(char * dataPaquete, uint32_t length){
	struct_numero * estructuraDestino = malloc(sizeof(struct_numero));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}

//HACER FREE DE ESTRUCTURA DESTINO DESPUES DE USAR LA FUNCION
void *deserializar(int type, char* data, uint32_t length) {
	void* estructuraDestino = NULL;

	switch(type) {
	case STRUCT_PUSH:
		estructuraDestino = sacarPaquetePush(data, length);
		break;
	case STRUCT_POP_DESREFERENCIAR:
		estructuraDestino = sacarPaquetePopDesreferenciar(data,length);
		break;
	case STRUCT_POP_RETORNAR:
		estructuraDestino = sacarPaquetePopRetornar(data,length);
		break;
	case STRUCT_NUMERO:
		estructuraDestino = sacarPaqueteNumero(data,length);
		break;
	case STRUCT_SIGNAL:
		estructuraDestino = sacarPaqueteSignal(data,length);
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
