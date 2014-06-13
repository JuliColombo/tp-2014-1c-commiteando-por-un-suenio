#include "funciones_PLP.h"


void calcularPeso(t_programa programa){ //Calcula peso del programa
	programa.peso=(5*programa.metadata.cantidad_de_etiquetas + 3* programa.metadata.cantidad_de_funciones + programa.metadata.instrucciones_size);
}

void agregarAColaSegunPeso(t_programa programa, t_list* lista){
	t_link_element *element = lista->head;
	int position = 0;

	while ((element != NULL)&&(((t_programa*)(element->data))->peso < programa.peso) ) {
		element = element->next;
		position++;
	}

	list_add_in_index(lista, position, &programa);
}

void mostrarNodosPorPantalla(t_list* lista){
	int i;
	int p;
	//system("clear");
	if(lista->head==NULL){
		printf("No hay nodos en la cola\n");
		return;
	}
	printf("Estado de la cola:\n");
	for (i=0;i<(list_size(lista));i++){
		t_programa* aux=(t_programa*)list_get(lista, i);
		p=aux->peso;
		printf(" Nodo %d   Peso %d\n", i, p); //Tal vez agregar el nombre del programa(? Preguntar
	}
}

int cantidadProgramasEnEjecucion(void){
	return (list_size(cola.ready)+list_size(cola.exec)+list_size(cola.block));
}

void completarGradoMultip(void){
	if(cantidadProgramasEnEjecucion()<configuracion_kernel.multiprogramacion){
		pthread_mutex_lock(mutex_cola_new);
		void* aux=list_remove(cola.new,0);
		pthread_mutex_unlock(mutex_cola_new);
		while(cantidadProgramasEnEjecucion()<configuracion_kernel.multiprogramacion && aux!=NULL){
			pthread_mutex_lock(mutex_cola_ready);
			list_add(cola.ready,aux);
			pthread_mutex_unlock(mutex_cola_ready);
			if(cantidadProgramasEnEjecucion()<configuracion_kernel.multiprogramacion){
				pthread_mutex_lock(mutex_cola_new);
				aux=list_remove(cola.new,0);
				pthread_mutex_unlock(mutex_cola_new);
			}
		}
	}
}

void inicializarColas(void){
	cola.new=list_create();
	cola.ready=list_create();
	cola.exec=list_create();
	cola.block=list_create();
	cola.exit=list_create();
}

void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1){
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		}
	else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_kernel.puerto_programas = config_get_int_value(config,"Puerto TCP para recibir conexiones de los Programas");
	configuracion_kernel.puerto_cpus = config_get_int_value(config,"Puerto TCP para recibir conexiones de los CPUs");
	configuracion_kernel.quantum = config_get_int_value(config,"Quantum de tiempo para algoritmos expropiativos");
	configuracion_kernel.retardo_quantum = config_get_int_value(config,"Retardo del Quantum");
	configuracion_kernel.multiprogramacion = config_get_int_value(config,"Maximo nivel de multiprogramacion");
	configuracion_kernel.id_semaforos = config_get_array_value(config,"Lista de nombres de Semaforos");
	configuracion_kernel.valor_semaforos=vector_num(config_get_array_value(config,"Lista de valores de Semaforos"),configuracion_kernel.id_semaforos);
	configuracion_kernel.id_hio = config_get_array_value(config,"Lista de hio");
	configuracion_kernel.retardo_hio = vector_num(config_get_array_value(config,"Retardo de hio"),configuracion_kernel.id_hio);
	configuracion_kernel.ip_umv = config_get_string_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_kernel.puerto_umv = config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
	configuracion_kernel.var_globales = config_get_array_value(config,"Variables globales");
	valor_var_globales = vector_num(NULL, configuracion_kernel.var_globales);
	configuracion_kernel.tamanio_stack = config_get_int_value(config,"Tamanio del Stack");

	}

void imprimirConfiguracion() { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Puerto programas: %d\n", configuracion_kernel.puerto_programas);
	printf("Puerto CPUs: %d\n", configuracion_kernel.puerto_cpus);
	printf("Quantum: %d\n", configuracion_kernel.quantum);
	printf("Retardo quantum: %d\n", configuracion_kernel.retardo_quantum);
	printf("Grado de multiprogramacion: %d\n", configuracion_kernel.multiprogramacion);


	int i;
	for(i=0;i<cant_identificadores(configuracion_kernel.id_semaforos);i++){
		printf("Semaforo (valor): %s (%d)\n",configuracion_kernel.id_semaforos[i], configuracion_kernel.valor_semaforos[i]);
	}

	free(configuracion_kernel.valor_semaforos);
	for(i=0;configuracion_kernel.id_hio[i]!=NULL;i++){
		printf("ID HIO (retardo): %s ", configuracion_kernel.id_hio[i]);
		printf("(%d)\n", configuracion_kernel.retardo_hio[i]);
	}
	free(configuracion_kernel.retardo_hio);
	printf("IP de la UMV: %s\n", configuracion_kernel.ip_umv);
	printf("Puerto UMV: %d\n", configuracion_kernel.puerto_umv);
	
	printf("Variables globales: ");
	for(i=0;configuracion_kernel.var_globales[i]!=NULL;i++){
		printf("\n%s (%d) ", configuracion_kernel.var_globales[i], valor_var_globales[i]);
	}


	printf("\nTamaño del Stack: %d\n", configuracion_kernel.tamanio_stack);

}




t_pcb crearPcb(char* codigo) {
	t_pcb nuevoPCB;
	t_medatada_program *pcbAux;
	pcbAux=metadatada_desde_literal(codigo);
	nuevoPCB.program_counter=pcbAux->instruccion_inicio;	//Seteamos el PC a la primera instruccion del parser
	nuevoPCB.pid=getpid();

	/*Esto es lo falta cargarle al PCB
	nuevoPCB.pid;				//Identificador único del Programa en el sistema
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




pthread_t conexion_plp_programas, conexion_plp_umv, conexion_plp_cpu;

void core_plp(void){

	pthread_create(&conexion_plp_programas, NULL, (void*) &core_conexion_plp_programas, NULL);
	pthread_create(&conexion_plp_umv, NULL, (void*) &core_conexion_umv, NULL);
	pthread_create(&conexion_plp_cpu, NULL, (void*) &core_conexion_pcp_cpu, NULL);
	//aca deberia llegar un programa nuevo a la cola de new e insertarlo segun peso --Segúin entiendo yo, el progarma entra en el thread de conexion_programas y ahi lo encolamos, o no?
	//deberia mandarlo para acá y que de ahí lo encole, no es responsabilidad de la conexion encolarlo, es que llegue nada más


	while (1){




	/*	completarGradoMultip();
		mostrarNodosPorPantalla(cola.new);


		t_programa programa; //Este programa llega por los sockets
		t_pcb pcb=crearPcb("Aca va el codigo fuente");
		programa.pcb = &pcb;
		programa.flag_terminado=0;
		calcularPeso(programa);
		agregarAColaSegunPeso(programa, cola.new);


		while(1){
			pthread_mutex_lock(mutex_cola_ready);
			t_programa* programa = list_remove(cola.ready,0);
			pthread_mutex_unlock(mutex_cola_ready);
		}*/
	}






	return;
}


void core_conexion_plp_programas(void){

	struct epoll_event event;
	struct epoll_event* events;

	fds_conectados = malloc(MAX_EVENTS_EPOLL*sizeof(int));
	sock_programas=socket_crearServidor("127.0.0.1",configuracion_kernel.puerto_programas);
	int efd_programas = epoll_crear();
	epoll_agregarSocketServidor(efd_programas,sock_programas);
	event.events=EPOLLIN|EPOLLRDHUP;
	events=calloc(MAX_EVENTS_EPOLL,sizeof(event));
	event.data.fd=sock_programas;

	int i = epoll_escucharGeneral(efd_programas,sock_programas, manejar_ConexionNueva_Programas(event), NULL, NULL);
	printf("epoll programas = %d\n", i);









	return;
}

void core_conexion_umv(void){
	if ((sock_umv=socket_crearYConectarCliente(configuracion_kernel.ip_umv, configuracion_kernel.puerto_umv))>0){
		printf("Conectado a la UMV\n");
	}


	return;
}

void core_conexion_pcp_cpu(void){

	struct epoll_event event;
	struct epoll_event* events;


	sock_cpu=socket_crearServidor("127.0.0.1", configuracion_kernel.puerto_cpus);
	int efd_cpu=epoll_crear();
	epoll_agregarSocketServidor(efd_cpu,sock_cpu);
	event.events=EPOLLIN | EPOLLRDHUP;
	events=calloc(MAX_EVENTS_EPOLL,sizeof(event));
	int i = epoll_escucharBloqueante(efd_cpu,events);
	printf("epoll cpu = %d \n", i);



	return;
}

void core_pcp(void){



/*
	while(1){





			while(programa.flag_terminado==0){


			//Acá manda el programa al cpu los quantums que le correspondan, si termina antes de que termine el quantum se devuelve y asigna con cuánto terminó
				pthread_mutex_lock(mutex_cola_exec);
				list_add(cola.exec,programa); //Agrego el programa a la lista exec porque está en la cpu, cuando vuelva se vé si vuelve a ready o pasa a block
				pthread_mutex_unlock(mutex_cola_exec);

			//Aca deberia esperar a que la cpu lo devuelva, de todas formas no estoy seguro
				if(programa.flag_bloqueado==0){
				//Sacar al programa por el pid de la cola exec y ponerlo en ready
				}else{
					pthread_mutex_lock(mutex_cola_block);
					list_add(cola.block,programa);
					pthread_mutex_unlock(mutex_cola_block);
				}


			}


			if(programa.flag_terminado==1){ //Esto va al final,
				pthread_mutex_lock(mutex_cola_exec);
				pthread_mutex_lock(mutex_cola_exit);
				list_add(cola.exit,list_remove(cola.exec,0)); //Hay que usar remove_by_condition y preguntar por el flag_terminado
				pthread_mutex_unlock(mutex_cola_exit);
				pthread_mutex_unlock(mutex_cola_exec);

			}
	}


*/
	return;
}


void core_io(int retardo){

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
