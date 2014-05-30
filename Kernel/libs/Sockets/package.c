/*
 * package.c
 *
 *  Created on: 24/04/2013
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
	t_stream * paquete;

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
			case D_STRUCT_NOMBRENIVEL:
				paquete = paquetizarStruct_nombreNivel((t_struct_nombreNivel *) estructuraOrigen);
				break;
			case D_STRUCT_PERSONAJESINTERBLOQUEADOS:
				paquete = paquetizarStruct_personajesInterbloqueados((t_struct_personajesInterbloqueados *) estructuraOrigen);
				break;
			case D_STRUCT_POSXPOSY:
				paquete = paquetizarStruct_posxposy((t_struct_posxposy *) estructuraOrigen);
				break;
			case D_STRUCT_SIGNAL:
				paquete = paquetizarStruct_signal((t_struct_signal *) estructuraOrigen);
				break;
			case D_STRUCT_DIRECCIONES:
				paquete = paquetizarStruct_direcciones((t_struct_direcciones *) estructuraOrigen);
				break;
			case D_STRUCT_PEDIRPOSICIONRECURSO:
				paquete = paquetizarStruct_charPedirPosicionRecurso((t_struct_pedirPosicionRecurso *) estructuraOrigen);
				break;
			case D_STRUCT_PEDIRRECURSO:
				paquete = paquetizarStruct_charPedirRecurso((t_struct_pedirRecurso *) estructuraOrigen);
				break;
			case D_STRUCT_DATOSNIVEL:
				paquete = paquetizarStruct_datosNivel((t_struct_datosNivel *) estructuraOrigen);
				break;
			case D_STRUCT_SIMBOLOPERSONAJE:
				paquete = paquetizarStruct_simboloPersonaje((t_struct_simboloPersonaje *) estructuraOrigen);
				break;
			case D_STRUCT_RECURSOBLOQUEANTE:
				paquete = paquetizarStruct_charRecursoBloqueante((t_struct_recursoBloqueante *) estructuraOrigen);
				break;
			case D_STRUCT_RECURSOSASIGNADOSYSOBRANTES:
				paquete = paquetizarStruct_recursosAsignadosYSobrantes((t_struct_recursosAsignadosYSobrantes *) estructuraOrigen);
				break;
			case D_STRUCT_RECURSOSLIBERADOS:
				paquete = paquetizarStruct_recursosLiberados((t_struct_recursosLiberados *) estructuraOrigen);
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
 * Nombre: paquetizarStruct_charPedirPosicionRecurso/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(2, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_charPedirPosicionRecurso(t_struct_pedirPosicionRecurso * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_PEDIRPOSICIONRECURSO, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), &estructuraOrigen->letra, sizeof(char));		//copio a data el char.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_charPedirRecurso/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(2, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_charPedirRecurso(t_struct_pedirRecurso * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_PEDIRRECURSO, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), &estructuraOrigen->letra, sizeof(char));		//copio a data el char.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_charRecursoBloqueante/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(2, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_charRecursoBloqueante(t_struct_recursoBloqueante * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_RECURSOBLOQUEANTE, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), &estructuraOrigen->recurso, sizeof(char));		//copio a data el char.

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_simboloPersonaje/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(2, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_simboloPersonaje(t_struct_simboloPersonaje * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(unsigned int);

	char * data = crearDataConHeader(D_STRUCT_SIMBOLOPERSONAJE, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), &estructuraOrigen->simbolo, sizeof(char));		//copio a data el char.

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
 * Funcion: crearDataConHeader(3, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_recursosLiberados(t_struct_recursosLiberados * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->recursosLiberados) + 1;

	char * data = crearDataConHeader(D_STRUCT_RECURSOSLIBERADOS, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->recursosLiberados, strlen(estructuraOrigen->recursosLiberados)+1);		//copio a data el string.

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_nombreNivel/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(3, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_nombreNivel(t_struct_nombreNivel * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre) + 1;

	char * data = crearDataConHeader(D_STRUCT_NOMBRENIVEL, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->nombre, strlen(estructuraOrigen->nombre)+1);		//copio a data el string del nombre.

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_personajesInterbloqueados/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(3, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_personajesInterbloqueados(t_struct_personajesInterbloqueados * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->simbolosPersonajes) + 1;

	char * data = crearDataConHeader(D_STRUCT_PERSONAJESINTERBLOQUEADOS, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header);

	memcpy(data + tamanoTotal, estructuraOrigen->simbolosPersonajes, strlen(estructuraOrigen->simbolosPersonajes)+1);		//copio a data el string de los personajes interbloqueados.

	paquete->data = data;

	return paquete;
}


/*
 * Nombre: paquetizarStruct_posxposy/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(4, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_posxposy(t_struct_posxposy * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + sizeof(t_struct_posxposy);

	char * data = crearDataConHeader(D_STRUCT_POSXPOSY, paquete->length); //creo el data

	memcpy(data + sizeof(t_header), estructuraOrigen, sizeof(t_struct_posxposy));	//copio a data la estructura.

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
 * Funcion: crearDataConHeader(5, length) -> reserva la memoria para el data del paquete, y le agrega el header.
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
 * Nombre: paquetizarStruct_direcciones/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(0, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_direcciones(t_struct_direcciones * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->direccionNivel) + strlen(estructuraOrigen->direccionPlanificador) + 2;

	char * data = crearDataConHeader(D_STRUCT_DIRECCIONES, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header), tamanoDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->direccionNivel, tamanoDato = strlen(estructuraOrigen->direccionNivel)+1);		//copio a data la direccion del nivel.

	tamanoTotal += tamanoDato;

	memcpy(data + tamanoTotal, estructuraOrigen->direccionPlanificador, tamanoDato = strlen(estructuraOrigen->direccionPlanificador)+1);		//copio a data la direccion del planificador.

	tamanoTotal += tamanoDato;

	paquete->data = data;

	return paquete;
}

/*
 * Nombre: paquetizarStruct_datosNivel/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(9, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_datosNivel(t_struct_datosNivel * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->nombre) + 1 + strlen(estructuraOrigen->direccion) + 1 + strlen(estructuraOrigen->recursos)+1;

	char * data = crearDataConHeader(D_STRUCT_DATOSNIVEL, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header), tamanoDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->nombre, tamanoDato = strlen(estructuraOrigen->nombre)+1);		//copio a data el nombre del nivel.

	tamanoTotal += tamanoDato;

	memcpy(data + tamanoTotal, estructuraOrigen->direccion, tamanoDato = strlen(estructuraOrigen->direccion)+1);		//copio a data la direccion del nivel.

	tamanoTotal += tamanoDato;

	memcpy(data + tamanoTotal, estructuraOrigen->recursos, tamanoDato = strlen(estructuraOrigen->recursos)+1);		//copio a data los recursos (char *).

	tamanoTotal += tamanoDato;
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
 * Nombre: paquetizarStruct_recursosAsignadosYSobrantes/1
 * Argumentos:
 * 		- estructuraOrigen
 *
 * Devuelve:
 * 		paquete (buffer con la estructura paquetizada).
 *
 * Funcion: crearDataConHeader(15, length) -> reserva la memoria para el data del paquete, y le agrega el header.
 */
t_stream * paquetizarStruct_recursosAsignadosYSobrantes(t_struct_recursosAsignadosYSobrantes * estructuraOrigen){

	t_stream * paquete = malloc(sizeof(t_stream));		//creo el paquete

	paquete->length = sizeof(t_header) + strlen(estructuraOrigen->recursosSobrantes) + strlen(estructuraOrigen->personajeRecurso) + 2;

	char * data = crearDataConHeader(D_STRUCT_RECURSOSASIGNADOSYSOBRANTES, paquete->length); //creo el data

	int tamanoTotal = sizeof(t_header), tamanoDato = 0;

	memcpy(data + tamanoTotal, estructuraOrigen->recursosSobrantes, tamanoDato = strlen(estructuraOrigen->recursosSobrantes)+1);		//copio a data el nombre.

	tamanoTotal += tamanoDato;

	memcpy(data + tamanoTotal, estructuraOrigen->personajeRecurso, tamanoDato = strlen(estructuraOrigen->personajeRecurso)+1);		//copio a data el mensaje.

	tamanoTotal += tamanoDato;

	paquete->data = data;

	return paquete;
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
			case D_STRUCT_STRING:
			case D_STRUCT_RECURSOSLIBERADOS:
				estructuraDestino = despaquetizarStruct_string(dataPaquete, length);
				break;
			case D_STRUCT_NOMBRENIVEL:
				estructuraDestino = despaquetizarStruct_nombreNivel(dataPaquete, length);
				break;
			case D_STRUCT_PERSONAJESINTERBLOQUEADOS:
				estructuraDestino = despaquetizarStruct_personajesInterbloqueados(dataPaquete, length);
				break;
			case D_STRUCT_POSXPOSY:
				estructuraDestino = despaquetizarStruct_posxposy(dataPaquete, length);
				break;
			case D_STRUCT_SIGNAL:
				estructuraDestino = despaquetizarStruct_signal(dataPaquete, length);
				break;
			case D_STRUCT_DIRECCIONES:
				estructuraDestino = despaquetizarStruct_direcciones(dataPaquete, length);
				break;
			case D_STRUCT_CHAR:
			case D_STRUCT_PEDIRRECURSO:
			case D_STRUCT_PEDIRPOSICIONRECURSO:
			case D_STRUCT_SIMBOLOPERSONAJE:
			case D_STRUCT_RECURSOBLOQUEANTE:
				estructuraDestino = despaquetizarStruct_char(dataPaquete, length);
				break;
			case D_STRUCT_DATOSNIVEL:
				estructuraDestino = despaquetizarStruct_datosNivel(dataPaquete, length);
				break;
			case D_STRUCT_RECURSOSASIGNADOSYSOBRANTES:
				estructuraDestino = despaquetizarStruct_recursosAsignadosYSobrantes(dataPaquete, length);
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
 * Nombre: despaquetizarStruct_posxposy/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_POSXPOSY.
 *
 */

t_struct_posxposy * despaquetizarStruct_posxposy(char * dataPaquete, uint16_t length){
	t_struct_posxposy * estructuraDestino = malloc(sizeof(t_struct_posxposy));

	memcpy(estructuraDestino, dataPaquete, sizeof(t_struct_posxposy)); //copio el data del paquete a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_posxposy/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_POSXPOSY.
 *
 */

t_struct_signal * despaquetizarStruct_signal(char * dataPaquete, uint16_t length){
	t_struct_signal * estructuraDestino = malloc(sizeof(t_struct_signal));

	memcpy(estructuraDestino, dataPaquete, sizeof(t_struct_signal)); //copio el data del paquete a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_direcciones/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_DIRECCIONES.
 *
 */
t_struct_direcciones * despaquetizarStruct_direcciones(char * dataPaquete, uint16_t length){
	t_struct_direcciones * estructuraDestino = malloc(sizeof(t_struct_direcciones));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->direccionNivel = malloc(tamanoDato);
	memcpy(estructuraDestino->direccionNivel, dataPaquete + tamanoTotal, tamanoDato); //copio la direccion del nivel a la estructura

	tamanoTotal += tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); //incremento tamanoDato, hasta el tamaño del mensaje.

	estructuraDestino->direccionPlanificador = malloc(tamanoDato);
	memcpy(estructuraDestino->direccionPlanificador, dataPaquete + tamanoTotal, tamanoDato); //copio la direccion del planificador a la estructura

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_datosNivel/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_DATOSNIVEL.
 *
 */
t_struct_datosNivel * despaquetizarStruct_datosNivel(char * dataPaquete, uint16_t length){
	t_struct_datosNivel * estructuraDestino = malloc(sizeof(t_struct_datosNivel));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->nombre = malloc(tamanoDato);
	memcpy(estructuraDestino->nombre, dataPaquete + tamanoTotal, tamanoDato); //copio el nombre del nivel a la estructura

	tamanoTotal += tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño de la direccion.

	estructuraDestino->direccion = malloc(tamanoDato);
	memcpy(estructuraDestino->direccion, dataPaquete + tamanoTotal, tamanoDato); //copio el nombre de la direccion del nivel a la estructura

	tamanoTotal += tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); //incremento tamanoDato, hasta el tamaño de la cadena con los símbolos de los recursos.

	estructuraDestino->recursos = malloc(tamanoDato);
	memcpy(estructuraDestino->recursos, dataPaquete + tamanoTotal, tamanoDato); //copio la direccion del planificador a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_personajesInterbloqueados/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_PERSONAJESINTERBLOQUEADOS.
 *
 */
t_struct_personajesInterbloqueados * despaquetizarStruct_personajesInterbloqueados(char * dataPaquete, uint16_t length){
	t_struct_personajesInterbloqueados * estructuraDestino = malloc(sizeof(t_struct_personajesInterbloqueados));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del string.

	estructuraDestino->simbolosPersonajes = malloc(tamanoDato);
	memcpy(estructuraDestino->simbolosPersonajes, dataPaquete + tamanoTotal, tamanoDato); //copio el string a la estructura

	return estructuraDestino;
}


/*
 * Nombre: despaquetizarStruct_nombreNivel/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_NOMBRENIVEL.
 *
 */
t_struct_nombreNivel * despaquetizarStruct_nombreNivel(char * dataPaquete, uint16_t length){
	t_struct_nombreNivel * estructuraDestino = malloc(sizeof(t_struct_nombreNivel));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->nombre = malloc(tamanoDato);
	memcpy(estructuraDestino->nombre, dataPaquete + tamanoTotal, tamanoDato); //copio el string a la estructura

	return estructuraDestino;
}

/*
 * Nombre: despaquetizarStruct_recursosAsignadosYSobrantes/2
 * Argumentos:
 * 		- char * dataPaquete
 * 		- length
 *
 * Devuelve:
 * 		una estructura de tipo D_STRUCT_RECURSOSASIGNADOSYSOBRANTES.
 *
 */
t_struct_recursosAsignadosYSobrantes * despaquetizarStruct_recursosAsignadosYSobrantes(char * dataPaquete, uint16_t length){
	t_struct_recursosAsignadosYSobrantes * estructuraDestino = malloc(sizeof(t_struct_recursosAsignadosYSobrantes));

	int tamanoTotal = 0, tamanoDato = 0;

	tamanoTotal = tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); 	//incremento tamanoDato, hasta el tamaño del nombre.

	estructuraDestino->recursosSobrantes = malloc(tamanoDato);
	memcpy(estructuraDestino->recursosSobrantes, dataPaquete + tamanoTotal, tamanoDato); //copio el string de recursos sobrantes, a la estructura

	tamanoTotal += tamanoDato;

	for(tamanoDato = 1; (dataPaquete + tamanoTotal)[tamanoDato -1] != '\0';tamanoDato++); //incremento tamanoDato, hasta el tamaño del mensaje.

	estructuraDestino->personajeRecurso = malloc(tamanoDato);
	memcpy(estructuraDestino->personajeRecurso, dataPaquete + tamanoTotal, tamanoDato); //copio el string de personajeRecurso a la estructura

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
