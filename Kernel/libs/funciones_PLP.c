#include "funciones_PLP.h"

pthread_t hilo_pcp_new, hilo_pcp_ready;

int calcularPeso(t_medatada_program* metadata){ //Calcula peso del programa
	int peso=(5*metadata->cantidad_de_etiquetas + 3* metadata->cantidad_de_funciones + metadata->instrucciones_size);

	return peso;
}

void agregarAColaSegunPeso(t_programa* programa, t_list* lista){
	t_link_element* element = lista->head;
	int position = 0;

	while ((element != NULL)&&((((t_programa*)(element->data))->peso) < (programa->peso))) {
		element = element->next;
		position++;
	}

	list_add_in_index(lista, position, programa);
}

void mostrarColasPorPantalla(t_list* lista, char* nombreLista){
	int peso, pid;
	if(lista->head==NULL){
		printf("No hay programas en la cola %s\n", nombreLista);
		return;
	}
	printf("Estado de la cola %s:\nPID  PESO\n", nombreLista);
	t_link_element* element = lista->head;
	while(element!=NULL){
		t_programa* programa = element->data;
		pid = programa->pcb->pid;
		peso = programa->peso;
		printf("%d    %d\n", pid, peso);
		element=element->next;
	}
	return;
}

int cantidadProgramasEnPCP(void){
	pthread_mutex_lock(mutex_cola_ready);
	pthread_mutex_lock(mutex_cola_exec);
	pthread_mutex_lock(mutex_cola_block_io);
	pthread_mutex_lock(mutex_cola_block_sem);

	int cantidad = (list_size(cola.ready)+list_size(cola.exec)+list_size(cola.block.io)+list_size(cola.block.sem));

	pthread_mutex_unlock(mutex_cola_ready);
	pthread_mutex_unlock(mutex_cola_exec);
	pthread_mutex_unlock(mutex_cola_block_io);
	pthread_mutex_unlock(mutex_cola_block_sem);
	return cantidad;
}



void inicializarColas(void){
	cola.new=list_create();
	cola.ready=list_create();
	cola.exec=list_create();
	cola.block.io=list_create();
	cola.block.sem=list_create();
	cola.exit=list_create();
}

void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH_config, &file_info);
	if (control == -1){
		escribir_log(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
	}else{
		leerConfiguracion();
		imprimirConfiguracion();
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH_config);

	configuracion_kernel.puerto_programas = config_get_int_value(config,"Puerto TCP para recibir conexiones de los Programas");
	configuracion_kernel.puerto_cpus = config_get_int_value(config,"Puerto TCP para recibir conexiones de los CPUs");
	configuracion_kernel.quantum = config_get_int_value(config,"Quantum de tiempo para algoritmos expropiativos");
	configuracion_kernel.retardo_quantum = config_get_long_value(config,"Retardo del Quantum");
	configuracion_kernel.multiprogramacion = config_get_int_value(config,"Maximo nivel de multiprogramacion");
	pthread_mutex_lock(mutex_semaforos);
	/*configuracion_kernel.semaforos.id = config_get_array_value(config,"Lista de nombres de Semaforos");
	configuracion_kernel.semaforos.valor =vector_num(config_get_array_value(config,"Lista de valores de Semaforos"),configuracion_kernel.semaforos.id);
	procesos_en_espera = list_create();
	int i;
	for(i=0; i<cant_identificadores(configuracion_kernel.semaforos.id);i++){
		t_cola_procesos* cola_proc = malloc(sizeof(t_cola_procesos));
		cola_proc->id = configuracion_kernel.semaforos.id[i];
		cola_proc->cola_procesos=queue_create();
		list_add(procesos_en_espera, cola_proc);
	}*/
	pthread_mutex_unlock(mutex_semaforos);
	configuracion_kernel.hio.id = config_get_array_value(config,"Lista de hio");
	configuracion_kernel.hio.retardo = vector_num(config_get_array_value(config,"Retardo de hio"),configuracion_kernel.hio.id);
	configuracion_kernel.ip_umv = config_get_string_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_kernel.puerto_umv = config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
	pthread_mutex_lock(mutex_var_compartidas);
	configuracion_kernel.var_globales.identificador = config_get_array_value(config,"Variables globales");
	configuracion_kernel.var_globales.valor = vector_num(NULL, configuracion_kernel.var_globales.identificador);
	pthread_mutex_unlock(mutex_var_compartidas);
	configuracion_kernel.tamanio_stack = config_get_int_value(config,"Tamanio del Stack");
	escribir_log(archLog, "Archivo de configuracion", INFO, "Se carga correctamente la configuracion");

	char* tempSemaforos = strdup(config_get_string_value(config,"Lista de nombres de Semaforos"));
	int cantidad_semaforos = string_count(tempSemaforos, ',') + 1;
	char** semaforos = string_get_string_as_array(tempSemaforos);
	free(tempSemaforos);

	char* temp_valorIni = strdup(config_get_string_value(config,"Lista de valores de Semaforos"));
	char** valor_semaforo = string_get_string_as_array(temp_valorIni);
	free(temp_valorIni);

	configuracion_kernel.semaforos = dictionary_create();

	int j = 0;
	int valor_entero;

	while (j < cantidad_semaforos) {
		t_struct_contenido_semaforo *contenido = malloc(sizeof(t_struct_contenido_semaforo));
		contenido->estado = atoi(valor_semaforo[j]); //hay que buscar el valor del arch
		contenido->cola_procesos = queue_create();

		dictionary_put(configuracion_kernel.semaforos, semaforos[j], contenido);
		valor_entero = atoi(valor_semaforo[j]);
		printf("- Semaforo: %s -> Valor: %d\n", semaforos[j], valor_entero);
		j++;
	}

	free(config);
}

int string_count(char * text, char c) {
	int i = 0;
	int pos = 0;

	while (text[pos] != '\0') {
		if (text[pos] == c)
			i++;
		pos++;
	}
	return i;
}


void imprimirConfiguracion() { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Puerto programas: %d\n", configuracion_kernel.puerto_programas);
	printf("Puerto CPUs: %d\n", configuracion_kernel.puerto_cpus);
	printf("Quantum: %d\n", configuracion_kernel.quantum);
	printf("Retardo quantum: %lu\n", configuracion_kernel.retardo_quantum);
	printf("Grado de multiprogramacion: %d\n", configuracion_kernel.multiprogramacion);


	int i;
	/*for(i=0;i<cant_identificadores(configuracion_kernel.semaforos.id);i++){
		printf("Semaforo (valor): %s (%d)\n",configuracion_kernel.semaforos.id[i], configuracion_kernel.semaforos.valor[i]);
	}*/

	for(i=0;configuracion_kernel.hio.id[i]!=NULL;i++){
		printf("ID HIO (retardo): %s ", configuracion_kernel.hio.id[i]);
		printf("(%d)\n", configuracion_kernel.hio.retardo[i]);
	}
	printf("IP de la UMV: %s\n", configuracion_kernel.ip_umv);
	printf("Puerto UMV: %d\n", configuracion_kernel.puerto_umv);
	
	printf("Variables globales: ");
	for(i=0;configuracion_kernel.var_globales.identificador[i]!=NULL;i++){
		printf("\n%s (%d) ", configuracion_kernel.var_globales.identificador[i], configuracion_kernel.var_globales.valor[i]);
	}

	printf("\nTamaño del Stack: %d\n", configuracion_kernel.tamanio_stack);

}

int solicitarMemoriaUMV(int pid, int tamanioScript, int tamanioIndiceCodigo, int tamanioIndiceEtiquetas, t_pcb* pcb){

	t_struct_numero* numpid = malloc(sizeof(t_struct_numero));
	numpid->numero=pid;
	socket_enviar(sock_umv, D_STRUCT_NUMERO, numpid);

	t_struct_memoria* mem = malloc(sizeof(t_struct_memoria));
	mem->tamanioScript=tamanioScript;
	mem->tamanioIndiceCodigo = tamanioIndiceCodigo;
	mem->tamanioIndiceEtiquetas = tamanioIndiceEtiquetas;
	socket_enviar(sock_umv, D_STRUCT_SOLICITARMEMORIA, mem);
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sock_umv,&tipoRecibido,&structRecibida);
	if(tipoRecibido==D_STRUCT_SF){
		t_struct_numero* sig = ((t_struct_numero*)structRecibida);
		free(sig);
		return -1;
	}
	if(tipoRecibido==D_STRUCT_BASES){
		t_struct_bases* base = ((t_struct_bases*)structRecibida);
		pcb->stack=base->stack;
		pcb->codigo=base->codigo;
		pcb->index_codigo=base->indice_codigo;
		pcb->index_etiquetas=base->indice_etiquetas;
		free(base);
		return 0;
	}
	return -1;

}


/*
 * Nombre: crearPcb/3
 * Argumentos:
 * 		- codigo
 * 		- metadata del programa
 * 		- fd del programa
 * Devuelve:
 * 		- null si no hay espacio suficiente para los segmentos de codigo
 *		- la pcb creada si hay espacio
 * Funcion:
 */
t_pcb* crearPcb(char* codigo, t_medatada_program* metadata_programa) {
	t_pcb* nuevoPCB=malloc(sizeof(t_pcb));
	pthread_mutex_lock(mutex_solicitarMemoria);
	pthread_mutex_lock(mutex_pid);
	nuevoPCB->pid=program_pid;


	int tamanioScript = strlen(codigo)+1; //DE ESTO NO ESTOY SEGURO
	int tamanioIndiceCodigo = (metadata_programa->instrucciones_size)*8;
	int tamanioIndiceEtiquetas = metadata_programa->etiquetas_size;
	if((solicitarMemoriaUMV(nuevoPCB->pid,tamanioScript,tamanioIndiceCodigo,tamanioIndiceEtiquetas,nuevoPCB))==0){ 	//Se fija si hay memoria suficiente para los 4 segmentos de codigo
		// enviarBytes

		t_struct_segmento* paquete = malloc(sizeof(t_struct_segmento));

		t_tipoEstructura tipoRecibido;
		void* structRecibida;

		paquete->base=nuevoPCB->codigo;
		paquete->tamanio=tamanioScript;
		paquete->segmento=(void*)codigo;
		socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);

		socket_recibir(sock_umv, &tipoRecibido, &structRecibida);
		free(structRecibida);
		paquete->base=nuevoPCB->index_codigo;
		paquete->tamanio=tamanioIndiceCodigo;
		paquete->segmento=((void*)metadata_programa->instrucciones_serializado);
		t_intructions* aux=malloc(sizeof(t_intructions));
		memcpy(aux,((t_intructions*)paquete->segmento), sizeof(t_intructions));
		socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);

		socket_recibir(sock_umv, &tipoRecibido, &structRecibida);
		free(structRecibida);

		paquete->base=nuevoPCB->index_etiquetas;
		paquete->tamanio=tamanioIndiceEtiquetas;
		paquete->segmento=((void*)metadata_programa->etiquetas);
		socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);
		free(paquete);

		socket_recibir(sock_umv, &tipoRecibido, &structRecibida);
		free(structRecibida);

		nuevoPCB->program_counter=metadata_programa->instruccion_inicio;
		nuevoPCB->tamanio_contexto=0;
		nuevoPCB->tamanio_indice=tamanioIndiceEtiquetas;
		nuevoPCB->c_stack=0;
		pthread_mutex_unlock(mutex_solicitarMemoria);
		program_pid+=1;
		pthread_mutex_unlock(mutex_pid);
	}else{	//Si no hay memoria suficiente, le avisa al programa
		pthread_mutex_unlock(mutex_solicitarMemoria);

		pthread_mutex_unlock(mutex_pid);
		free(nuevoPCB);
		return 0;
	}
	metadata_destruir(metadata_programa);
	return nuevoPCB;
}





void enviar_pcb_a_cpu(void){
	pthread_mutex_lock(mutex_array);
	int pos = buscar_cpu_libre();
	pthread_mutex_lock(mutex_cola_ready);
	printf("-----> SE ENVIA PCB A CPU %d\n",pos);
	t_struct_descriptor_cpu* cpu = (t_struct_descriptor_cpu*)list_get(cpus, pos);
	cpu->estado=USADA;
	t_struct_pcb* paquete = malloc(sizeof(t_struct_pcb));
	t_programa* programa = (t_programa*)list_remove(cola.ready,0);
	cpu->id=programa->pcb->pid;
	pthread_mutex_unlock(mutex_cola_ready);
	paquete->c_stack=programa->pcb->c_stack;
	paquete->codigo=programa->pcb->codigo;
	paquete->index_codigo=programa->pcb->index_codigo;
	paquete->index_etiquetas=programa->pcb->index_etiquetas;
	paquete->pid=programa->pcb->pid;
	paquete->program_counter=programa->pcb->program_counter;
	paquete->stack=programa->pcb->stack;
	paquete->tamanio_contexto=programa->pcb->tamanio_contexto;
	paquete->tamanio_indice=programa->pcb->tamanio_indice;
	escribir_log(archLog,"Se armo un pcb y se envia a cpu",INFO,"");
	int i = socket_enviar(cpu->socketCPU,D_STRUCT_PCB,paquete);
	if(i==1){
		pthread_mutex_lock(mutex_cola_exec);
		list_add(cola.exec,programa);
		pthread_mutex_unlock(mutex_cola_exec);
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "CPU", INFO, "Se envio el programa %d a la CPU %d", programa->pcb->pid, pos+1);
		pthread_mutex_unlock(mutex_log);
		free(paquete);

	}else{
		free(paquete);
		log_escribir(archLog, "CPU", INFO, "Se envio mal");
		perror("Se mando mal la pcb");
	}
	pthread_mutex_unlock(mutex_array);
	return;
}



void core_plp(void){

	t_programa* programa;

	pthread_create(&conexion_plp_programas, NULL, (void*) &core_conexion_plp_programas, NULL);

	while(1){
		sem_wait(&sem_new);
		sem_wait(&sem_multiProg);


		pthread_mutex_lock(mutex_cola_new);
		pthread_mutex_lock(mutex_cola_ready);

		programa = (t_programa*)list_remove(cola.new,0);

		pthread_mutex_unlock(mutex_cola_new);
		list_add(cola.ready, (void*) programa);
		pthread_mutex_unlock(mutex_cola_ready);
		sem_post(&sem_ready);


	}

	pthread_join(conexion_plp_programas,NULL);



	return;
}

void core_pcp(void){

	pthread_create(&conexion_plp_cpu, NULL, (void*) &core_conexion_pcp_cpu, NULL);


	while(1){
		sem_wait(&sem_ready);
		sem_wait(&sem_cpu);
		pthread_mutex_lock(mutex_cola_ready);
		mostrarColasPorPantalla(cola.ready, "Ready");
		pthread_mutex_unlock(mutex_cola_ready);
		if(list_size(cola.ready)!=0){

			enviar_pcb_a_cpu();
			pthread_mutex_lock(mutex_cola_exec);
			mostrarColasPorPantalla(cola.exec, "Exec");
			pthread_mutex_unlock(mutex_cola_exec);
		}else{
			printf("No quedan más programas en la cola Ready\n");
		}



	}

	pthread_join(conexion_plp_cpu,NULL);


}




void core_io(t_struct_pcb_io* bloqueo){
	pthread_detach(pthread_self());


	int i;


	for(i=0;configuracion_kernel.hio.id[i]!=NULL; i++){
		if((strcmp(bloqueo->dispositivo,configuracion_kernel.hio.id[i]))==0){
			usleep((bloqueo->tiempo)*configuracion_kernel.hio.retardo[i]);
			break;
		}
	}

		t_programa* programa = buscarPrograma(bloqueo->pid,cola.block.io,mutex_cola_block_io);
		mandarAOtraCola(programa, cola.block.io, mutex_cola_block_io, cola.ready, mutex_cola_ready);
		sem_post(&sem_ready);

		free(bloqueo);

	return;
}

/************************* HILOS DE CONEXIONES *************************/

void core_conexion_plp_programas(void){

	sock_programas=socket_crearServidor("127.0.0.1",configuracion_kernel.puerto_programas);
	efd_programas = epoll_crear();
	epoll_agregarSocketCliente(efd_programas,sock_programas);
	while(1){
		int i = epoll_escucharGeneral(efd_programas,sock_programas,(void*) &manejar_ConexionNueva_Programas, NULL, NULL);
		if(i==-1){
			escribir_log(archLog, "Epoll", ERROR, "Error al recibir evento");
		}
	}
	return;
}

void core_conexion_umv(void){
	if ((sock_umv=socket_crearYConectarCliente(configuracion_kernel.ip_umv, configuracion_kernel.puerto_umv))>0){
		escribir_log(archLog, "Conexion", INFO, "Se conectó a la UMV");
	}else{
		escribir_log(archLog, "Conexion", ERROR, "No se pudo conectar a la UMV");
		//abort();
	}
	t_struct_numero* num = malloc(sizeof(t_struct_numero));
	num->numero=0;
	socket_enviar(sock_umv,D_STRUCT_NUMERO,num);
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sock_umv, &tipoRecibido, &structRecibida);
	if(tipoRecibido==D_STRUCT_NUMERO){
		num->numero=configuracion_kernel.tamanio_stack;
		socket_enviar(sock_umv,D_STRUCT_NUMERO,num);
	}
	free(num);



	return;
}

void core_conexion_pcp_cpu(void){

	int i;
	pthread_mutex_lock(mutex_array);
	cpus = list_create();
	pthread_mutex_unlock(mutex_array);
	sock_cpu=socket_crearServidor("127.0.0.1", configuracion_kernel.puerto_cpus);
	efd_cpu=epoll_crear();
	epoll_agregarSocketCliente(efd_cpu,sock_cpu);

	while(1){
		i = epoll_escucharGeneral(efd_cpu,sock_cpu,(void*) &manejar_ConexionNueva_CPU, (void*)&handler_conexion_cpu, (void*) &desconexion_cpu);
		if(i==-1){
			escribir_log(archLog, "Epoll", ERROR, "Error al recibir evento");
		}
	}
	free(fds_conectados_cpu);
	free(estado_cpu);

	return;
}




void esperarYCerrarConexiones(void){
	pthread_join(conexion_plp_programas, NULL);
	pthread_join(conexion_plp_umv, NULL);
	pthread_join(conexion_plp_cpu, NULL);
	socket_cerrarConexion(sock_programas);
	socket_cerrarConexion(sock_umv);
	socket_cerrarConexion(sock_cpu);
	return;
}



