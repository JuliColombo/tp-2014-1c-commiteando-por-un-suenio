/*
 * funciones.c
 *
 *  Created on: 12/04/2014
 *      Author: utnso
 */
#include "funciones.h"

//No creo que vaya aca, pero voy poniendo una idea de PLP:

//Tenemos una estructura con programas que se conectan o la funcion va a tener una llamada cada vez que entra uno nuevo?? lo modélo pensando lo ultimo

/* int plp(programa){
//Faltaria tambien inicializar el segmento de Stack y el puntero de Contexto Actual(tienen que coincidir), y setear el contador de variables locales en 0
	int crearPcb(programa);
	//Cuando creamos el PCB hay que aprovechar la obtencion de los datos(codigo literal, indice de etiquetas,indice de funciones, indice de codigo y stack) para conseguir los tamaños de este ""malloc"" de umv
	int solicitarMemoria(codigo,indiceEtiquetas,indiceFunciones,indiceCodigoYStack);
	//Ni idea los tipos de esos parametros, no me da mas la nafta

	int agregarACola(programa);

	return 0;

}
*/


/*Aca intente hacer el crearPcb. La consigna dice "PLP creara PCB y usara la funcionalidad del parser, que
 * recibira todo el codigo del script y devolvera una estrucutra con la info del programa, que contiene:
 * primera instruc, indice etiquetas, indice de codigo etc"
 *

int crearPcb(int primeraInstruc,Pares indiceCod, int tamanioIndEti, int tamanioIndCod, int tamanioContext, int PC) {
	PCB nuevoPcb;
	nuevoPcb->ID = getpid();
	nuevoPcb->indiceEtiquetas = dictionary_create();
	nuevoPcb->indiceCodigo = indiceCod;
	nuevoPcb->TamanioContext = tamanioContext;
	nuevoPcb->ProgramCounter = PCB;
	//No se que hacer con CODE, STACK y CursorSTACK
	//Creo que aca podria venir el solicitarMemoria. No use el tamanioIndEti ni tamanioIndCod que son cosas que
	//me da el parser supuestamente, y podrian servir para solicitar la memoria. No se

	return 0;
	}
*/
