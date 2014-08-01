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
	free(vector_string_num);
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
	int pos;
	for(pos=0; ((t_struct_descriptor_cpu*)list_get(cpus, pos))->socketCPU!=fd;pos++);
	t_struct_descriptor_cpu* cpu = (t_struct_descriptor_cpu*)list_get(cpus,pos);
	cpu->estado=LIBRE;
	cpu->id=-1;
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
 * Nombre:liberarMutex/0
 * Argumentos:
 * 		-nada
 *
 * Devuelve:
 *		nada
 *
 * Funcion: libera la memoria de los mutex
 */
void liberarMutex(void){
	free(mutex_cola_new);
	free(mutex_cola_ready);
	free(mutex_cola_exec);
	free(mutex_cola_block_io);
	free(mutex_cola_block_sem);
	free(mutex_cola_exit);
	free(mutex_pid);
	free(mutex_solicitarMemoria);
	free(mutex_array);
	free(mutex_semaforos);
	free(mutex_var_compartidas);
	free(mutex_log);

	return;
}

/*
 * Nombre:detruirColas/0
 * Argumentos:
 * 		-nada
 *
 * Devuelve:
 *		nada
 *
 * Funcion: destruye las colas para liberar memoria
 */
void destruirColas(void){
	list_destroy(cola.new);
	list_destroy(cola.ready);
	list_destroy(cola.exec);
	list_destroy(cola.block.io);
	list_destroy(cola.block.sem);
	list_destroy(cola.exit);

	return;
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

	if((sem_init(&sem_ready, 1, 0))==-1){
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
		sem_cerrado=sem_destroy(&sem_ready);
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
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Programa", INFO, "Se envio el programa %d a la cola Block I/O", programa->pcb->pid);
	pthread_mutex_unlock(mutex_log);

	return;
}




/*
 * Nombre: agregarNuevoPrograma/2
 * Argumentos:core_io
 * 		- codigo ansisop del programa nuevo conectado
 * 		- fd donde el Kernel se comunica con el programa
 *
 * Devuelve:
 * 		0 si se pudieron crear bien los segmentos y -1 si no hubo espacio
 *
 * Funcion: encola un nuevo programa en la cola de new
 */

int agregarNuevoPrograma(char* codigo, int fd){
	t_programa* programa=malloc(sizeof(t_programa));
	programa->peso=0;
	t_medatada_program* metadata = metadata_desde_literal(codigo);
	programa->pcb=crearPcb(codigo, metadata);
	if(programa->pcb!=0){
		programa->peso=calcularPeso(metadata);
		programa->socket_descriptor_conexion=fd;
		pthread_mutex_lock(mutex_cola_new);
		agregarAColaSegunPeso(programa,cola.new);
		pthread_mutex_unlock(mutex_cola_new);
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Programa", INFO, "Se conecto un nuevo Programa con el pid %d y peso %d", programa->pcb->pid, programa->peso);
		pthread_mutex_unlock(mutex_log);

		free(codigo);
		return 0;
	}else{
		free(programa);
		return -1;
	}

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
	int pos=0;
	for(pos=0; ((t_struct_descriptor_cpu*)list_get(cpus, pos))->socketCPU!=fd; pos++);

	return pos;
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
	t_struct_string* paquete = malloc(sizeof(t_struct_string));
	paquete->string=variablesAImprimir;
	socket_enviar(programa->socket_descriptor_conexion, D_STRUCT_STRING, paquete);
	free(paquete);
	t_struct_numero* fin = malloc(sizeof(t_struct_numero));
	fin->numero=0;
	socket_enviar(programa->socket_descriptor_conexion,D_STRUCT_PROGFIN, paquete);
	free(fin);
	sem_post(&sem_multiProg);
	mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.exit, mutex_cola_exit);
	pthread_mutex_lock(mutex_cola_exit);
	mostrarColasPorPantalla(cola.exit, "Exit");
	pthread_mutex_unlock(mutex_cola_exit);
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Estado programa",INFO,"Se finalizo el programa: %d", programa->pcb->pid);
	pthread_mutex_unlock(mutex_log);

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
	int fd_aceptado,j, i;
	log_escribir(archLog, "Programa",INFO,"Se recibe la solicitud de un nuevo programa");
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	fd_aceptado=socket_aceptarCliente(data.fd);
	epoll_agregarSocketCliente(efd_programas,fd_aceptado);
	j=socket_recibir(fd_aceptado,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_string* k = ((t_struct_string*)structRecibida);
		i = agregarNuevoPrograma(k->string, fd_aceptado);
		if(i==0){
			sem_post(&sem_new);
			pthread_mutex_lock(mutex_cola_new);
			mostrarColasPorPantalla(cola.new,"New");
			pthread_mutex_unlock(mutex_cola_new);
		}else{
			escribir_log(archLog, "Conexion Programa", INFO, "Se rechazó el programa nuevo por falta de espacio en memoria");
			t_struct_numero* paquete = malloc(sizeof(t_struct_numero));
			paquete->numero= 0;
			i=socket_enviar(fd_aceptado, D_STRUCT_MO, paquete);
			free(paquete);
		}
		free(k);
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
	int fd_aceptado;
	t_struct_numero* paquete = malloc(sizeof(t_struct_numero));
	uint32_t k=configuracion_kernel.quantum;
	paquete->numero=k;
	fd_aceptado=socket_aceptarCliente(data.fd);
	if((epoll_agregarSocketCliente(efd_cpu,fd_aceptado))==0){
		//	escribir_log(archLog,"Conexion", INFO, "Se acepto la conexion de cpu");
			pthread_mutex_lock(mutex_array);
			t_struct_descriptor_cpu* cpuNueva = malloc(sizeof(t_struct_descriptor_cpu));
			cpuNueva->estado=LIBRE;
			cpuNueva->id=-1;
			cpuNueva->socketCPU=fd_aceptado;
			list_add(cpus,(void*) cpuNueva);
//			fds_conectados_cpu[n]=fd_aceptado;
//			estado_cpu[n]=LIBRE;
			pthread_mutex_unlock(mutex_array);
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Conexion", INFO, "Se acepto la conexion de una cpu");
			pthread_mutex_unlock(mutex_log);
			socket_enviar(fd_aceptado,D_STRUCT_NUMERO,paquete);
			sem_post(&sem_cpu);
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
	t_struct_nombreMensaje* mensaje;
	t_struct_string* string;
	t_struct_numero* num;
	t_struct_asignar_compartida* compartida;
	t_struct_pcb* pcb;
	t_struct_pcb_fin* pcb_fin;
	t_programa* programa;
	switch(tipoRecibido){
		case D_STRUCT_NOMBREMENSAJE:
			mensaje = ((t_struct_nombreMensaje*)structRecibida);
			programa = (t_programa*)buscarPrograma(mensaje->pid,cola.exec, mutex_cola_exec);
			t_struct_string* textoAImprimir = malloc(sizeof(t_struct_string));
			textoAImprimir->string = mensaje->mensaje;
			int fd = programa->socket_descriptor_conexion;
			socket_enviar(fd,D_STRUCT_STRING,textoAImprimir);
			free(textoAImprimir);
			break;
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
			break;
		case D_STRUCT_STRING:
			string = malloc(sizeof(t_struct_string));
			string->string= ((t_struct_string*)structRecibida)->string;
			printf("%s", ((t_struct_string*)structRecibida)->string);
			int pos = buscar_cpu_por_fd(data.fd);
			t_struct_descriptor_cpu* cpu = (t_struct_descriptor_cpu*)list_get(cpus, pos);
			programa = (t_programa*)buscarPrograma(cpu->id, cola.exec, mutex_cola_exec);
			socket_enviar(programa->socket_descriptor_conexion,D_STRUCT_STRING, string);
			num = malloc(sizeof(t_struct_numero));
			num->numero=1;
			socket_enviar(data.fd, D_STRUCT_NUMERO, string);
			free(string);
			free(num);
			break;
		case D_STRUCT_IMPRIMIR:
			num = malloc(sizeof(t_struct_numero));
			num->numero=((t_struct_numero*)structRecibida)->numero;
			printf("%d\n",num->numero);
			pos = buscar_cpu_por_fd(data.fd);
			cpu = (t_struct_descriptor_cpu*)list_get(cpus, pos);
			programa = (t_programa*)buscarPrograma(cpu->id, cola.exec, mutex_cola_exec);
			socket_enviar(programa->socket_descriptor_conexion,D_STRUCT_NUMERO, num);
			free(num);


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
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Variables Compartidas", INFO, "Se le asigno el valor %d a la variable %s", compartida->valor, compartida->nombre);
				pthread_mutex_unlock(mutex_log);
				pthread_mutex_unlock(mutex_var_compartidas);
				num = malloc(sizeof(t_struct_numero));
				num->numero=1;
				socket_enviar(data.fd,D_STRUCT_NUMERO,num);
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
				senial->numero=1;
				socket_enviar(data.fd, D_STRUCT_NUMERO,senial);
				configuracion_kernel.semaforos.valor[pos_sem_wait]--;
			}else{
				senial->numero=0;
				socket_enviar(data.fd, D_STRUCT_NUMERO, senial);
				t_tipoEstructura tipoRecibido;
				void * estructuraRecibida;
				socket_recibir(data.fd,&tipoRecibido,&estructuraRecibida);
				configuracion_kernel.semaforos.valor[pos_sem_wait]--;
				queue_push(configuracion_kernel.semaforos.cola_procesos[pos_sem_wait],estructuraRecibida);
				programa=(t_programa*)buscarPrograma(((t_struct_pcb*)estructuraRecibida)->pid, cola.exec, mutex_cola_exec);
				mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.block.sem, mutex_cola_block_sem);
				pthread_mutex_lock(mutex_cola_block_sem);
				mostrarColasPorPantalla(cola.block.sem, "Bloqueados por semaforos");
				pthread_mutex_unlock(mutex_cola_block_sem);

			}
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Semaforo", INFO, "Se dio wait al semaforo %s", semaforo->nombre_semaforo);
			pthread_mutex_unlock(mutex_log);
			pthread_mutex_unlock(mutex_semaforos);
			free(senial);
			break;

		case D_STRUCT_SIGNALSEMAFORO:
			semaforo = ((t_struct_semaforo*)structRecibida);
			pthread_mutex_lock(mutex_semaforos);
			int pos_sem_signal = posicion_Semaforo(semaforo->nombre_semaforo);
			configuracion_kernel.semaforos.valor[pos_sem_signal]++;
			if(queue_size(configuracion_kernel.semaforos.cola_procesos[pos_sem_signal])>0){
				t_programa* prog = queue_pop(configuracion_kernel.semaforos.cola_procesos[pos_sem_signal]);
				programa = (t_programa*)buscarPrograma(prog->pcb->pid, cola.block.sem, mutex_cola_block_sem);
				mandarAOtraCola(programa, cola.block.sem, mutex_cola_block_sem, cola.ready, mutex_cola_ready);

			}
			pthread_mutex_unlock(mutex_semaforos);
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Semaforo", INFO, "Se dio signal al semaforo %s", semaforo->nombre_semaforo);
			pthread_mutex_unlock(mutex_log);
			break;

		case D_STRUCT_PCBSEM:
			liberarCPU(data.fd);
			pcb = ((t_struct_pcb*)structRecibida);
			programa = (t_programa*)buscarPrograma(pcb->pid,cola.exec, mutex_cola_exec);
			actualizarPCB(programa, pcb);
			mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.block.sem, mutex_cola_block_sem);
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Programa", INFO, "Se envio el programa %d a la cola Block por Semaforos", programa->pcb->pid);
			pthread_mutex_unlock(mutex_log);
			pthread_mutex_lock(mutex_cola_block_sem);
			mostrarColasPorPantalla(cola.block.sem, "Block por Semaforos");
			pthread_mutex_unlock(mutex_cola_block_sem);


			break;
		case D_STRUCT_PCBSF:
			liberarCPU(data.fd);
			pcb = ((t_struct_pcb*)structRecibida);
			programa = (t_programa*)buscarPrograma(pcb->pid,cola.exec, mutex_cola_exec);
			free(programa->pcb);
			mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.exit, mutex_cola_exit);
			num = malloc(sizeof(t_struct_numero));
			num->numero=0;
			socket_enviar(programa->socket_descriptor_conexion, D_STRUCT_SF, num);
			free(num);


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

			break;
		case D_STRUCT_PCBFIN:
			liberarCPU(data.fd);
			pcb_fin = ((t_struct_pcb_fin*)structRecibida);
			pcb->c_stack=pcb_fin->c_stack;
			pcb->codigo=pcb_fin->codigo;
			pcb->index_codigo=pcb_fin->index_codigo;
			pcb->index_etiquetas=pcb_fin->index_etiquetas;
			pcb->program_counter=pcb_fin->program_counter;
			pcb->stack=pcb_fin->stack;
			pcb->tamanio_contexto=pcb_fin->tamanio_contexto;
			pcb->tamanio_indice=pcb_fin->tamanio_indice;
			num = malloc(sizeof(t_struct_numero));
			num->numero=pcb_fin->pid;
			socket_enviar(sock_umv, D_STRUCT_DESTRUIRSEGMENTOS,num);
			programa = (t_programa*) buscarPrograma(pcb_fin->pid, cola.exec, mutex_cola_exec);
			actualizarPCB(programa, pcb);
			string = malloc(sizeof(t_struct_string));
			string->string=pcb_fin->variables;
			socket_enviar(programa->socket_descriptor_conexion, D_STRUCT_STRING, string);
			free(string);
			num->numero=0;
			socket_enviar(programa->socket_descriptor_conexion, D_STRUCT_PROGFIN,num);
			free(num);
			mandarAOtraCola(programa, cola.exec, mutex_cola_exec, cola.exit, mutex_cola_exit);
			pthread_mutex_lock(mutex_cola_exit);
			mostrarColasPorPantalla(cola.exit, "Exit");
			pthread_mutex_unlock(mutex_cola_exit);
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Programa", INFO, "Se envio el programa %d a la cola Exit", programa->pcb->pid);
			pthread_mutex_unlock(mutex_log);
			free(programa->pcb);
			sem_post(&sem_multiProg);
	}
//	pthread_mutex_lock(mutex_cola_ready);
//	mostrarColasPorPantalla(cola.ready,"Ready");
//	pthread_mutex_unlock(mutex_cola_ready);
	sem_post(&sem_cpu);
	free(structRecibida);

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
	for(pos=0; ((t_struct_descriptor_cpu*)list_get(cpus, pos))->socketCPU!=data.fd; pos++);
	list_remove(cpus,pos);
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Conexiones CPU", INFO, "Se desconecto una cpu");
	pthread_mutex_unlock(mutex_log);


}
