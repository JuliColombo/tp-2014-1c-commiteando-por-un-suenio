/*
 * funciones_CPU.c
 *
 *  Created on: 13/06/2014
 *      Author: utnso
 */
#include "funciones_CPU.h"

config_cpu configuracion_cpu;


void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1){
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
	}
	else{
	leerConfiguracion(PATH);
	imprimirConfiguracion();
	}
}

void leerConfiguracion(void){
	config=config_create(PATH);

	configuracion_cpu.ip_kernel=config_get_string_value(config,"Direccion IP para conectarse al Kernel");
	configuracion_cpu.puerto_kernel=config_get_int_value(config,"Puerto TCP para conectarse al Kernel");
	configuracion_cpu.ip_umv=config_get_string_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_cpu.puerto_umv=config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
}

void imprimirConfiguracion(void){
	printf("Direccion IP para conectarse al Kernel: %s\n", configuracion_cpu.ip_kernel);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_cpu.puerto_kernel);
	printf("Direccion IP para conectarse a la UMV: %s\n", configuracion_cpu.ip_umv);
	printf("Puerto para conexiones con UMV: %d\n", configuracion_cpu.puerto_umv);
}

void log_error_socket(void){
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
}

/******************************** CONEXION KERNEL ***************************************************/


uint32_t temp_id;
uint32_t temp_ind_codigo;
uint32_t temp_counter;
uint32_t temp_seg_codigo;
uint32_t temp_cursor_stack;
uint32_t var_ind_etiquetas;
uint32_t var_tamanio_etiquetas;
uint32_t temp_cursor_ant;
uint32_t var_seg_stack;
uint32_t var_tamanio_contexto;
int quantum;
int fin_quantum;
int sockKernel;

void core_conexion_kernel(void){
	//int sock;
	if((sockKernel=socket_crearYConectarCliente(configuracion_cpu.ip_kernel,configuracion_cpu.puerto_kernel))==-1){
		log_error_socket();
		//abort();
	}
	log_escribir(archLog, "Conexion", INFO, "Se conecto correctamente al Kernel");

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
	t_struct_numero* k = ((t_struct_numero*)structRecibida);
	quantum = k->numero;
	free(k);

	int fin_PCB;
	int sig_flag;
	int UMV_flag;
	int SEG_flag;
	int id;
	t_dictionary* dicc_variables =dictionary_create();

	t_struct_pcb* pcb_recibida;
	t_struct_pcb_io* pcb_actualizada;
	sem_wait(&sem_kernel);

	while(1){
		sleep(retardo);
		fin_PCB = 0;
		sig_flag = 0;
		UMV_flag = 0;
		SEG_flag = 0;
		socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
			//habria que hacer el free de este pcb cuando se lo mando al kernel
			pcb_recibida = ((t_struct_pcb*)structRecibida);
			log_escribir(archLog, "Recibida pcb", INFO, "Se recibio la pcb con pid: %d", pcb_recibida->pid);


			temp_id=pcb_recibida->pid;
			log_escribir(archLog, "PCB", INFO, "Se recibio el pid %d", temp_id);
			temp_cursor_stack= pcb_recibida->c_stack;
			temp_seg_codigo = pcb_recibida->codigo;
			temp_ind_codigo = pcb_recibida->index_codigo;
			var_ind_etiquetas = pcb_recibida->index_etiquetas;
			temp_counter = pcb_recibida->program_counter;
			var_seg_stack = pcb_recibida->stack;
			var_tamanio_contexto = pcb_recibida->tamanio_contexto;
			var_tamanio_etiquetas = pcb_recibida->tamanio_indice;
			recupero_diccionario(dicc_variables,var_tamanio_contexto);
			darValoresDeStackYCursor(pcb);

			while(fin_PCB==0){

			}



			if(fin_PCB==0){
				t_struct_pcb_fin* pcb_fin = malloc(sizeof(t_struct_pcb_fin));
				pcb_fin->c_stack=pcb->c_stack;
				pcb_fin->codigo=pcb->codigo;
				pcb_fin->index_codigo=pcb->index_codigo;
				pcb_fin->index_etiquetas=pcb->index_etiquetas;
				pcb_fin->pid=pcb->pid;
				pcb_fin->program_counter=pcb->program_counter;
				pcb_fin->stack=pcb->stack;
				pcb_fin->tamanio_contexto=pcb->tamanio_contexto;
				pcb_fin->tamanio_indice=pcb->tamanio_contexto;


				socket_enviar(sockKernel, D_STRUCT_PCBFIN, pcb_fin);
			}

	}


	if(socket_cerrarConexion(sockKernel)==-1){
		log_escribir(archLog,"Cerrar Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return;
}

void recupero_diccionario(t_dictionary* diccionario, int tamanio_contexto) {
	dictionary_clean(diccionario);  // debemos limpiar el diccionario y rellenarlo con las variables del contexto anterior

	int cant_vars = tamanio_contexto;
	int i = 0;
	int incremento = 0;
	int posicion = 1;
	int offset;

	while (i < cant_vars) {		// con esto volvemos a llenar el dicc_variables con las variables del contexto anterior

		int valor_var;
		char nombre_var;

		t_struct_sol_bytes * sol_var_ant = malloc(sizeof(t_struct_sol_bytes));

		sol_var_ant->base = var_seg_stack;
		sol_var_ant->offset = temp_cursor_stack + incremento;
		sol_var_ant->tamanio = sizeof(char) + sizeof(int);
		socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, sol_var_ant);

		void * structRecibido2;
		t_tipoEstructura tipoStruct2;

		socket_recibir(sockUMV, &tipoStruct2, &structRecibido2);

		memcpy(&nombre_var, ((t_struct_respuesta_umv*) structRecibido2)->buffer, offset = sizeof(char));
		memcpy(&valor_var, ((t_struct_respuesta_umv*) structRecibido2)->buffer + offset, sizeof(int));		// no es necesario este valor

		char key = nombre_var;
		char keyABuscar[2] = { key, '\0' };

		int* posicion_variable = malloc(sizeof(int));
		*posicion_variable = posicion;

		dictionary_put(diccionario, keyABuscar, posicion_variable);

		incremento = incremento + (sol_var_ant->tamanio);
		i++;
		posicion++;
		free(sol_var_ant);
		free(((t_struct_respuesta_umv*) structRecibido2)->buffer);
		free(structRecibido2);
	}
}






/******************************** CONEXION UMV ***************************************************/

void core_conexion_umv(void){
	//int sock;
	if ((sockUMV=socket_crearYConectarCliente(configuracion_cpu.ip_umv, configuracion_cpu.puerto_umv))>0){
		log_escribir(archLog, "Conexion", INFO, "Se conecto correctamente a UMV");
	} else {
		log_error_socket();
		abort();
	}

	t_struct_numero* num = malloc(sizeof(t_struct_numero));
	num->numero=1;
	socket_enviar(sockUMV, D_STRUCT_NUMERO, num);
	free(num);
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sockUMV, &tipoRecibido, structRecibida);
	if(tipoRecibido==D_STRUCT_NUMERO){
		log_escribir(archLog, "Conexion", INFO, "La UMV reconoció la CPU");
		sem_post(&sem_kernel);
	}


	//ESTO LO USO DE PRUEBA. TENGO QUE USAR EL PCB
	uint32_t a= 5;
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->index_etiquetas = &a;
	pcb->tamanio_indice = 3;

	//ENVIO PEDIDO DE INDICE DE ETIQUETAS
	t_struct_indice_etiquetas* estructura = malloc(sizeof(t_struct_indice_etiquetas));
	estructura->index_etiquetas = pcb->index_etiquetas;
	estructura->etiquetas_size = pcb->tamanio_indice;
	int k=socket_enviar(sockUMV,D_STRUCT_INDICE_ETIQUETAS,estructura);
	if(k==1){
		printf("se le mando pedido de indice de etiquetas a umv \n");
	}

	free(estructura);
	free(pcb);

	//RECIBO INDICE
	//t_tipoEstructura tipoRecibido;
		//void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_string* k = ((t_struct_string*)structRecibida);
			indiceEtiquetas= k->string;
			printf("me llego %s\n",indiceEtiquetas);
			free(k);}

	while(1){



	}

	if(socket_cerrarConexion(sockUMV)==-1){
		log_escribir(archLog,"Cerrar Conexion",ERROR,"No se pudo conectar a la UMV");
	}
}

