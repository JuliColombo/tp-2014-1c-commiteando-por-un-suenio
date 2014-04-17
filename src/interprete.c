/*
 * interprete.c
 *
 *  Created on: 14/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

t_log* archLog;

int main (int argc, char **argv){
	//int i;

	off_t fileSize=0;
	//char* buffer=
	archLog=log_create();

   /*	for(i=0;i<argc;i++) {
   // imprime indicador de órdenes;
   // lee la línea de ordenes;
   // analiza la línea de ordenes (arg0,arg1,...,>,<,|,&,...);
   //prepara entorno según lo que aparece en línea de ordenes;
   //crea un proceso para ejecutar orden;

		if /*(estamos en el proceso hijo)*/ //{
	//		ejecuta la orden dada en arg0;

	/*	else // es el proceso padre
	//		if (línea ordenes no aparece el símbolo &)
	//		espera hasta que finalice el hijo;
		}*/
	return 0;
}


