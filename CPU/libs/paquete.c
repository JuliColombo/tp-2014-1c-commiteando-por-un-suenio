/*
 * paquete.c
 *
 *  Created on: 29/05/2014
 *      Author: utnso
 */

#include "paquete.h"

/*t_nipc nipc_crearPaquete(char type, int length, char* payload) {

	t_nipc paquete;
	paquete.Type = type;
	paquete.Lenght = length;
	paquete.Payload = malloc(length);
	memcpy(paquete.Payload, payload, length);
	return paquete;

}

void nipc_destruirPaquete(t_nipc* paquete) {

	if(paquete->Lenght > 0) {

		free(paquete->Payload);
		paquete->Payload=NULL;
	}

}*/

t_nipc cargarData(uint8_t estructura, uint32_t length) {
	t_nipc package;
	package.Type = estructura;
	package.Lenght = length;
	return package;

}

char* crearData(uint8_t estructura, uint32_t length) {
	char* data = malloc(length);
	uint32_t lengthDatos = length - sizeof(t_nipc);
	t_nipc package = cargarData(estructura, lengthDatos);

	uint32_t tamanioDato=0;

	memcpy(data,&package.Type,tamanioDato = sizeof(package.Type));
	memcpy(data+tamanioDato, &package.Lenght, sizeof(package.Lenght));

	return data;

}

t_stream* nipc_paquetePush(struct_push* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_nipc) + sizeof(estructura->id) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_PUSH,paquete->length);

	int tamanioDato = 0;

	memcpy(data, &estructura->id, tamanioDato = sizeof(estructura->id));
	memcpy(data + tamanioDato, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	return paquete;
}

t_stream* nipc_paquetePop(struct_pop* estructura) {
	t_stream* paquete;

	//Aca me las tengo que ingeniar para decirle a la UMV que popee, pero no le mando nada :(

	return paquete;
}

t_stream* nipc_paquetePopDesreferenciar(struct_pop_desreferenciar* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_nipc) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_POP_DESREFERENCIAR,paquete->length);

	memcpy(data, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	return paquete;
}

t_stream* nipc_paquetePopRetornar(struct_pop_retornar* estructura) {
	t_stream* paquete;

	paquete = malloc(sizeof(t_stream));
	paquete->length = sizeof(t_nipc) + sizeof(estructura->posicion);

	char* data = crearData(STRUCT_POP_RETORNAR,paquete->length);

	memcpy(data, &estructura->posicion, sizeof(estructura->posicion));

	paquete->buffer = data;
	return paquete;
}

t_stream* nipc_serializar2(int type, void* estructura) {
	t_stream* stream = NULL;
	switch(type) {
	case PUSH_SIZE_CHECK:
		stream = nipc_paquetePush((struct_push*) estructura);
		break;
	case POP:
		stream = nipc_paquetePop((struct_pop*) estructura);
		break;
	case POP_DESREFERENCIAR:
		stream = nipc_paquetePopDesreferenciar((struct_pop_desreferenciar*) estructura);
		break;
	case POP_RETORNAR:
		stream = nipc_paquetePopRetornar((struct_pop_retornar*) estructura);
		break;
	}
	return stream;
}
