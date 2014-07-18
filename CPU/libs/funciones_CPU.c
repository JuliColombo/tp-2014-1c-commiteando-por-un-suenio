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
	t_config* config=config_create(PATH);

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
void core_conexion_kernel(void){
	//int sock;
	if((sockKernel=socket_crearYConectarCliente(configuracion_cpu.ip_kernel,configuracion_cpu.puerto_kernel))==-1){
		log_error_socket();
		abort();
	}
	printf("se conecto al kernel\n");
	//log_escribir(archLog, "Conexion", INFO, "Se conecto correctamente al Kernel");

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		quantum = k->numero;
		printf("me llego quantum %d\n",quantum);
		free(k);
	}
	j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		retardo = k->numero;
		printf("me llego retardo %li\n",retardo);
		free(k);
	}
	t_struct_pcb* pcb_recibida;
	t_struct_pcb* pcb_actualizada;
	while(1){
			j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
			if(j==1){
				//habria que hacer el free de este pcb cuando se lo mando al kernel
				pcb_recibida = ((t_struct_pcb*)structRecibida);
				t_pcb* pcb = malloc(sizeof(t_pcb));
				pcb->pid=pcb_recibida->pid;
				pcb->c_stack = pcb_recibida->c_stack;
				pcb->codigo = pcb_recibida->codigo;
				pcb->index_codigo = pcb_recibida->index_codigo;
				pcb->index_etiquetas = pcb_recibida->index_etiquetas;
				pcb->program_counter = pcb_recibida->program_counter;
				pcb->stack = pcb_recibida->stack;
				pcb->tamanio_contexto = pcb_recibida->tamanio_contexto;
				pcb->tamanio_indice = pcb_recibida->tamanio_indice;

				darValoresDeStackYCursor(pcb);

				//Esto va a aca? O lo pongo en parserCPU cuando devuelvo el pcb?
				printf("el pid es: %d\n", pcb->pid);
				pcb_actualizada = malloc(sizeof(t_struct_pcb));
				pcb_actualizada->c_stack=pcb->c_stack;
				pcb_actualizada->codigo=pcb->codigo;
				pcb_actualizada->index_codigo=pcb->index_codigo;
				pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
				pcb_actualizada->program_counter=pcb->program_counter;
				pcb_actualizada->stack=pcb->stack;
				pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
				pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
				int i = socket_enviar(sockKernel,D_STRUCT_PCB,pcb_actualizada);
				if(i==1){
				printf("Se mando bien el paquete\n");
				free(pcb_actualizada);
				}
			}
//			pcb->program_counter=pcb->program_counter+1;
//			pcb_actualizada=pcb_recibida;
//			pcb_actualizada->program_counter=(pcb_recibida->program_counter +1);
//			socket_enviar(sock,D_STRUCT_PCB,pcb_actualizada);







	}
	//CODEO ACA


	if(socket_cerrarConexion(sockKernel)==-1){
		log_escribir(archLog,"Cerrar Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return;
}


/******************************** CONEXION UMV ***************************************************/

void core_conexion_umv(void){
	//int sock;
	if ((sockUMV=socket_crearYConectarCliente(configuracion_cpu.ip_umv, configuracion_cpu.puerto_umv))<0){
		log_error_socket();
	} else {
	printf("Conectado a la UMV\n");
	log_escribir(archLog, "Conexion", INFO, "Se conecto correctamente a UMV");
	}
	//CODEO ACA

	/*t_pcb* pcb_actualizada = malloc(sizeof(t_pcb));
	pcb_actualizada->index_etiquetas = &a;
	pcb_actualizada->tamanio_indice = 3;*/
	/*t_struct_indice_etiquetas* estructura = malloc(sizeof(t_struct_indice_etiquetas));
	estructura->index_etiquetas = pcb->index_etiquetas;
	estructura->etiquetas_size = pcb->tamanio_indice;
	socket_enviar(sockUMV,D_STRUCT_INDICE_ETIQUETAS,estructura);


	free(estructura);
	//free(pcb_actualizada);

	t_tipoEstructura tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_string* k = ((t_struct_string*)structRecibida);
			proximaInstruccion= k->string;
			free(k);}*/


	if(socket_cerrarConexion(sockUMV)==-1){
		log_escribir(archLog,"Cerrar Conexion",ERROR,"No se pudo conectar a la UMV");
	}
	return;
}

