/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include "cpu.h"

t_dictionary* diccionario;
int top_index = -1;


config_cpu configuracion_cpu;
char* PATH;
pthread_t conexion_kernel;
log_t* archLog;

int main (int argc, char **argv){
	PATH=argv[1];

	inicializarConfiguracion();

	//Cuando me conecto a UMV, que me diga si la pila esta vacia y me guardo su valor de top_index. Si esta vacia, es -1, sino lo que corresponda.
	//Con el top_index voy a poder calcular la posicion a pushear


	int thread_kernel = pthread_create(&conexion_kernel, NULL, core_conexion_kernel(), NULL);
	return 0;
}

/***************************************  FUNCIONES  ***************************************/


void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
			struct stat file_info;
			int control = lstat(PATH, &file_info);
			if (control == -1){
				log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
				}
			else{
				leerConfiguracion(PATH);
			}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_cpu.ip_kernel=config_get_int_value(config,"Direccion IP para conectarse al Kernel");
	configuracion_cpu.puerto_kernel=config_get_int_value(config,"Puerto TCP para conectarse al Kernel");
	configuracion_cpu.ip_umv=config_get_int_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_cpu.puerto_umv=config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
}

void log_error_socket(void){
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
}


void* core_conexion_kernel(void){
	int sock;		//El socket de conexion
	int n_sock;		//El socket de datos
	t_nipc* paquete;	//El paquete que recibe el socket

	if ((sock = nipc_abrirConexion(configuracion_cpu.puerto_kernel))<0){
		log_error_socket();
	}//El socket esta creado y listo para escuchar a los clientes por el puerto_programas

	while(1){
		printf("Esperando conexion de programas...\n");
		n_sock = nipc_aceptarConexion(sock);
		memset(paquete, 0, sizeof(paquete));
		if (nipc_recibir(n_sock,paquete)<0){
			//No se recibieron datos
		} else {
			//Se recibieron datos
			//Acá va la lógica de qué hacer cuando llega un programa nuevo
		}
		break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
	}
	//Esto nunca se ejecutaria, al salir del bucle deberia terminar el proceso. Esta para que el eclipse no se queje
	if (close(sock)<0){
		//Error con el close
	}

	return 0;
}
