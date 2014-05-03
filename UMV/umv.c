/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "libs/funciones_UMV.h"
#include "libs/estructuras_umv.h"

int main (int argc, char **argv){
	//Acceder a archConfig y obtener datos
	crearMP(/*configuracionUMV*/);
	char comando[32];
	puts("Ingrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	gets(comando);
	while(estaEnDicOP(comando)== 0){
		puts("Operacion erronea, escriba la operacion de nuevo");
		gets(comando);
	}
	while(strcmp(comando, "exit") != 0){
			if(strcmp(comando, "operacion") == 0){
				char tipoOperacion[32];
				puts("Desea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?\n");
				gets(tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0){
						puts("Tipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						gets(tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					//solicitarPosicionDeMemoria();
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					//escribirBuffer();
				}
				if(strcmp(tipoOperacion, "crear") == 0){
					//crearSegmentoPrograma();
				 }
				if(strcmp(tipoOperacion, "destruir") == 0){
					//destruirSegmentoPrograma();
				 }
			}

		else { if (strcmp(comando, "retardo") == 0){
				//retardarRespuesta();
			       }
			   if (strcmp(comando, "algoritmo") == 0){
				//cambiarAlgoritmo();
			   }
			   if (strcmp(comando, "compactacion") == 0){
				//compactar();
							}
			   if (strcmp(comando,"dump") ==0){
				//generarReporte();
							}
			}
		puts("Escriba la siguiente operacion\n");
		gets(comando);
		while(estaEnDicOP(comando)== 0){
				puts("Operacion erronea, escriba la operacion de nuevo");
				gets(comando);
			}
		}

	return 0;
}
