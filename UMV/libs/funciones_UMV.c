/*
 * UMV.c
 *
 *  Created on: 21/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "commons/config.h"
#include "commons/collections/dictionary.h"
#include "estructuras_umv.h"
#include "funciones_UMV.h"
#include <string.h>
//#include "FuncionesPLP.h"
//#include <Estructuras.h>
//Al iniciar, la UMV crea la MP de un tamaño configurable por el archivo de configuracion, que supuse era
//el tamaño maximo que puede tener el diccionario de ese archivo de configuracion

int estaEnDicOP(char palabra[]){
					char *dic_op[]={"operacion",
									  "dump",
									  "compactacion",
									  "retardo",
									  "algoritmo",
									  "exit"
					};
					int aux;
					for(aux=0;aux<6;aux++){
						if(strcmp(dic_op[aux], palabra) == 0){
							return 1;
							break;
						}

					}
					return 0;
}

int estaEnDicTOP(char palabra[]){
					char *dic_top[]={"solicitar",
							        "escribir",
							        "crear",
							        "destruir"
						};
					int aux;
					for(aux=0;aux<4;aux++){
						if((strcmp(dic_top[aux], palabra)) == 0){
							return 1;
							break;
						}
					}
					return 0;
}

// Ojo!! Correccion del enunciado dice que es un campo del archivo,si es asi hay que revisar esto
int* crearMP(t_config archConfig) {
	int tamanio = (archConfig.properties)->table_max_size;
	int* MP = malloc(tamanio);

	return MP;
}


//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
_Bool solicitarMemoria(t_programa programa){
	return true;
}

//Operacion Basica de UMV 1, se toma una cantidad de bytes (longitud) desde la posicion de memoria dada(base+offset)?
void solicitarBytes(uint32_t base,uint32_t offset, uint32_t longitud){

}

//Operacion Basica de UMV 2, se envia una cantidad de bytes (en el buffer?) a la posicion dada(base+offset)
void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud/*,t_buffer buffer*/){

}

//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion
_Bool validarSolicitud(uint32_t longitud){
	if(0/*MemoriaSuficiente*/){
	}
	return true;
}

//Comandos de consola:
void operacion(/*t_proceso proceso,*/ uint32_t base,uint32_t offset, uint32_t longitud){
	//Solicitar una posicion de memoria o escribir un buffer por teclado en una posicion
	/*if ( esValido(buffer) ){
	 * void enviarBytes(base,offset,longitud,buffer)
	 * }else{
	 *  solicitarBytes(base,offset,longitud)
	 *  } */
	printf(" "/*posicionDeMemoria*/);
	/*OPCIONAL: grabar en archivo*/
}

void retardo(int valorRetardoEnMilisegundos){ //Cantidad de ms que debe esperar UMV para responder una solicitud

}

void algoritmo(char tipo){//Cambiar entre Worst fit y First fit

//TODO compactar y dump !!

}
