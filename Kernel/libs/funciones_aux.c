/*
 * funciones_aux.c
 *
 *  Created on: 18/05/2014
 *      Author: utnso
 */

#include "funciones_aux.h"

/*
 * Nombre: escribir_log/4
 * Argumentos:
 * 		-
 *
 * Devuelve:
 *
 *
 * Funcion:
 */

int escribir_log(log_t *log, const char *program_name, e_message_type type,	const char* format){
	pthread_mutex_lock(mutex_log);
	int i = log_escribir(log, program_name, type, format);
	pthread_mutex_unlock(mutex_log);
	return i;
}
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
 * Nombre: validarSemaforo
 * Argumentos:
 * 		- semaforo(string)
 *
 * Devuelve:
 * 		0 si es valido, -1 si es invalido
 *
 * Funcion: busca si el semaforo existe
 */

int validarSemaforo(char* semaforo){
	int i=0;
	pthread_mutex_lock(mutex_semaforos);
	while(configuracion_kernel.semaforos.id[i]!=semaforo){
		i++;
	}
	pthread_mutex_unlock(mutex_semaforos);
	if(cant_identificadores(configuracion_kernel.semaforos.id)<i){
		return 0;
	}
	return -1;
}

/*
 * Nombre: validarVarGlobal
 * Argumentos:
 * 		- variable global(string)
 *
 * Devuelve:
 * 		0 si es valido, -1 si es invalido
 *
 * Funcion: busca si la variable global existe
 */

int validarVarGlobal(char* var_global){
	int i=0;
	pthread_mutex_lock(mutex_var_compartidas);
	while(configuracion_kernel.var_globales.identificador[i]!=var_global){
		i++;
	}
	pthread_mutex_unlock(mutex_var_compartidas);
	if(cant_identificadores(configuracion_kernel.var_globales.identificador)<i){
		return 0;
	}
	return -1;
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
	escribir_log(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
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
 * Nombre: valor_Variable_Global
 * Argumentos:
 * 		- nombre de la variable
 *
 * Devuelve:
 * 		- el valor de la variable global o -1 en caso de error
 *
 * Funcion:	busca una variable global en la configuracion y retorna su valor
 */


int valor_Variable_Global(char* variable){
	pthread_mutex_lock(mutex_var_compartidas);
	int pos = posicion_Variable_Global(variable);
	if(pos!=-1){
		int valor = configuracion_kernel.var_globales.valor[pos];
		pthread_mutex_unlock(mutex_var_compartidas);
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Variable Global", ERROR, "Se solicitó la variable global: %s, pero esta no existe", variable);
		pthread_mutex_unlock(mutex_log);
		return valor;
	}
	pthread_mutex_unlock(mutex_var_compartidas);
	return -1;
}

/*
 * Nombre:posicion_Semaforo/1
 * Argumentos:
 * 		-identif semaforo
 *
 * Devuelve:
 *		posicion en el array
 *
 * Funcion:
 */

int posicion_Semaforo(char* semaforo){
	int pos;
	for(pos=0;configuracion_kernel.semaforos.id[pos]!=semaforo;pos++){
	}
	return pos;
}


/*
 * Nombre: liberarCPU/1
 * Argumentos:
 * 		- fd
 *
 * Devuelve: nada
 *
 *
 * Funcion: libera un cpu
 */

void liberarCPU(int fd){
	pthread_mutex_lock(mutex_array);
	int pos = buscar_cpu_por_fd(fd);
	estado_cpu[pos]=LIBRE;
	pthread_mutex_unlock(mutex_array);
	return;
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
	mutex_cola_block_io=malloc(sizeof(pthread_mutex_t));
	mutex_cola_block_sem=malloc(sizeof(pthread_mutex_t));
	mutex_cola_exit=malloc(sizeof(pthread_mutex_t));
	mutex_pid=malloc(sizeof(pthread_mutex_t));
	mutex_solicitarMemoria=malloc(sizeof(pthread_mutex_t));
	mutex_array=malloc(sizeof(pthread_mutex_t));
	mutex_semaforos=malloc(sizeof(pthread_mutex_t));
	mutex_var_compartidas=malloc(sizeof(pthread_mutex_t));
	mutex_log=malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(mutex_cola_new,NULL);
	pthread_mutex_init(mutex_cola_ready,NULL);
	pthread_mutex_init(mutex_cola_exec,NULL);
	pthread_mutex_init(mutex_cola_block_io,NULL);
	pthread_mutex_init(mutex_cola_block_sem,NULL);
	pthread_mutex_init(mutex_cola_exit,NULL);
	pthread_mutex_init(mutex_pid,NULL);
	pthread_mutex_init(mutex_solicitarMemoria,NULL);
	pthread_mutex_init(mutex_array,NULL);
	pthread_mutex_init(mutex_semaforos,NULL);
	pthread_mutex_init(mutex_var_compartidas,NULL);
	pthread_mutex_init(mutex_log,NULL);

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
	if((sem_init(&sem_multiProg, 1, 0))==-1){
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
	list_add(cola.block.io,(void*)programa);

	return;
}




/*
 * Nombre: agregarNuevoPrograma/2
 * Argumentos:core_io
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
 * 		- mutex de la lista
 *
 * Devuelve:
 * 		la data de la pcb que se busca
 *
 * Funcion: recorre la lista buscando el programa con el pid que recibe por parametro
 */

void* buscarPrograma(int pid, t_list* lista, pthread_mutex_t *mutex){
	pthread_mutex_lock(mutex);
	if(lista->head==NULL){
		return NULL;
	}
	t_link_element* element = lista->head;
	while((element!=NULL) && ((t_programa*)element->data)->pcb->pid!= pid){
		element = element->next;
	}
	pthread_mutex_unlock(mutex);
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

void mandarAOtraCola(t_programa* programa, t_list* listaActual, pthread_mutex_t* mutexActual, t_list* listaNueva, pthread_mutex_t* mutexNuevo){
	t_link_element* element=listaActual->head;
	int pos=0;
	pthread_mutex_lock(mutexActual);
	pthread_mutex_lock(mutexNuevo);
	while((element != NULL) && ((((t_programa*)element->data)->pcb->pid) != (programa->pcb->pid))){
		pos++;
	}
	if(element!=NULL){
	list_remove(listaActual,pos);
	pthread_mutex_unlock(mutexActual);
	list_add(listaNueva, programa);
	pthread_mutex_unlock(mutexNuevo);
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

int buscar_cpu_por_fd(int fd){
	int i=0;
	while(fds_conectados_cpu[i]!=fd){
		i++;
	}

	return i;
}


/*
 * Nombre: finalizarPrograma
 * Argumentos:
 * 		- programa
 *
 * Devuelve:
 * 		nada
 *
 * Funcion: manda al programa
 */

void finalizarPrograma(t_programa* programa, char* variablesAImprimir){
	pthread_mutex_lock(mutex_cola_exec);
	pthread_mutex_lock(mutex_cola_exit);
	t_struct_string* paquete = malloc(sizeof(t_struct_string));
	paquete->string=variablesAImprimir;
	socket_enviar(programa->socket_descriptor_conexion, D_STRUCT_STRING, paquete);
	paquete->string=0;
	socket_enviar(programa->socket_descriptor_conexion,D_STRUCT_STRING, paquete);
	free(paquete);
	sem_post(&sem_multiProg);
	list_add(cola.exit,(void*)programa);
	pthread_mutex_unlock(mutex_cola_exec);
	mostrarColasPorPantalla(cola.exit, "Exit");
	pthread_mutex_unlock(mutex_cola_exit);
	escribir_log(archLog, "Estado programa",INFO,"Se finalizo un programa");
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
	epoll_agregarSocketCliente(efd_programas,fd_aceptado);
	j=socket_recibir(fd_aceptado,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_string* k = ((t_struct_string*)structRecibida);
		agregarNuevoPrograma(k->string, fd_aceptado);
		free(k);
		sem_post(&sem_new);
		escribir_log(archLog, "Conexion Programa", INFO, "Se conecto un nuevo Programa");
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
	for(n=0; fds_conectados_cpu[n]!=0;n++){
	}
	if(n<MAX_EVENTS_EPOLL){
		fd_aceptado=socket_aceptarCliente(data.fd);
		if((epoll_agregarSocketCliente(efd_cpu,fd_aceptado))==0){
		//	escribir_log(archLog,"Conexion", INFO, "Se acepto la conexion de cpu");
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
		escribir_log(archLog,"Conexion",ERROR,"No se pudo conectar la cpu");
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
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(data.fd,&tipoRecibido,&structRecibida);
	t_struct_semaforo* semaforo;
	t_struct_pcb_io* pcb_io;
	t_struct_string* string;
	t_struct_asignar_compartida* compartida;
	t_struct_pcb* pcb;
	t_struct_pcb_fin* pcb_fin;
	t_programa* programa;
	switch(tipoRecibido){
		case D_STRUCT_PCB:
			liberarCPU(data.fd);
			pcb = ((t_struct_pcb*)structRecibida);
			programa = (t_programa*)buscarPrograma(pcb->pid,cola.exec, mutex_cola_exec);
			if(programa != NULL){
				actualizarPCB(programa, pcb);
				mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.ready, mutex_cola_ready);
			}else{
				escribir_log(archLog, "PCB", ERROR, "La cola de exec estaba vacia al recibir un pcb de CPU");
			}
			sem_post(&sem_cpu);
			sem_post(&sem_multiProg);
			break;
		case D_STRUCT_OBTENERCOMPARTIDA:
			string = ((t_struct_string*)structRecibida);
			if((validarVarGlobal(string->string))==0){
				int valor = valor_Variable_Global(string->string);
				t_struct_numero* paquete = malloc(sizeof(t_struct_numero));
				paquete->numero=valor;
				socket_enviar(data.fd,D_STRUCT_NUMERO,paquete);
				free(paquete);
			}else{
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Variables globales", ERROR, "La variable '%s' no está en el archivo de Configuraciones", string->string);
				pthread_mutex_unlock(mutex_log);
			}
			break;

		case D_STRUCT_ASIGNARCOMPARTIDA:
			compartida = ((t_struct_asignar_compartida*)structRecibida);
			if((validarVarGlobal(string->string))==0){
				int posicion = posicion_Variable_Global(compartida->nombre);
				pthread_mutex_lock(mutex_var_compartidas);
				configuracion_kernel.var_globales.valor[posicion]=compartida->valor;
				pthread_mutex_unlock(mutex_var_compartidas);
			}else{
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Variables globales", ERROR, "La variable '%s' no está en el archivo de Configuraciones", string->string);
				pthread_mutex_unlock(mutex_log);

			}
			break;

		case D_STRUCT_WAIT:
			semaforo = ((t_struct_semaforo*)structRecibida);
			t_struct_numero* senial = malloc(sizeof(t_struct_numero));
			pthread_mutex_lock(mutex_semaforos);
			int pos_sem_wait = posicion_Semaforo(semaforo->nombre_semaforo);
			if(configuracion_kernel.semaforos.valor[pos_sem_wait]>0){
				senial->numero=0;
			}else{
				senial->numero=1;
			}
			socket_enviar(data.fd, D_STRUCT_NUMERO,senial);
			configuracion_kernel.semaforos.valor[pos_sem_wait]-=1;
			pthread_mutex_unlock(mutex_semaforos);
			free(senial);
			break;

		case D_STRUCT_SIGNALSEMAFORO:
			semaforo = ((t_struct_semaforo*)structRecibida);
			pthread_mutex_lock(mutex_semaforos);
			int pos_sem_signal = posicion_Semaforo(semaforo->nombre_semaforo);
			configuracion_kernel.semaforos.valor[pos_sem_signal]+=1;
			pthread_mutex_unlock(mutex_semaforos);
			break;

		case D_STRUCT_PCBSEM:
			liberarCPU(data.fd);
			pcb = ((t_struct_pcb*)structRecibida);
			programa = (t_programa*)buscarPrograma(pcb->pid,cola.exec, mutex_cola_exec);
			actualizarPCB(programa, pcb);
			mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.block.sem, mutex_cola_block_sem);
			pthread_mutex_lock(mutex_cola_block_sem);
			mostrarColasPorPantalla(cola.block.sem, "Block por Semaforos");
			pthread_mutex_unlock(mutex_cola_block_sem);


			break;
		case D_STRUCT_PCBIO:
			liberarCPU(data.fd);
			t_struct_pcb_io* bloqueo = ((t_struct_pcb_io*)structRecibida);
			pthread_mutex_lock(mutex_cola_exec);
			pthread_mutex_lock(mutex_cola_block_io);
			bloquearPrograma(bloqueo->pid);
			pthread_mutex_unlock(mutex_cola_exec);
			mostrarColasPorPantalla(cola.block.io,"block I/O");
			pthread_mutex_unlock(mutex_cola_block_io);
			pthread_create(&io, NULL, (void*) &core_io, bloqueo);

			sem_post(&sem_cpu);

			break;
		case D_STRUCT_PCBFIN:
			liberarCPU(data.fd);
			pcb_fin = ((t_struct_pcb_fin*)structRecibida);


			printf("llego %s\n",pcb_fin->variables);
	}


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
		//	escribir_log(archLog, "Conexiones CPU", INFO, "Se desconectó una cpu");
			fds_conectados_cpu[pos]=0;
			break;
		}

	}
}
