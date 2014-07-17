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
	//t_struct_pcb* pcb_actualizada;
		//t_tipoEstructura tipoRecibido;
		//void* structRecibida;
	while(1){
			j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
			if(j==1){
				printf("MUNDO FELIZ\n");
				pcb_recibida = ((t_struct_pcb*)structRecibida);
				t_pcb* pcb = malloc(sizeof(t_pcb));
				pcb->pid=pcb_recibida->pid;

				printf("el pid es: %d\n", pcb->pid);

				/*pcb_actualizada->c_stack=pcb->c_stack;
				pcb_actualizada->codigo=pcb->codigo;
				pcb_actualizada->index_codigo=pcb->index_codigo;
				pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
				pcb_actualizada->pid=9;
				printf("El pid que se manda en la pcb es: %d", pcb_actualizada->pid);
				pcb_actualizada->program_counter=pcb->program_counter;
				pcb_actualizada->stack=pcb->stack;
				pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
				pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
				int i = socket_enviar(sock,D_STRUCT_PCB,pcb_actualizada);*/

			}
//			pcb->program_counter=pcb->program_counter+1;
//			pcb_actualizada=pcb_recibida;
//			pcb_actualizada->program_counter=(pcb_recibida->program_counter +1);
//			socket_enviar(sock,D_STRUCT_PCB,pcb_actualizada);







	}
	//CODEO ACA


	if(socket_cerrarConexion(sockKernel)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar al Kernel");
	}
	return;
}


/******************************** CONEXION UMV ***************************************************/

void core_conexion_umv(void){
	//int sock;
	if ((sockUMV=socket_crearYConectarCliente(configuracion_cpu.ip_umv, configuracion_cpu.puerto_umv))>0){
		log_error_socket();
		printf("Conectado a la UMV\n");
	}

	//CODEO ACA


	if(socket_cerrarConexion(sockUMV)==-1){
		log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar a la UMV");
	}
	return;
}

