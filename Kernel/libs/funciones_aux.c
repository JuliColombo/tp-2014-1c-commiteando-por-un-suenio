/*
 * funciones_aux.c
 *
 *  Created on: 18/05/2014
 *      Author: utnso
 */

#include "funciones_aux.h"

int* vector_num(char** vector_string_num, char** config_ids){
	int n,*vector;
	vector=malloc(sizeof(int)*cant_identificadores(config_ids));
	for(n=0;vector_string_num[n]!=NULL;n++){
	vector[n]=atoi(vector_string_num[n]);
	}

	return vector;
}


int cant_identificadores(char** config_ids){
	int i,a=0;
	for(i=0;config_ids[i]!=NULL;i++){
	a+=1;
	}
return a;
}

void log_error_socket(void){
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
}

