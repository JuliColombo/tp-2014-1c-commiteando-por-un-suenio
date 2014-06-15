/*
 * funciones_aux.c
 *
 *  Created on: 18/05/2014
 *      Author: utnso
 */

#include "funciones_aux.h"

/*
 * Nombre: vector_num/2
 * Argumentos:
 * 		- vector de strings
 * 		- vector de ids
 *
 * Devuelve:
 * 		- int* (puntero a un array de numeros)
 *
 * Funcion: convierte un array de numeros que estan en formato string a un array de int
 */

int* vector_num(char** vector_string_num, char** config_ids){
	int n,*vector;
	vector=malloc(sizeof(int)*cant_identificadores(config_ids));
	if(vector_string_num !=NULL){
		for(n=0;vector_string_num[n]!=NULL;n++){
			vector[n]=atoi(vector_string_num[n]);
		}
	}else{
		for(n=0;config_ids[n]!=NULL; n++){
			vector[n]=0;
		}
	}

	return vector;
}

/*
 * Nombre: cant_identificadores/1
 * Argumentos:
 * 		- array de string
 *
 * Devuelve:
 * 			int (cantidad de posiciones de un array de strings)
 *
 *
 * Funcion: -
 */
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

/*
 * Nombre: posicion_Variable_Global/1
 * Argumentos:
 * 			  - Variable global que está en el codigo AnsisOP
 * Devuelve:
 * 			int (-1 si hay error, la posicion en caso de que la encuentre)
 *
 * Funcion: es usada para buscar la posicion de la variable global (si es que existe) dentro del array de variables globales
 *
 *
*/
int posicion_Variable_Global(char* variable){
	int pos=0;
	for(pos=0;configuracion_kernel.var_globales[pos]!=NULL;pos++){
		if(strcmp(variable,configuracion_kernel.var_globales[pos])==0){
			return pos;
		}
	}
	return -1;
}


/************************* FUNCIONES PARA EL MANEJO DE EPOLL *************************/

void aceptarConexionEntrante(struct epoll_event event){

}



void* manejar_ConexionNueva_Programas(struct epoll_event event){
	int n,i,j;
	char* buffer;
	void** buff;
	for(n=0;fds_conectados[n]!=NULL;n++){
		if(n<60){
			i=fds_conectados[n]=socket_aceptarCliente(event.data.fd);
			j=socket_recibir(i,&buffer, &buff);
			if(j==1){
				printf("%d \n", j);
			}
		}else{
			perror("No se pueden conectar más programas");
		}

	}
}

