#include "funciones_PLP.h"

pthread_t hilo_pcp_new, hilo_pcp_ready;

int calcularPeso(t_programa* programa){ //Calcula peso del programa
	int peso=(5*programa->metadata->cantidad_de_etiquetas + 3* programa->metadata->cantidad_de_funciones + programa->metadata->instrucciones_size);

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
	configuracion_kernel.semaforos.id = config_get_array_value(config,"Lista de nombres de Semaforos");
	configuracion_kernel.semaforos.valor =vector_num(config_get_array_value(config,"Lista de valores de Semaforos"),configuracion_kernel.semaforos.id);
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

	free(config);
}

void imprimirConfiguracion() { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Puerto programas: %d\n", configuracion_kernel.puerto_programas);
	printf("Puerto CPUs: %d\n", configuracion_kernel.puerto_cpus);
	printf("Quantum: %d\n", configuracion_kernel.quantum);
	printf("Retardo quantum: %lu\n", configuracion_kernel.retardo_quantum);
	printf("Grado de multiprogramacion: %d\n", configuracion_kernel.multiprogramacion);


	int i;
	for(i=0;i<cant_identificadores(configuracion_kernel.semaforos.id);i++){
		printf("Semaforo (valor): %s (%d)\n",configuracion_kernel.semaforos.id[i], configuracion_kernel.semaforos.valor[i]);
	}

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
		// enviarBytes()

		t_struct_segmento* paquete = malloc(sizeof(t_struct_segmento));

		paquete->base=nuevoPCB->codigo;
		paquete->tamanio=tamanioScript;
		paquete->segmento=(void*)codigo;
		socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);

		paquete->base=nuevoPCB->index_codigo;
		paquete->tamanio=tamanioIndiceCodigo;
		paquete->segmento=((void*)metadata_programa->instrucciones_serializado);
		socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);

		paquete->base=nuevoPCB->index_etiquetas;
		paquete->tamanio=tamanioIndiceEtiquetas;
		paquete->segmento=((void*)metadata_programa->etiquetas);
		//socket_enviar(sock_umv,D_STRUCT_ESCRIBIRSEGMENTO, paquete);
		free(paquete);

		nuevoPCB->program_counter=metadata_programa->instruccion_inicio;
		nuevoPCB->tamanio_contexto=0;
		nuevoPCB->tamanio_indice=0;
		pthread_mutex_unlock(mutex_solicitarMemoria);
		program_pid+=1;
		pthread_mutex_unlock(mutex_pid);
	}else{	//Si no hay memoria suficiente, le avisa al programa
		pthread_mutex_unlock(mutex_solicitarMemoria);


		pthread_mutex_unlock(mutex_pid);
		free(nuevoPCB);
		return 0;
	}
	/*Esto es lo falta cargarle al PCB
	nuevoPCB.codigo;			//Dirección del primer byte en la UMV del segmento de código
	nuevoPCB.stack;				//Dirección del primer byte en la UMV del segmento de stack
	nuevoPCB.c_stack;			//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
	nuevoPCB.index_codigo;		//Dirección del primer byte en la UMV del Índice de Código
	nuevoPCB.index_etiquetas;	//Dirección del primer byte en la UMV del Índice de Etiquetas
	nuevoPCB.tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
	nuevoPCB.tamanio_indice;
	*/

	/*Esto es lo que devuelve el parser
	*pcbAux.instrucciones_size;				// Cantidad de instrucciones
	*pcbAux.instrucciones_serializado; 		// Instrucciones del programa
	*pcbAux.etiquetas_size;					// Tamaño del mapa serializado de etiquetas
	*pcbAux.etiquetas;						// La serializacion de las etiquetas
	*pcbAux.cantidad_de_funciones;
	*pcbAux.cantidad_de_etiquetas;
	*/
	//Creo que aca podria venir el solicitarMemoria. No use el tamanioIndEti ni tamanioIndCod que son cosas que
	//me da el parser supuestamente, y podrian servir para solicitar la memoria. No se

	return nuevoPCB;
}

void enviar_pcb_a_cpu(void){
	pthread_mutex_lock(mutex_array);
	int pos = buscar_cpu_libre();
	int fd_cpu_libre = fds_conectados_cpu[pos];
	t_struct_pcb* paquete = malloc(sizeof(t_struct_pcb));
	pthread_mutex_lock(mutex_cola_ready);
	t_programa* programa = (t_programa*)list_remove(cola.ready,0);
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
	int i = socket_enviar(fd_cpu_libre,D_STRUCT_PCB,paquete);
	if(i==1){
		estado_cpu[pos]=USADA;
		pthread_mutex_lock(mutex_cola_exec);
		list_add(cola.exec,programa);
		pthread_mutex_unlock(mutex_cola_exec);
		free(paquete);

	}else{
		free(paquete);
		enviar_pcb_a_cpu();
	}
	pthread_mutex_unlock(mutex_array);
	return;
}



void core_plp(void){

	t_programa* programa;

	pthread_create(&conexion_plp_programas, NULL, (void*) &core_conexion_plp_programas, NULL);

	while(1){
		sem_wait(&sem_new);
		pthread_mutex_lock(mutex_cola_new);
		mostrarColasPorPantalla(cola.new,"New");
		pthread_mutex_unlock(mutex_cola_new);

		if(configuracion_kernel.multiprogramacion > cantidadProgramasEnPCP()){
			pthread_mutex_lock(mutex_cola_new);
			pthread_mutex_lock(mutex_cola_ready);

			programa = (t_programa*)list_remove(cola.new,0);

			pthread_mutex_unlock(mutex_cola_new);
			list_add(cola.ready, (void*) programa);
			mostrarColasPorPantalla(cola.ready,"Ready");
			pthread_mutex_unlock(mutex_cola_ready);
			sem_post(&sem_multiProg);

		}


	}

	pthread_join(conexion_plp_programas,NULL);



	return;
}

void core_pcp(void){

	pthread_create(&conexion_plp_cpu, NULL, (void*) &core_conexion_pcp_cpu, NULL);


	while(1){
		sem_wait(&sem_multiProg);
		sem_wait(&sem_cpu);

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
			sleep((bloqueo->tiempo)*configuracion_kernel.hio.retardo[i]*0.001);
			break;
		}
	}
		t_struct_pcb* pcb = malloc(sizeof(t_struct_pcb));
		pcb->c_stack=bloqueo->c_stack;
		pcb->codigo=bloqueo->codigo;
		pcb->index_codigo=bloqueo->index_codigo;
		pcb->index_etiquetas=bloqueo->index_etiquetas;
		pcb->program_counter=bloqueo->program_counter;
		pcb->stack=bloqueo->stack;
		pcb->tamanio_contexto=bloqueo->tamanio_contexto;
		pcb->tamanio_indice=bloqueo->tamanio_indice;
		t_programa* programa = buscarPrograma(bloqueo->pid,cola.block.io,mutex_cola_block_io);
		actualizarPCB(programa, pcb);
		mandarAOtraCola(programa, cola.block.io, mutex_cola_block_io, cola.ready, mutex_cola_ready);
//		pthread_mutex_lock(mutex_cola_ready);
//		mostrarColasPorPantalla(cola.ready, "Ready");
//		pthread_mutex_unlock(mutex_cola_ready);
		sem_post(&sem_multiProg);

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
	fds_conectados_cpu = malloc(MAX_EVENTS_EPOLL*sizeof(int));
	estado_cpu=malloc(MAX_EVENTS_EPOLL*sizeof(int));
	for(i=0; i<MAX_EVENTS_EPOLL;i++){
		 estado_cpu[i]=LIBRE;
		 fds_conectados_cpu[i]=0;
	}
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



