/*
 * UMV.c
 *
 *  Created on: 21/04/2014
 *      Author: utnso
 */


#include "funciones_UMV.h"

//#include "FuncionesPLP.h"
//#include <Estructuras.h>





int estaEnDicOP(char palabra[]){
					char *dic_op[]={"operacion",
									  "dump",
									  "compactacion",
									  "retardo",
									  "algoritmo",
									  "exit"
					};
					int aux;
					for(aux=0;aux<6;aux++){
						if(strcmp(dic_op[aux], palabra) == 0){
							return 1;
							break;
						}

					}
					return 0;
}

int estaEnDicTOP(char palabra[]){
					char *dic_top[]={"solicitar",
							        "escribir",
							        "crear",
							        "destruir"
						};
					int aux;
					for(aux=0;aux<4;aux++){
						if((strcmp(dic_top[aux], palabra)) == 0){
							return 1;
							break;
						}
					}
					return 0;
}


int* crearMP(void) { // Cambie para que no reciba parametro, total la config es una variable externa -- Fede
	int tamanio = configuracion_UMV.memSize;
	int* MP;
	MP = malloc(tamanio);
	return MP;
}

_Bool segmentationFault(uint32_t base,uint32_t offset){
	if (MP[base+offset] == NULL) {
	    printf("Segmentation Fault al intentar acceder a posicion %d \n", base+offset);
		return true;
	} else{
		return false;
	}
}



_Bool memoryOverload(uint32_t base,uint32_t offset, uint32_t longitud){
	if (MP[base+offset+longitud] == NULL) {
		    printf("Memory Overload al intentar escribir %d bytes en la posicion %d \n", longitud,base+offset);
			return true;
		} else{
			return false;
		}
}

//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
_Bool solicitarMemoria(t_programa programa){
	return true;
}

//Operacion Basica de UMV 1, se toma una cantidad de bytes (longitud) desde la posicion de memoria dada(base+offset)?
void solicitarBytes(uint32_t base,uint32_t offset, uint32_t longitud){
//	if (validarSolicitud(base,offset,longitud)){
//		SOLICITAR LOS BYTES
//	} else {
//		puts("No se pudo realizar la asignacion");
//    }

}

//Operacion Basica de UMV 2, se envia una cantidad de bytes (en el buffer?) a la posicion dada(base+offset)
void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud/*,t_buffer buffer*/){

}

//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion
_Bool validarSolicitud(uint32_t base,uint32_t offset, uint32_t longitud){
	if(/*hayEspacioEnMemoria()*/1){
		return true;
	} else{
		puts("No alcanza el espacio en memoria:");
		if(segmentationFault(base,offset)){
			return false;
		} else { if(memoryOverload(base,offset,longitud)){
												return false;
					} else {
							//puts("Excepcion Desconocida"); ???
							return false;
					}
				}
			}
}

//Comandos de consola:
void operacion(/*t_proceso proceso,*/ uint32_t base,uint32_t offset, uint32_t longitud){
	//Solicitar una posicion de memoria o escribir un buffer por teclado en una posicion o crear segmentos de programa o destruirlos
	/*if ( esValido(buffer) ){
	 * void enviarBytes(base,offset,longitud,buffer)
	 * }else{
	 *  solicitarBytes(base,offset,longitud)
	 *  } */
	printf(" "/*posicionDeMemoria*/);
	/*OPCIONAL: grabar en archivo*/
}

void retardo(int valorRetardoEnMilisegundos){ //Cantidad de ms que debe esperar UMV para responder una solicitud

}

void algoritmo(int* algor){//Cambiar entre Worst fit y First fit
	if(*algor==worstfit){
							*algor=firstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}
	else{
							*algor=worstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}

}

void compactar(){
	//Forzar compactacion (ver para checkpoint 3)
}

void dump(){
	//obtenerDatosDeMemoria() y mostrar (y,opcional, guardar en archivo)
}


//Funciones para lectura del archivo config y una funcion que imprime dichos campos para testear la lectura

void leerConfiguracion(void) {

	t_config* config = config_create(PATH);

		configuracion_UMV.memSize = config_get_int_value(config,
					"MemSize");

		configuracion_UMV.puerto_cpus = config_get_int_value(config,
					"Puerto TCP para recibir conexiones de los CPUs");

		configuracion_UMV.puerto_kernel = config_get_int_value(config,
					"Puerto TCP para recibir conexiones del Kernel");


		configuracion_UMV.ip_kernel = config_get_int_value(config,
					"Direccion IP para conectarse al Kernel");


		configuracion_UMV.algoritmo = config_get_int_value(config,
					"Algoritmo de seleccion de ubicacion de segmento");

}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Tamanio de memoria Principal: %d\n", configuracion_UMV.memSize);
	printf("Puerto para conexiones con CPUs: %d\n", configuracion_UMV.puerto_cpus);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_UMV.puerto_kernel);
	printf("IP del Kernel: %d\n", configuracion_UMV.ip_kernel);
//	printf("%d\n", configuracion.id_semaforos);
//	printf("%d\n", configuracion.valor_semaforos);
	printf("Algoritmo de segmentacion: %d\n", configuracion_UMV.algoritmo);
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

//****************************************Atender Conexiones de Kernel/CPU*******************

void core_conexion_cpu(void){
	int sock_cpu;		//El socket de conexion
	int n_sock_cpu;		//El socket de datos
	t_nipc* paquete;	//El paquete que recibe el socket

	if ((sock_cpu = nipc_abrirConexion(configuracion_UMV.puerto_cpus))<0){
		if (close(sock_cpu)<0){
			//Error con el close
		}
		abort();
	}//El socket esta creado y listo para escuchar a los clientes por el puerto_cpus


	while(1){
			printf("Esperando conexion de CPU...\n");
			n_sock_cpu = nipc_aceptarConexion(sock_cpu);
			memset(paquete, 0, sizeof(paquete));
			if (nipc_recibir(n_sock_cpu,paquete)<0){
				//No se recibieron datos
			} else {
				//Se recibieron datos
			}
			break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
		}
}

void core_conexion_kernel(void){
	int sock_kernel;		//El socket de conexion
	int n_sock_kernel;		//El socket de datos
	t_nipc* paquete;	//El paquete que recibe el socket

	if ((sock_kernel = nipc_abrirConexion(configuracion_UMV.puerto_kernel))<0){
		if (close(sock_kernel)<0){
			//Error con el close
		}
		abort();
	}//El socket esta creado y listo para escuchar a los clientes por el puerto_cpus


	while(1){
			printf("Esperando conexion de Kernel...\n");
			n_sock_kernel = nipc_aceptarConexion(sock_kernel);
			memset(paquete, 0, sizeof(paquete));
			if (nipc_recibir(n_sock_kernel,paquete)<0){
				//No se recibieron datos
			} else {
				//Se recibieron datos
			}
			break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
		}
}
//***********************************************Inicializacion de semaforos************************************


void inicializarSemaforos(void){
	mutex=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex,NULL);
}

//***********************************************Inicializacion y espera de hilos************************************

void inicializarHilos(void){
	pthread_create(&CONSOLA, NULL, (void*) &core_consola, NULL);
	pthread_create(&KERNEL, NULL, (void*) &core_conexion_kernel, NULL);
	pthread_create(&CPU, NULL, (void*) &core_conexion_cpu, NULL);
}

void esperarHilos(void){
	pthread_join(CPU,NULL);
	pthread_join(CONSOLA,NULL);
	pthread_join(KERNEL,NULL);
}

//***********************************************Consola************************************

void *core_consola(void) {

	pthread_t inicio;
	pthread_create(&inicio, NULL, (void*) &consola, NULL);
	pthread_join(inicio,NULL);
	return EXIT_SUCCESS;
}

void *consola (void){

	//system("clear");
	char comando[32];
	puts("Ingrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	gets(comando);
	while(estaEnDicOP(comando)== 0){
		puts("\nOperacion erronea, escriba la operacion de nuevo");
		gets(comando);
	}
	while(strcmp(comando, "exit") != 0){
			if(strcmp(comando, "operacion") == 0){
				char tipoOperacion[32];
				puts("\nDesea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?");
				gets(tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0/*<- aca no iria un 1?*/){
						puts("\nTipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						gets(tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					//solicitarPosicionDeMemoria();
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					//escribirBuffer();
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "crear") == 0){
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					//crearSegmentoPrograma();
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "destruir") == 0){
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					//destruirSegmentoPrograma(t_programa Programa);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
			}

		else { if (strcmp(comando, "retardo") == 0){
				  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				//retardo(int valorRetardoEnMilisegundos);
				  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando, "algoritmo") == 0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				   algoritmo(&algor);
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando, "compactacion") == 0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				 //compactar();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando,"dump") ==0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				 //generarReporte();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			}

		puts("\nEscriba la siguiente operacion");
		gets(comando);
		while(estaEnDicOP(comando)== 0){
				puts("\nOperacion erronea, escriba la operacion de nuevo");
				gets(comando);
			}
		}

	return EXIT_SUCCESS;
}




