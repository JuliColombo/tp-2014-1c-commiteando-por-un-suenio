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


int* crearMP(t_config_UMV configuracionUMV) {
	int tamanio = configuracionUMV.MemSize;
	int* MP;
	MP = malloc(tamanio);
	return MP;
}

//_Bool segmentationFault(uint32_t base,uint32_t offset){
//	if (MP[base+offset] == NULL) {
//	    puts("Segmentation Fault al intentar acceder a posicion %d", base+offset);
//		return true;
//	} else{
//		return false;
//	}
//}
//
//_Bool memoryOverload(uint32_t base,uint32_t offset, uint32_t longitud){
//	if (MP[base+offset+longitud] == NULL) {
//		    puts("Memory Overload al intentar escribir %d bytes en la posicion %d", longitud,base+offset);
//			return true;
//		} else{
//			return false;
//		}
//}

//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
_Bool solicitarMemoria(t_programa programa){
	return true;
}

//Operacion Basica de UMV 1, se toma una cantidad de bytes (longitud) desde la posicion de memoria dada(base+offset)?
void solicitarBytes(uint32_t base,uint32_t offset, uint32_t longitud){
//	if (validarSolicitud(base,offset,longitud)){
//		SOLICITAR LOS BYTES
//	} else {
//		puts("No se pudo realizar la asignacion");
//    }

}

//Operacion Basica de UMV 2, se envia una cantidad de bytes (en el buffer?) a la posicion dada(base+offset)
void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud/*,t_buffer buffer*/){

}

//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion
//_Bool validarSolicitud(uint32_t base,uint32_t offset, uint32_t longitud){
//	if(hayEspacioEnMemoria()){
//		return true;
//	} else{
//		puts("No alcanza el espacio en memoria:");
//		switch(base,offset,longitud){
//			case segmentationFault(base,offset):
//				return false;
//			case memoryOverload(base,offset,longitud):
//				return false;
//			default:
//				//puts("Excepcion Desconocida"); ???
//				return false;
//		}
//	}
//}

//Comandos de consola:
void operacion(/*t_proceso proceso,*/ uint32_t base,uint32_t offset, uint32_t longitud){
	//Solicitar una posicion de memoria o escribir un buffer por teclado en una posicion o crear segmentos de programa o destruirlos
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

void algoritmo(int* algor){//Cambiar entre Worst fit y First fit
	if(*algor==worstfit){
							*algor=firstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}
	else{
							*algor=worstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}

}

void compactar(){
	//Forzar compactacion (ver para checkpoint 3)
}

void dump(){
	//obtenerDatosDeMemoria() y mostrar (y,opcional, guardar en archivo)
}




