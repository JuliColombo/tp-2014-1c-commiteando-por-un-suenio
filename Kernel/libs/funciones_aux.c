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

void inicializarMutex(void){
	mutex_cola_new=malloc(sizeof(pthread_mutex_t));
	mutex_cola_ready=malloc(sizeof(pthread_mutex_t));
	mutex_cola_exec=malloc(sizeof(pthread_mutex_t));
	mutex_cola_block=malloc(sizeof(pthread_mutex_t));
	mutex_cola_exit=malloc(sizeof(pthread_mutex_t));
	mutex_pid=malloc(sizeof(pthread_mutex_t));
	mutex_solicitarMemoria=malloc(sizeof(pthread_mutex_t));
	mutex_array=malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(mutex_cola_new,NULL);
	pthread_mutex_init(mutex_cola_ready,NULL);
	pthread_mutex_init(mutex_cola_exec,NULL);
	pthread_mutex_init(mutex_cola_block,NULL);
	pthread_mutex_init(mutex_cola_exit,NULL);
	pthread_mutex_init(mutex_pid,NULL);
	pthread_mutex_init(mutex_solicitarMemoria,NULL);
	pthread_mutex_init(mutex_array,NULL);
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
	if((sem_init(&sem_multiProg, 1, configuracion_kernel.multiprogramacion))==-1){
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

	if((sem_init(&sem_cpu, 1, 0))==-1){
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
		sem_cerrado=sem_destroy(&sem_multiProg);
	}

	sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_pcp);
	}

	sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_new);
	}

	sem_cerrado=-1;

	while(sem_cerrado==-1){
		sem_cerrado=sem_destroy(&sem_cpu);
	}

}


/*
 * Nombre: bloquearPrograma/1
 * Argumentos:
 * 		-pid
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: pasa un programa, buscandolo por su pid, de la cola de exec a la de bloqueados
 */

void bloquearPrograma(int pid){
	t_link_element *element= cola.exec->head;
	int position = 0;

	while ((element != NULL)&&(((t_programa*)(element->data))->pcb->pid !=pid)){
		element = element->next;
		position++;
	}
	t_programa* programa;
	programa = (t_programa*) list_remove(cola.exec,position);
	list_add(cola.block,(void*)programa);

	return;
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
	t_programa* programa=malloc(sizeof(t_programa));
	programa->peso=0;
	programa->codigo=codigo;
	programa->flag_bloqueado=0;
	programa->flag_terminado=0;
	programa->metadata=malloc(sizeof(t_medatada_program));
	programa->metadata=metadata_desde_literal(codigo);
	if((programa->pcb=crearPcb(codigo, programa->metadata, fd))!=NULL){
		programa->peso=calcularPeso(programa);
		programa->socket_descriptor_conexion=fd;

		pthread_mutex_lock(mutex_cola_new);
		agregarAColaSegunPeso(programa,cola.new);
		pthread_mutex_unlock(mutex_cola_new);
	}else{
		free(programa);
	}


	return;

}

/*
 * Nombre: actualizarPCB/2
 * Argumentos:
 * 		-programa
 * 		-pcb actualizada
 *
 * Devuelve:
 *		nada
 *
 * Funcion: actualiza el pcb recibido de una cpu y libera la memoria en que estaba la pcb vieja
 */
void actualizarPCB(t_programa* programa, t_struct_pcb* pcb ){
	programa->pcb->pid = pcb->pid;
	programa->pcb->c_stack=pcb->c_stack;
	programa->pcb->codigo=pcb->codigo;
	programa->pcb->index_codigo=pcb->index_codigo;
	programa->pcb->index_etiquetas=pcb->index_etiquetas;
	programa->pcb->program_counter=pcb->program_counter;
	programa->pcb->stack=pcb->stack;
	programa->pcb->tamanio_contexto=pcb->tamanio_contexto;
	programa->pcb->tamanio_indice=pcb->tamanio_indice;
	free(pcb);

	return;
}

/*
 * Nombre: buscarPrograma/2
 * Argumentos:
 * 		- pid
 * 		- lista
 *
 * Devuelve:
 * 		la data de la pcb que se busca
 *
 * Funcion: recorre la lista buscando el programa con el pid que recibe por parametro
 */

void* buscarPrograma(int pid, t_list* lista){
	if(lista->head==NULL){
		return NULL;
	}
	t_link_element* element = lista->head;
	while( (element!=NULL) && (((t_programa*)element->data)->pcb->pid!=pid)){
		element = element->next;
	}
	if(element == NULL){
		perror("No esta el elemento en la lista");
	}
	return element->data;
}

/*
 * Nombre: mandarAReady
 * Argumentos:
 * 		-programa
 *
 * Devuelve:
 *		nada
 *
 * Funcion:envia un programa de exec a ready
 */

void mandarAReady(t_programa* programa){
	t_link_element* element=cola.exec->head;
	int pos=0;
	pthread_mutex_lock(mutex_cola_exec);
	pthread_mutex_lock(mutex_cola_ready);
	while((element != NULL) && ((((t_programa*)element->data)->pcb->pid) != (programa->pcb->pid))){
		pos++;
	}
	if(element!=NULL){
	list_remove(cola.exec,pos);
	pthread_mutex_unlock(mutex_cola_exec);
	list_add(cola.ready, programa);
	pthread_mutex_unlock(mutex_cola_ready);
	}
	return;
}

/*
 * Nombre: buscar_cpu_libre
 * Argumentos:
 * 		-nada
 *
 * Devuelve:
 *		- devuelve el fd de la cpu
 *
 * Funcion: busca el fd de una cpu libre
 */
int buscar_cpu_libre(void){
	int i=0;
	while(estado_cpu[i]!=LIBRE){
		i++;
	}
	return i;
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
		t_struct_string* k = ((t_struct_string*)structRecibida);
		agregarNuevoPrograma(k->string, fd_aceptado);
		free(k);
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
	int n, fd_aceptado;
	t_struct_numero* paquete = malloc(sizeof(t_struct_numero));
	uint32_t k=configuracion_kernel.quantum;
	paquete->numero=k;
	for(n=0; estado_cpu[n]!=LIBRE;n++){
	}
	if(n<MAX_EVENTS_EPOLL){
		fd_aceptado=socket_aceptarCliente(data.fd);
		if((epoll_agregarSocketCliente(efd_cpu,fd_aceptado))==0){
			log_escribir(archLog,"Conexion", INFO, "Se acepto la conexion de cpu");
			pthread_mutex_lock(mutex_array);
			fds_conectados_cpu[n]=fd_aceptado;
			estado_cpu[n]=LIBRE;
			pthread_mutex_unlock(mutex_array);
			socket_enviar(fd_aceptado,D_STRUCT_NUMERO,paquete);
			k=configuracion_kernel.retardo_quantum;
			paquete->numero=k;
			socket_enviar(fd_aceptado,D_STRUCT_NUMERO,paquete);
			sem_post(&sem_cpu);
		}
	} else {
		log_escribir(archLog,"Conexion",ERROR,"Ya no se pueden conectar mas CPUs");
	}
	free(paquete);


}

/*
 * Nombre: handler_conexion_cpu
 * Argumentos:
 * 		-data
 *
 * Devuelve:
 *		nada
 *
 * Funcion: actualiza el pcb y envia el programa a ready
 */

void handler_conexion_cpu(epoll_data_t data){
	printf("Llega al handler\n");
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(data.fd,&tipoRecibido,&structRecibida);
	t_struct_pcb* pcb = ((t_struct_pcb*)structRecibida);
	printf("el pid recibido es: %d\n", pcb->pid);
	pthread_mutex_lock(mutex_cola_exec);
	t_programa* programa = (t_programa*)buscarPrograma(pcb->pid,cola.exec);
	actualizarPCB(programa, pcb);
	pthread_mutex_unlock(mutex_cola_exec);
	mandarAReady(programa);
	sem_post(&sem_cpu);
	return;
}

/*
 * Nombre: desconectar_cpu/1
 * Argumentos:
 * 		- data epoll
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: saca la cpu del kernel
 */

void desconexion_cpu(epoll_data_t data){
	int pos;
	for(pos=0; pos<MAX_EVENTS_EPOLL; pos++){
		if(fds_conectados_cpu[pos]==data.fd){
			estado_cpu[pos]=LIBRE;
			break;
		}

	}
}
