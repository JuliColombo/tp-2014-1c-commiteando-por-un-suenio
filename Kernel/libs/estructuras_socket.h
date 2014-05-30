/*
 * estructurasPackage.h
 *
 *  Created on: 11/05/2013
 *      Author: utnso
 */

#ifndef ESTRUCTURASPACKAGE_H_
#define ESTRUCTURASPACKAGE_H_

#include "list.h"

/* Defino los tipos de señales que se pueden mandar
 *
 * Sintaxis correcta para escribir una nueva señal:
 *
 * 	#define S_[Origen]_[Destino]_[Mensaje] numeroIncremental
 *
 * 	Donde:
 * 		[Origen] es quien manda el mensaje
 * 		[Destino] es quien recibe el mensaje
 * 		[Mensaje] lo que se le quiere mandar
 * 		numeroIncrementar un numero mas que la señal anterior
 */
#define S_ERROR 0	// Utilizada en socket_recibirSeñal, para decir que lo que se recibio no era una señal
#define S_Personaje_Nivel_MoverIzquierda 1
#define S_Personaje_Nivel_MoverDerecha 2
#define S_Personaje_Nivel_MoverArriba 3
#define S_Personaje_Nivel_MoverAbajo 4
#define S_Planificador_Personaje_HabilitarMover 5
#define S_Planificador_Personaje_Matate 6	//Esta la manda el orquestador, pero usando el fd que tiene guardado el planificador, entonces al personaje le va a parecer que lo dijo el planificador.
#define S_Personaje_Planificador_MeMovi 7
#define S_Personaje_Orquestador_EmpeceJuego 8
#define S_Personaje_Planificador_MeMoviYTomeRecurso 9
#define S_Personaje_Orquestador_TermineJuego 10
#define S_Personaje_Nivel_PedirPosicionRecurso 11
#define S_Nivel_Personaje_ConfirmaQueSeMovio 12
#define S_Personaje_Nivel_PedirRecurso 13
#define S_Nivel_Personaje_TeAsigneRecurso 14
#define S_Nivel_Personaje_NoTeAsigneRecurso 15
#define S_Personaje_Orquestador_DameDireccionDeNivel 16
#define S_Personaje_Nivel_Reiniciar 17
#define S_Personaje_Planificador_Reiniciar 18
#define S_Orquestador_Nivel_Finalizate 19

/*Defino las estructuras de los paquetes
 *
 * Sintaxis correcta para escribir un nuev mensaje:
 *
 * 	#define D_STRUCT_[Nombre Estructura]
 *
 * 	Donde:
 * 		[Nombre Estructura] es el nombre de la estructura en MAYUSCULA
 *
 * 	Nota:
 * 		No olvidarse de crear la estructura y su paquetizador y despaquetizador asociado!
 */

#define D_STRUCT_NOMBREMENSAJE 0
#define D_STRUCT_NUMERO 1
#define D_STRUCT_CHAR 2
#define D_STRUCT_STRING 3
#define D_STRUCT_POSXPOSY 4
#define D_STRUCT_SIGNAL 5
#define D_STRUCT_DIRECCIONES 6
#define D_STRUCT_PEDIRRECURSO 7
#define D_STRUCT_PEDIRPOSICIONRECURSO 8
#define D_STRUCT_DATOSNIVEL 9
#define D_STRUCT_SIMBOLOPERSONAJE 10
#define D_STRUCT_NOMBRENIVEL 11 //de tipo t_struct_string
#define D_STRUCT_PERSONAJESINTERBLOQUEADOS 12 //de tipo t_struct_string (de nivel a orque)
#define D_STRUCT_RECURSOSLIBERADOS 13 //de tipo t_struct_string
#define D_STRUCT_RECURSOBLOQUEANTE 14 //se lo manda el personaje al plani;
#define D_STRUCT_RECURSOSASIGNADOSYSOBRANTES 15 //dos strings

//Estructura auxiliar de stream
typedef struct {
	int length;
	char *data;
} t_stream;

// Header de stream
typedef struct {
	uint8_t tipoEstructura;
	uint16_t length;
} __attribute__ ((__packed__)) t_header;


/*
 *	TYPEDEF DE LAS ESTRUCTURAS A ENVIAR
 */

//Estructura tipo STRUCT_NOMBREMENSAJE(0)
typedef struct struct_nombreMensaje {
	char * nombre;
	char * mensaje;
} __attribute__ ((__packed__)) t_struct_nombreMensaje;

//Estructura tipo STRUCT_NUMERO(1)
typedef struct struct_numero {
	unsigned int numero;
} __attribute__ ((__packed__)) t_struct_numero;

//Estructura tipo STRUCT_CHAR(2)
typedef struct struct_char {
	char letra;
} __attribute__ ((__packed__)) t_struct_char;

//Estructura tipo STRUCT_STRING(3)
typedef struct struct_string {
	char * string;
} __attribute__ ((__packed__)) t_struct_string;

/*Estructura tipo STRUCT_POSXPOSY(4)
 * Va a enviar la posicion en x y en y (dos unsigned int) en la que se encuentra el personaje o el recurso.
 */
typedef struct struct_posxposy {
	unsigned int posX;
	unsigned int posY;
} __attribute__ ((__packed__)) t_struct_posxposy;


/*Estructura tipo STRUCT_SIGNAL(5)
 * se va a usar cuando quiera enviar una señal.
 * cada señal va a estar numerada. Entonces aca envio el nro de señal.
 */
typedef unsigned int t_signal;

typedef struct struct_signal {
	t_signal signal;
} __attribute__ ((__packed__)) t_struct_signal;


/*Estructura tipo STRUCT_DIRECCIONES(6)
 * se va a enviar dos string (char*) con la direccion ip y puerto de nivel y planificador.
 */
typedef struct {
	char * direccionNivel;
	char * direccionPlanificador;
}  __attribute__ ((__packed__)) t_struct_direcciones;

//Estructura tipo STRUCT_PEDIRRECURSO (7)
typedef struct struct_pedirRecurso {
	char letra;
} __attribute__ ((__packed__)) t_struct_pedirRecurso;


//Estructura tipo STRUCT_PEDIRPOSICIONRECURSO(8)
typedef struct struct_pedirPosicionRecurs {
	char letra;
} __attribute__ ((__packed__)) t_struct_pedirPosicionRecurso;


/*Estructura tipo STRUCT_DATOSNIVEL(9)
 * Se usa en orquestador, para conectar un nivel.
 */
typedef struct {
	char * nombre;
	char * direccion;
	char * recursos;
}  __attribute__ ((__packed__)) t_struct_datosNivel;

//Estructura tipo STRUCT_SIMBOLOPERSONAJE(10)
typedef struct struct_simboloPersonaje {
	char simbolo;
} __attribute__ ((__packed__)) t_struct_simboloPersonaje;


/*Estructura tipo STRUCT_NOMBRENIVEL(11)
 * La usa el personaje para mandarle al orquestador:
 * señal: "empecé juego", con nombre primer nivel
 * o señal: "me bloquee", con nombre nivel actual (en el que se bloqueó).
*/
typedef struct struct_nombreNivel { //personaje le manda esta estructura al orquestador, para que le devuelva la struct direcciones del mismo.
	char * nombre;
} __attribute__ ((__packed__)) t_struct_nombreNivel;

/*Estructura tipo D_STRUCT_PERSONAJESINTERBLOQUEADOS(12)
 * La usa el nivel para mandarle al orquestador:
 * la cadena de símbolos de los personajes que quedaron interbloqueados
*/
typedef struct struct_personajesInterbloqueados{
	char * simbolosPersonajes;
} __attribute__ ((__packed__)) t_struct_personajesInterbloqueados;

/*Estructura tipo D_STRUCT_RECURSOSLIBERADOS(13)
 * La usa el nivel para mandarle al orquestador:
 * la cadena de símbolos de los recursos liberados por un personaje
*/
typedef struct struct_recursosLiberados{
	char * recursosLiberados;
} __attribute__ ((__packed__)) t_struct_recursosLiberados;


/*Estructura tipo D_STRUCT_RECURSOBLOQUEANTE(14)
 * La usa el personaje para mandarle al planificador:
 * El recurso por el cual se bloqueo. Se infiere con esto
 * que el personaje esta bloqueado. No se manda signal antes si esta bloqueadoi o no
*/
typedef struct struct_recursoBloqueante {
	char recurso;
} __attribute__ ((__packed__)) t_struct_recursoBloqueante;

/*Estructura tipo D_STRUCT_RECURSOSASIGNADOSYSOBRANTES(15)
 * La usa el orquestador para mandarselo al nivel:
 * la cadena de símbolos de los recursos liberados, ej: "MMH"
 * y los recursos asignados y a quien se los asigno, ej: "@M%H"
*/
typedef struct struct_recursosAsignadosYSobrantes {
	char * recursosSobrantes;
	char * personajeRecurso;	//contiene char pesonaje seguido de char recurso, asi repetido por el string
} __attribute__ ((__packed__)) t_struct_recursosAsignadosYSobrantes;

#endif /* ESTRUCTURASPACKAGE_H_ */
