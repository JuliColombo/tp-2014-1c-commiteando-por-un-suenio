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
		free(k);
	}
	j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		retardo = k->numero;
		free(k);
	}
	t_struct_pcb* pcb_recibida;
	t_struct_pcb_io* pcb_actualizada;
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
				sleep(3);
				int tam = sizeof(t_struct_pcb_io);//+strlen(str)+1;//+sizeof(int);
				pcb_actualizada = malloc(tam);
				pcb_actualizada->pid=pcb->pid;
				pcb_actualizada->c_stack=pcb->c_stack;
				pcb_actualizada->codigo=pcb->codigo;
				pcb_actualizada->index_codigo=pcb->index_codigo;
				pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
				pcb_actualizada->program_counter=pcb->program_counter;
				pcb_actualizada->stack=pcb->stack;
				pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
				pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
				pcb_actualizada->dispositivo="HDD1";
				pcb_actualizada->tiempo=10;

				if(pcb_recibida->pid==0){

					int i = socket_enviar(sockKernel,D_STRUCT_PCBIO,pcb_actualizada);
					if(i==1){
						printf("Se mando bien la actualizada\n");
					}
				}else{
					socket_enviar(sockKernel,D_STRUCT_PCB,pcb_recibida);
				}
						free(pcb_actualizada);
						free(pcb);
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
	//log_escribir(archLog, "Conexion", INFO, "Se conecto correctamente a UMV");
	}
	//ESTO LO USO DE PRUEBA. TENGO QUE USAR EL PCB
	int a= 5;
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
	t_tipoEstructura tipoRecibido;
		void* structRecibida;
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
	return;
}

