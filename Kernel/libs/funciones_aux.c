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
	for(pos=0;configuracion_kernel.var_globales.identificador[pos]!=NULL;pos++){
		if(strcmp(variable,configuracion_kernel.var_globales.identificador[pos])==0){
			return pos;
		}
	}
	return -1;
}

/*
 * Nombre: inicializarSemaforos/0
 * Argumentos:
 * 		-
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: inicializa los mutex
 */

void inicializarSemaforos(void){
	mutex_cola_new=malloc(sizeof(pthread_mutex_t));
	mutex_cola_ready=malloc(sizeof(pthread_mutex_t));
	mutex_cola_exec=malloc(sizeof(pthread_mutex_t));
	mutex_cola_block=malloc(sizeof(pthread_mutex_t));
	mutex_cola_exit=malloc(sizeof(pthread_mutex_t));
	mutex_pid=malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(mutex_cola_new,NULL);
	pthread_mutex_init(mutex_cola_ready,NULL);
	pthread_mutex_init(mutex_cola_exec,NULL);
	pthread_mutex_init(mutex_cola_block,NULL);
	pthread_mutex_init(mutex_cola_exit,NULL);
	pthread_mutex_init(mutex_pid,NULL);
}

/*
 * Nombre: crearSemaforos/0
 * Argumentos:
 * 		-
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: inicializa los semaforos
 */

void crearSemaforos(void){
	if((sem_init(&sem_plp, 1, configuracion_kernel.multiprogramacion))==-1){
			perror("No se puede crear el semáforo");
			exit(1);
	}

	if((sem_init(&sem_pcp, 1, 0))==-1){
		perror("No se puede crear el semáforo");
		exit(1);
	}

	if((sem_init(&sem_new, 1, 0))==-1){
		perror("No se puede crear el semáforo");
		exit(1);
	}
}

/*
 * Nombre: cerrarSemaforos/0
 * Argumentos:
 * 		-
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: cierra los semaforos cuando terminan para liberar memoria
 */

void cerrarSemaforos(void){
	int sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_plp);
	}

	sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_pcp);
	}

	sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_new);
	}
}



/*
 * Nombre: agregarNuevoPrograma/2
 * Argumentos:
 * 		- codigo ansisop del programa nuevo conectado
 * 		- fd donde el Kernel se comunica con el programa
 *
 * Devuelve:
 * 		Nada
 *
 * Funcion: encola un nuevo programa en la cola de new
 */

void agregarNuevoPrograma(char* codigo, int fd){
	t_programa programa;
	programa.peso=0;
	programa.codigo=codigo;
	programa.flag_bloqueado=0;
	programa.flag_terminado=0;
	programa.metadata=malloc(sizeof(t_medatada_program));
	programa.metadata=metadata_desde_literal(codigo);
	programa.pcb=crearPcb(codigo, programa.metadata);
	programa.peso=calcularPeso(programa);
	printf("El peso es: %d\n",programa.peso);
	programa.socket_descriptor_conexion=fd;




	pthread_mutex_lock(mutex_cola_new);
	agregarAColaSegunPeso(programa,cola.new);
	pthread_mutex_unlock(mutex_cola_new);

	return;

}





/************************* FUNCIONES PARA EL MANEJO DE EPOLL *************************/

/*
 * Nombre: manejar_ConexionNueva_Programas
 * Argumentos:
 * 		- data del evento recibido en el epoll
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: acepta la conexion de un nuevo programa y lo agrega a la cola de New
 */

void manejar_ConexionNueva_Programas(epoll_data_t data){
	int fd_aceptado,j;
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	fd_aceptado=socket_aceptarCliente(data.fd);
	j=socket_recibir(fd_aceptado,&tipoRecibido,&structRecibida);
	if(j==1){
		printf("Se recibio bien el paquete\n");
		t_struct_string* k = ((t_struct_string*)structRecibida);
		agregarNuevoPrograma(k->string, fd_aceptado);
		sem_post(&sem_new);
	}
}

/*
 * Nombre: manejar_ConexionNueva_CPU/1
 * Argumentos:
 * 		- data del evento recibido en el epoll
 *
 * Devuelve:
 *		nada
 *
 * Funcion:
 * 		acepta la conexion de una nueva cpu al sistema
 */

void manejar_ConexionNueva_CPU(epoll_data_t data){
	int n, fd_aceptado, j;
	int* id;
	void* buffer;
	for(n=0; fds_conectados_cpu[n]!=NULL;n++){
		if(n<MAX_EVENTS_EPOLL){
			fd_aceptado=fds_conectados_cpu[n]=socket_aceptarCliente(data.fd);
			j=socket_recibir(fd_aceptado,(void*)&id,&buffer);
			if(j==1){
				printf("Se recibio una conexion de cpu\n");
			}
		}
	}

}
