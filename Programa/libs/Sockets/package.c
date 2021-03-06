/*
 * package.c
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */

#include "package.h"

/******************** PAQUETIZAR *************************/

/*
 * Nombre: paquetizar/2
 * Argumentos:
 * 		- tipoEstructura (un int, bajo criterio nuestro que representará la estructura que estoy enviando)
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 * 		--> Recordar hacer free del paquete luego de utilizar esta función.
 *
 * Funcion: Según el tipo de estructura, va a distintas funciones que las paquetizan.
 */
t_stream * paquetizar(int tipoEstructura, void * estructuraOrigen){
	t_stream * paquete=NULL;

	switch (tipoEstructura){
			case D_STRUCT_NOMBREMENSAJE:
				paquete = paquetizarStruct_nombreMensaje((t_struct_nombreMensaje *) estructuraOrigen);
				break;
			case D_STRUCT_NUMERO:
				paquete = paquetizarStruct_numero((t_struct_numero *) estructuraOrigen);
				break;
			case D_STRUCT_CHAR:
				paquete = paquetizarStruct_char((t_struct_char *) estructuraOrigen);
				break;
			case D_STRUCT_STRING:
				paquete = paquetizarStruct_string((t_struct_string *) estructuraOrigen);
				break;
			case D_STRUCT_SIGNAL:
				paquete = paquetizarStruct_signal((t_struct_signal *) estructuraOrigen);
				break;
			case D_STRUCT_PCB:
				paquete = paquetizarStruct_pcb((t_struct_pcb *) estructuraOrigen);
				break;
			case D_STRUCT_GRADOMP:
				paquete = paquetizarStruct_numero((t_struct_gradoMP*) estructuraOrigen);
				break;
			case D_STRUCT_PIDYCODIGO:
				paquete = paquetizarStruct_pidycodigo((t_struct_pidycodigo*) estructuraOrigen);
				break;
			case D_STRUCT_PUSH:
				paquete = paquetizarStruct_push((t_struct_push*) estructuraOrigen);
				break;
			case D_STRUCT_POP:
				paquete = paquetizarStruct_pop((t_struct_pop*) estructuraOrigen);
				break;
			case D_STRUCT_ASIGNARCOMPARTIDA:
				paquete = paquetizarStruct_asignarCompartida((t_struct_asignar_compartida*) estructuraOrigen);
				break;
			case D_STRUCT_INDICE_ETIQUETAS:
				paquete = paquetizarStruct_indiceEtiquetas((t_struct_indice_etiquetas*) estructuraOrigen);
				break;
			case D_STRUCT_OBTENERCOMPARTIDA:
				paquete = paquetizarStruct_obtenerCompartida((t_struct_string*) estructuraOrigen);
				break;
			case D_STRUCT_WAIT:
				paquete = paquetizarStruct_wait((t_struct_semaforo*) estructuraOrigen);
				break;
			case D_STRUCT_SIGNALSEMAFORO:
				paquete = paquetizarStruct_signalSemaforo((t_struct_semaforo*) estructuraOrigen);
				break;
			case D_STRUCT_IO:
				paquete = paquetizarStruct_io((t_struct_io*) estructuraOrigen);
				break;
			case D_STRUCT_VARIABLES:
				paquete = paquetizarStruct_variables((t_struct_string*) estructuraOrigen);
				break;
			case D_STRUCT_SF:
				paquete = paquetizarStruct_SF((t_struct_numero*) estructuraOrigen);
				break;
			case D_STRUCT_PROGFIN:
				paquete = paquetizarStruct_PROGFIN((t_struct_numero*) estructuraOrigen);
				break;
		}


	return paquete;
}

/*
 * Nombre: paquetizarStruct_nombreMensaje/1
 * EJEMPLO PAQUETIZAR ESTRUCTURA DINAMICA
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(0, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_nombreMensaje(t_struct_nombreMensaje * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre) + strlen(estructuraOrigen->mensaje) + 2;

	char * data = crearDataConHeader(D_STRUCT_NOMBREMENSAJE, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header), tamanoDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->nombre, tamanoDato = strlen(estructuraOrigen->nombre)+1);		//copio a data el nombre.

	tamanoTotal += tamanoDato;

	memcpy(data + tamanoTotal, estructuraOrigen->mensaje, tamanoDato = strlen(estructuraOrigen->mensaje)+1);		//copio a data el mensaje.

	tamanoTotal += tamanoDato;

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_numero/1
 * EJEMPLO PAQUETIZAR ESTRUCTURA ESTATICA
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(1, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_numero(t_struct_numero * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_NUMERO, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_numero));		//copio a data el numero.

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_char/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(2, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_char(t_struct_char * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_CHAR, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), &estructuraOrigen->letra, sizeof(char));		//copio a data el char.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_string/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(3, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_string(t_struct_string * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->string) + 1;

	char * data = crearDataConHeader(D_STRUCT_STRING, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->string, strlen(estructuraOrigen->string)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_string/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(18, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_variables(t_struct_string * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->string) + 1;

	char * data = crearDataConHeader(D_STRUCT_VARIABLES, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->string, strlen(estructuraOrigen->string)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_wait/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(15, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_wait(t_struct_semaforo * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre_semaforo) + 1;

	char * data = crearDataConHeader(D_STRUCT_WAIT, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->nombre_semaforo, strlen(estructuraOrigen->nombre_semaforo)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_signal/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(16, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_signalSemaforo(t_struct_semaforo * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre_semaforo) + 1;

	char * data = crearDataConHeader(D_STRUCT_SIGNALSEMAFORO, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->nombre_semaforo, strlen(estructuraOrigen->nombre_semaforo)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_obtenerCompartida/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(14, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_obtenerCompartida(t_struct_string * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->string) + 1;

	char * data = crearDataConHeader(D_STRUCT_OBTENERCOMPARTIDA, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->string, strlen(estructuraOrigen->string)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_signal/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(4, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_signal(t_struct_signal * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(t_struct_signal);

	char * data = crearDataConHeader(D_STRUCT_SIGNAL, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_signal));	//copio a data la estructura.

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_pcb/1
 * Argumentos:
 * 		- estructura de tipo pcb
 *
 * Devuelve:
 * 		paquete
 *
 * Funcion: crearDataConHeader(5, length)
 */
t_stream* paquetizarStruct_pcb(t_struct_pcb* estructuraOrigen){

	t_stream* paquete = malloc(sizeof(t_stream));

	paquete->length = sizeof(t_header) + sizeof(t_struct_pcb);

	char* data = crearDataConHeader(D_STRUCT_PCB, paquete->length);

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_pcb));

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_pidycodigo/1
 * Argumentos:
 * 		-estructura tipo pidycodigo
 *
 * Devuelve:
 *		paquete
 *
 * Funcion: crearDataConHeader(6,length)
 */

t_stream* paquetizarStruct_pidycodigo(t_struct_pidycodigo* estructuraOrigen){
	t_stream* paquete = malloc(sizeof(t_stream));

	paquete->length = sizeof(t_header) + sizeof(estructuraOrigen->codigo) + sizeof(estructuraOrigen->pid);

	char* data = crearDataConHeader(D_STRUCT_PIDYCODIGO, paquete->length);

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->pid, sizeof(t_pid));

	memcpy(data + tamanoTotal, estructuraOrigen->codigo, strlen(estructuraOrigen->codigo)+1);

	paquete->data = data;


	return paquete;
}


/*
 * Nombre: paquetizarStruct_push/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete
 *
 * Funcion: crearDataConHeader(8, length)
 */
t_stream * paquetizarStruct_push(t_struct_push * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));

	paquete->length = sizeof(t_header) + sizeof(uint32_t) + sizeof(int32_t);

	char * data = crearDataConHeader(D_STRUCT_PUSH, paquete->length);

	int tamanioTotal = sizeof(t_header);
	int tamanioDato = 0;

	memcpy(data + tamanioTotal, &estructuraOrigen->posicion,tamanioDato = sizeof(uint32_t));

	tamanioTotal += tamanioDato;

	memcpy(data + tamanioTotal, &estructuraOrigen->valor, sizeof(int32_t));

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_indiceEtiquetas/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete
 *
 * Funcion: crearDataConHeader(8, length)
 */
t_stream * paquetizarStruct_indiceEtiquetas(t_struct_indice_etiquetas * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));

	paquete->length = sizeof(t_header) + sizeof(uint32_t) + sizeof(uint32_t);

	char * data = crearDataConHeader(D_STRUCT_INDICE_ETIQUETAS, paquete->length);

	int tamanioTotal = sizeof(t_header);
	int tamanioDato = 0;

	memcpy(data + tamanioTotal, &estructuraOrigen->index_etiquetas ,tamanioDato = sizeof(uint32_t));

	tamanioTotal += tamanioDato;

	memcpy(data + tamanioTotal, &estructuraOrigen->etiquetas_size, sizeof(uint32_t));

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_pop/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete
 *
 * Funcion: crearDataConHeader(9, length)
 */
t_stream * paquetizarStruct_pop(t_struct_pop * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_POP, paquete->length);

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_pop));

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_asignarCompartida/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(11, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_asignarCompartida(t_struct_asignar_compartida * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre) + 1 + sizeof(int32_t);

	char * data = crearDataConHeader(D_STRUCT_ASIGNARCOMPARTIDA, paquete->length); //creo el data.

	int tamanoTotal = sizeof(t_header);
	int tamanioDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->nombre, tamanioDato = strlen(estructuraOrigen->nombre)+1);		//copio a data el nombre de la variable.

	tamanoTotal += tamanioDato;

	memcpy(data + tamanoTotal, &estructuraOrigen->valor, sizeof(int32_t));  //copio a data el valor de la variable.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_io/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(11, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_io(t_struct_io * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->dispositivo) + 1 + sizeof(int32_t);

	char * data = crearDataConHeader(D_STRUCT_IO, paquete->length); //creo el data.

	int tamanoTotal = sizeof(t_header);
	int tamanioDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->dispositivo, tamanioDato = strlen(estructuraOrigen->dispositivo)+1);		//copio a data el nombre de la variable.

	tamanoTotal += tamanioDato;

	memcpy(data + tamanoTotal, &estructuraOrigen->tiempo, sizeof(int32_t));  //copio a data el valor de la variable.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_instruccion/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete .
 *
 * Funcion: crearDataConHeader(12, length)
 */
t_stream * paquetizarStruct_instruccion(t_struct_instruccion * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));	//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(t_struct_instruccion);

	char * data = crearDataConHeader(D_STRUCT_INSTRUCCION, paquete->length); 	//creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_instruccion));		//copio a data el numero.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_SF
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */
t_stream * paquetizarStruct_SF(t_struct_numero * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_SF, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_numero));		//copio a data el numero.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_PROGFIN
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */

t_stream * paquetizarStruct_PROGFIN(t_struct_numero * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_PROGFIN, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_numero));		//copio a data el numero.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: crearDataConHeader/2
 * Argumentos:
 * 		- tipoEstructura
 * 		- length
 *
 * Devuelve:
 * 		char * data (con los primeros bytes ocupados con el tipoEstructura, y el length de la misma.
 *
 * Funcion: crearHeader -> genera y devuelve la estructura del header, con los datos cargados.
 */
char * crearDataConHeader(uint8_t tipoEstructura, int length){
	char * data = malloc(length);

	uint16_t lengthDatos = length - sizeof(t_header);

	t_header header = crearHeader(tipoEstructura, lengthDatos); //creo el header

	int tamanoTotal = 0, tamanoDato = 0;

	memcpy(data, &header.tipoEstructura, tamanoDato = sizeof(uint8_t)); //copio el tipoEstructura del header a data
	tamanoTotal = tamanoDato;
	memcpy(data + tamanoTotal, &header.length, tamanoDato = sizeof(uint16_t)); //copio el length del header a data

	return data;
}


/*
 * Nombre: crearHeader/2
 * Argumentos:
 * 		- tipoEstructura
 * 		- lengthDatos
 *
 * Devuelve:
 * 		genera y devuelve la estructura del t_header, con los datos cargados.
 *
 */
t_header crearHeader(uint8_t tipoEstructura, uint16_t lengthDatos){
	t_header header;
	header.tipoEstructura = tipoEstructura;
	header.length = lengthDatos;
	return header;
}

/******************** DESPAQUETIZAR *************************/

/*
 * Nombre: despaquetizar/3
 * Argumentos: (estos 3 datos serían el data del paquete:).
 * 		- tipoEstructura (un numero que, bajo criterio nuestro, representará la estructura que estoy recibiendo para despaquetizar)
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		estructuraDestino (estructura donde se guardó el paquete).
 * 		--> Recordar hacer free de la estructuraDestino luego de utilizar esta función.
 *
 * Funcion: Según el tipo de estructura, va a distintas funciones que las despaquetizan.
 */
void * despaquetizar(uint8_t tipoEstructura, char * dataPaquete, uint16_t length){

	void * estructuraDestino;

	switch (tipoEstructura){
			case D_STRUCT_NOMBREMENSAJE:
				estructuraDestino = despaquetizarStruct_nombreMensaje(dataPaquete, length);
				break;
			case D_STRUCT_NUMERO:
				estructuraDestino = despaquetizarStruct_numero(dataPaquete, length);
				break;
			case D_STRUCT_CHAR:
				estructuraDestino = despaquetizarStruct_char(dataPaquete,length);
				break;
			case D_STRUCT_STRING:
				estructuraDestino = despaquetizarStruct_string(dataPaquete,length);
				break;
			case D_STRUCT_SIGNAL:
				estructuraDestino = despaquetizarStruct_signal(dataPaquete, length);
				break;
			case D_STRUCT_PCB:
				estructuraDestino = despaquetizarStruct_pcb(dataPaquete, length);
				break;
			case D_STRUCT_PIDYCODIGO:
				estructuraDestino = despaquetizarStruct_pidycodigo(dataPaquete, length);
				break;
			case D_STRUCT_PUSH:
				estructuraDestino = despaquetizarStruct_push(dataPaquete, length);
				break;
			case D_STRUCT_POP:
				estructuraDestino = despaquetizarStruct_pop(dataPaquete, length);
				break;
			case D_STRUCT_ASIGNARCOMPARTIDA:
				estructuraDestino = despaquetizarStruct_asignarCompartida(dataPaquete, length);
				break;
			case D_STRUCT_INSTRUCCION:
				estructuraDestino = despaquetizarStruct_instruccion(dataPaquete, length);
				break;
			case D_STRUCT_INDICE_ETIQUETAS:
				estructuraDestino = despaquetizarStruct_indiceEtiquetas(dataPaquete, length);
				break;
			case D_STRUCT_SIGNALSEMAFORO:
				estructuraDestino = despaquetizarStruct_signalSemaforo(dataPaquete, length);
				break;
			case D_STRUCT_WAIT:
				estructuraDestino = despaquetizarStruct_wait(dataPaquete, length);
				break;
			case D_STRUCT_IO:
				estructuraDestino = despaquetizarStruct_io(dataPaquete, length);
				break;
			case D_STRUCT_VARIABLES:
				estructuraDestino = despaquetizarStruct_variables(dataPaquete, length);
				break;
			case D_STRUCT_SF:
				estructuraDestino = despaquetizarStruct_SF(dataPaquete, length);
				break;
			case D_STRUCT_PROGFIN:
				estructuraDestino = despaquetizarStruct_PROGFIN(dataPaquete, length);
				break;
			case D_STRUCT_MO:
				estructuraDestino = despaquetizarStruct_MO(dataPaquete, length);
				break;
		}

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_nombreMensaje/2
 * * EJEMPLO DESPAQUETIZAR ESTRUCTURA DINAMICA
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_NOMBREMENSAJE.
 *
 * Funcion: Según el tipo de estructura, va a distintas funciones que las despaquetizan.
 */
t_struct_nombreMensaje * despaquetizarStruct_nombreMensaje(char * dataPaquete, uint16_t length){
	t_struct_nombreMensaje * estructuraDestino = malloc(sizeof(t_struct_nombreMensaje));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->nombre = malloc(tamanoDato);
	memcpy(estructuraDestino->nombre, dataPaquete + tamanoTotal, tamanoDato); //copio el nombre a la estructura

	tamanoTotal += tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); //incremento tamanoDato, hasta el tamaño del mensaje.

	estructuraDestino->mensaje = malloc(tamanoDato);
	memcpy(estructuraDestino->mensaje, dataPaquete + tamanoTotal, tamanoDato); //copio el mensaje a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_numero/2
 * EJEMPLO DESPAQUETIZAR ESTRUCTURA ESTATICA
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_NUMERO.
 *
 * Funcion: Según el tipo de estructura, va a distintas funciones que las despaquetizan.
 */

t_struct_numero * despaquetizarStruct_numero(char * dataPaquete, uint16_t length){
	t_struct_numero * estructuraDestino = malloc(sizeof(t_struct_numero));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_char/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_CHAR.
 *
 */

t_struct_char * despaquetizarStruct_char(char * dataPaquete, uint16_t length){
	t_struct_char * estructuraDestino = malloc(sizeof(t_struct_char));

	memcpy(&estructuraDestino->letra, dataPaquete, sizeof(char)); //copio la letra a la estructura

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_string/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_STRING.
 *
 */
t_struct_string * despaquetizarStruct_string(char * dataPaquete, uint16_t length){
	t_struct_string * estructuraDestino = malloc(sizeof(t_struct_string));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->string = malloc(tamanoDato);
	memcpy(estructuraDestino->string, dataPaquete + tamanoTotal, tamanoDato); //copio el string a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_variables/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_VARIABLES.
 *
 */
t_struct_string * despaquetizarStruct_variables(char * dataPaquete, uint16_t length){
	t_struct_string * estructuraDestino = malloc(sizeof(t_struct_string));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->string = malloc(tamanoDato);
	memcpy(estructuraDestino->string, dataPaquete + tamanoTotal, tamanoDato); //copio el string a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_obtenerCompartida/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_OBTENERCOMPARTIDA.
 *
 */
t_struct_string * despaquetizarStruct_obtenerCompartida(char * dataPaquete, uint16_t length){
	return despaquetizarStruct_string(dataPaquete, length);
}

/*
 * Nombre: despaquetizarStruct_signal/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_SIGNALSEMAFORO.
 *
 */
t_struct_semaforo * despaquetizarStruct_signalSemaforo(char * dataPaquete, uint16_t length){
	t_struct_semaforo * estructuraDestino = malloc(sizeof(t_struct_semaforo));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->nombre_semaforo = malloc(tamanoDato);
	memcpy(estructuraDestino->nombre_semaforo, dataPaquete + tamanoTotal, tamanoDato); //copio el string a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_wait/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_WAIT.
 *
 */
t_struct_semaforo * despaquetizarStruct_wait(char * dataPaquete, uint16_t length){
	return despaquetizarStruct_signalSemaforo(dataPaquete, length);
}

t_struct_signal * despaquetizarStruct_signal(char * dataPaquete, uint16_t length){
	t_struct_signal * estructuraDestino = malloc(sizeof(t_struct_signal));

	memcpy(estructuraDestino, dataPaquete, sizeof(t_struct_signal)); //copio el data del paquete a la estructura

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_pcb/1
 * Argumentos:
 * 		-paquete
 * 		-length
 *
 * Devuelve:
 *		estructura de tipo D_STRUCT_PCB
 *
 * Funcion:
 * 		recibe el paquete y lo despaquetiza
 */
t_struct_pcb* despaquetizarStruct_pcb(char* dataPaquete, uint16_t lenght){
	t_struct_pcb* estructuraDestino = malloc(sizeof(t_struct_pcb));

	memcpy(estructuraDestino, dataPaquete, sizeof(t_struct_pcb));

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_pidycodigo/2
 * Argumentos:
 * 		-paquete
 * 		-length
 *
 * Devuelve:
 *		estructura de tipo D_STRUCT_PIDYCODIGO
 *
 * Funcion:
 *
 */
t_struct_pidycodigo* despaquetizarStruct_pidycodigo(char* dataPaquete, uint16_t length){
	t_struct_pidycodigo* estructuraDestino;


	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;
	estructuraDestino->pid = malloc(sizeof(int));
	memcpy(&estructuraDestino->pid, dataPaquete, sizeof(int));

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != NULL;tamanoDato++);

	estructuraDestino->codigo= malloc(tamanoDato);
	memcpy(estructuraDestino->codigo, dataPaquete + tamanoTotal, tamanoDato);

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_push/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_PUSH.
 *
 */
t_struct_push * despaquetizarStruct_push(char * dataPaquete, uint16_t length){//
	t_struct_push * estructuraDestino = malloc(sizeof(t_struct_push));

	memcpy(&estructuraDestino->posicion, dataPaquete, sizeof(uint32_t)); //copio la posicion del paquete a la estructura.

	memcpy(&estructuraDestino->valor, dataPaquete+ sizeof(uint32_t), sizeof(int32_t)); //copio el valor del paquete a la estructura.

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_indiceEtiquetas/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_INDICE_ETIQUETAS.
 *
 */
t_struct_indice_etiquetas * despaquetizarStruct_indiceEtiquetas(char * dataPaquete, uint16_t length){//
	t_struct_indice_etiquetas * estructuraDestino = malloc(sizeof(t_struct_indice_etiquetas));

	memcpy(&estructuraDestino->index_etiquetas, dataPaquete, sizeof(uint32_t)); //copio la posicion del paquete a la estructura.

	memcpy(&estructuraDestino->etiquetas_size, dataPaquete+ sizeof(uint32_t), sizeof(uint32_t)); //copio el valor del paquete a la estructura.

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_pop/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_POP.
 *
 */
t_struct_pop * despaquetizarStruct_pop(char * dataPaquete, uint16_t length){//
	t_struct_pop * estructuraDestino = malloc(sizeof(t_struct_pop));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_asignarCompartida/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_ASIGNARCOMPARTIDA.
 *
 */
t_struct_asignar_compartida * despaquetizarStruct_asignarCompartida(char * dataPaquete, uint16_t length){
	t_struct_asignar_compartida * estructuraDestino = malloc(sizeof(t_struct_asignar_compartida));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->nombre = malloc(tamanoDato);
	memcpy(estructuraDestino->nombre, dataPaquete + tamanoTotal, tamanoDato); //copio el nombre a la estructura

	tamanoTotal += tamanoDato;

	memcpy(&estructuraDestino->valor, dataPaquete + tamanoTotal, sizeof(int32_t));

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_asignarCompartida/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_ASIGNARCOMPARTIDA.
 *
 */
t_struct_io * despaquetizarStruct_io(char * dataPaquete, uint16_t length){
	t_struct_io * estructuraDestino = malloc(sizeof(t_struct_io));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->dispositivo = malloc(tamanoDato);
	memcpy(estructuraDestino->dispositivo, dataPaquete + tamanoTotal, tamanoDato); //copio el nombre a la estructura

	tamanoTotal += tamanoDato;

	memcpy(&estructuraDestino->tiempo, dataPaquete + tamanoTotal, sizeof(int32_t));

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_instruccion/2
 * EJEMPLO DESPAQUETIZAR ESTRUCTURA ESTATICA
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_INSTRUCCION.
 *
 */

t_struct_instruccion * despaquetizarStruct_instruccion(char * dataPaquete, uint16_t length){
	t_struct_instruccion * estructuraDestino = malloc(sizeof(t_struct_instruccion));

	memcpy(&estructuraDestino->inst, dataPaquete, sizeof(t_struct_instruccion)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_SF
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */
t_struct_numero * despaquetizarStruct_SF(char * dataPaquete, uint16_t length){
	t_struct_numero * estructuraDestino = malloc(sizeof(t_struct_numero));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_PROGFIN
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */
t_struct_numero * despaquetizarStruct_PROGFIN(char * dataPaquete, uint16_t length){
	t_struct_numero * estructuraDestino = malloc(sizeof(t_struct_numero));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int)); //copio el data del paquete a la estructura.

	return estructuraDestino;
}
/*
 * Nombre:
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */
t_struct_numero * despaquetizarStruct_MO(char * dataPaquete, uint16_t length){
	t_struct_numero * estructuraDestino = malloc(sizeof(t_struct_numero));

	memcpy(estructuraDestino, dataPaquete, sizeof(unsigned int));

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarHeader/1
 * Argumentos:
 * 		- char * header
 *
 * Devuelve:
 * 		una estructura de tipo t_header.
 *
 */
t_header despaquetizarHeader(char * header){
	t_header estructuraHeader;

	int tamanoTotal = 0, tamanoDato = 0;
	memcpy(&estructuraHeader.tipoEstructura, header + tamanoTotal, tamanoDato = sizeof(uint8_t));
	tamanoTotal = tamanoDato;
	memcpy(&estructuraHeader.length, header + tamanoTotal, tamanoDato = sizeof(uint16_t));

	return estructuraHeader;
}
