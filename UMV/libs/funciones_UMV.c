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
	char *dic_op[]={"operacion","dump","compactacion","retardo","algoritmo","exit"};
	int aux;
	for(aux=0;aux<6;aux++){
		if(strcmp(dic_op[aux], palabra) == 0){
			return 1;
		}

	}
	return 0;
}

int estaEnDicTOP(char palabra[]){
	char *dic_top[]={"solicitar","escribir","crear","destruir"};
	int aux;
	for(aux=0;aux<4;aux++){
		if((strcmp(dic_top[aux], palabra)) == 0){
			return 1;
		}
	}
	return 0;
}


int* crearMP(void) { // Cambie para que no reciba parametro, total la config es una variable externa -- Fede
	tamanioMP = configuracion_UMV.memSize;
	int* MP;
	MP = malloc(tamanioMP);
	return MP;
}

void log_error_socket(void){
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
}

/*_Bool segmentationFault(uint32_t base,uint32_t offset){// TODO Revisar bien esto y el memOverload de abajo
	if ( > tamanioMP) {
	    printf("Segmentation Fault al intentar acceder a posicion %d \n", base+offset);
		return true;
	} else{
		return false;
	}
}

_Bool memoryOverload(uint32_t base,uint32_t offset, uint32_t longitud){
	if (> tamanioMP) {
		    printf("Memory Overload al intentar escribir %d bytes en la posicion %d \n", longitud,base+offset);
			return true;
		} else{
			return false;
		}
}*/

//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
/*_Bool solicitarMemoria(t_programa programa){
	return true;
}         esto quedo colgado no????*/

// ***********************************Solicitar bytes en memoria*******************

void solicitarDesdePosicionDeMemoria(uint32_t base,uint32_t offset, uint32_t longitud){


}

void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud,t_buffer buffer){
	if (validarSolicitud(longitud)){
		int posicionReal = tablaDeSegmentos[programaEnUso][base].ubicacionMP + offset;
		int nuevaPosicionReal = asignarFisicamente(posicionReal,buffer); /*va a retornar la direccion fisica segun WF o FF - necesita solo buffer?*/
		tablaDeSegmentos[programaEnUso][base].ubicacionMP = posicionReal;
		puts("resultadodelaasignacion");
		} else {
			puts("No se pudo realizar la asignacion");
	    }
}

int asignarFisicamente(){
	//Podria tambien ser una funcion modificarFisicamente(posicionReal,buffer,ASIGNAR-QUITAR) que valide que hacer segun el tercer parametro, habra que ver si conviene
	return 0; //ojo que va a retornar la dir fisica
}

/*************************    Logica de validacion de solicitudes ***************************/
//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion - REVISAR
_Bool validarSolicitud(uint32_t longitud){
	if(hayEspacioEnMemoriaPara(longitud)){
		return true;
	} else{
		puts("No alcanza el espacio en memoria:");
		if(segmentationFault(longitud)){
			return false;
		} else { if(memoryOverload(longitud)){
							return false;
				} else {
							//puts("Excepcion Desconocida"); ???
							return false;
					}
				}
			}
}

_Bool hayEspacioEnMemoriaPara(uint32_t longitud){
	if( tamanioSuficienteEnMemoriaPara(longitud) ){
		return true;
	}else{
		compactar();
		if( tamanioSuficienteEnMemoriaPara(longitud) ) return true;
		else return false;
	}
}

_Bool tamanioSuficienteEnMemoriaPara(uint32_t longitud){
	int aux=0;
	int contador=0;
	while (aux < tamanioMP){
			if (MP[aux] != NULL){
				aux++;
			} else{
				while (aux == NULL && contador < longitud){
					contador++;
					aux++;
				}
				if (contador == longitud)return true;
				else contador=0;
			}
	}
	return false;
}


//Comandos de consola:

void retardo(int valorRetardoEnMilisegundos){ //Cantidad de ms que debe esperar UMV para responder una solicitud

}

void algoritmo(void){//Cambiar entre Worst fit y First fit
	if(configuracion_UMV.algoritmo==worstfit){
		configuracion_UMV.algoritmo=firstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}
	else{
		configuracion_UMV.algoritmo=worstfit;
							//printf("%d\n", *algor); para checkear el cambio del valor
	}

}

//****************************************Compactacion*****************************************

void compactar(){
	int sigSegmento;
	int posicionDeDestino;
	//Obtengo primer posicion libre en MP
		int i=0;
		while (MP[i]!=NULL) i++;
		posicionDeDestino= i;
		sigSegmento=i;

	while (sigSegmento != tamanioMP){
		if (sigSegmento == NULL){
			sigSegmento++;
		} else{
			int tamanio= obtenerTamanioDelSegmento();
			//actualizar tabla de segmentos de el proceso dueño del segmento
			//desplazar (MP[sigSegmento] hasta MP[sigSegmento+tamanio]) a MP[posicionDeDestino]
			sigSegmento= sigSegmento+tamanio+1;
			posicionDeDestino= MP[posicionDeDestino+tamanio+1];
		}
	}
}


int obtenerTamanioDelSegmento(){
	int tamanio;
	//Habia pensado buscar hasta que sea NULL, pero si hay un segmento al lado de otro lo va a mover tambien,
	//lo cual no seria un problema siempre y cuando actualizemos bien la tabla de segmentos de cada programa (suponiendo que esos dos segmentos corresponden a programas diferentes)
	return tamanio;
}

void dump(){
	//obtenerDatosDeMemoria() y mostrar (y,opcional, guardar en archivo)
}

void crearSegmentoPrograma(int id_prog, int tamanio){
	int ubicacion;
	//Escoge la ubicacion en base al algoritmo de config
	if(configuracion_UMV.algoritmo == firstfit){
		ubicacion = escogerUbicacionF(tamanio);
	}
	if(configuracion_UMV.algoritmo == worstfit){
		ubicacion = escogerUbicacionW(tamanio);
	}
	//TODO: Crear el segmento en sí

}

int escogerUbicacionF(int tamanio){
	int finDeSegmento;
	int posicionDeDestino;
		//Obtengo primer posicion libre en MP
			int i=0;
			while (MP[i]!=NULL) i++;
			posicionDeDestino= i;
			finDeSegmento=posicionDeDestino+tamanio;
		//Checkeo si la ultima posicion del segmento esta libre, si no, busca de nuevo
		if(MP[finDeSegmento] != NULL){
			while(MP[finDeSegmento] !=NULL){
				i=finDeSegmento;
				while (MP[i]!=NULL)i++;
				posicionDeDestino= i;
				finDeSegmento=posicionDeDestino+tamanio;
			}
		}
		//Devuelve la posicion de ubicacion del segmento
			return posicionDeDestino;
}

int escogerUbicacionW(int tamanio){
	int finDeSegmento;
	int posicionDeDestino;
	/*TODO: acá habría que recorrer la MP buscando la última posición en la
que entraría el segmento del programa*/


	//Devuelve la posicion
		return posicionDeDestino;
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
		configuracion_UMV.algoritmo=firstfit;

}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Tamanio de memoria Principal: %d\n", configuracion_UMV.memSize);
	printf("Puerto para conexiones con CPUs: %d\n", configuracion_UMV.puerto_cpus);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_UMV.puerto_kernel);
	printf("IP del Kernel: %d\n", configuracion_UMV.ip_kernel);
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
			log_error_socket();	//Error con el close
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
				crear_hilo_por_cpu(paquete);
			}
			break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
		}
}

void crear_hilo_por_cpu(t_nipc* paquete){
	pthread_t atender_pedido;
	pthread_create(&atender_pedido, NULL, (void*) &atender_cpu, paquete);	//Crea un hilo para atender cada conexion de cpu
	pthread_join(atender_pedido, NULL);	//Espera a que termine de ejecutarse la atencion del pedido
}

void atender_cpu(t_nipc* paquete){
	//Aca habria que atender el pedido de una cpu
}


void core_conexion_kernel(void){
	int sock_kernel;		//El socket de conexion
	int n_sock_kernel;		//El socket de datos
	t_nipc* paquete;	//El paquete que recibe el socket

	if ((sock_kernel = nipc_abrirConexion(configuracion_UMV.puerto_kernel))<0){
		log_error_socket();	//Error con el close
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
				atender_kernel(paquete);
			}
			break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
		}
}

void atender_kernel(t_nipc* paquete){
	//Aca habria que atender el pedido del kernel
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
				puts("Ingrese el processID de programa a usar");
				int nuevoPrograma;
				gets(nuevoPrograma);
				if(nuevoPrograma != programaEnUso){
					cambioDeProcesoActivo(nuevoPrograma);
				}else{}
				char tipoOperacion[32];
				puts("\nDesea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?");
				gets(tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0){
						puts("\nTipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						gets(tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					  puts("\n Ingrese Base, Offset y Tamanio de segmento");
					  int unaBase,unOffset,unTamanio;
					  scanf("%d","%d","%d",unaBase,unOffset,unTamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  solicitarDesdePosicionDeMemoria(unaBase,unOffset,unTamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					  puts("\n Ingrese Base, Offset, Tamanio de segmento y Buffer a escribir");
					  int unaBase,unOffset,unTamanio;
					  t_buffer buffer;
					  scanf("%d","%d","%d","%s",unaBase,unOffset,unTamanio,buffer);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  enviarBytes(unaBase,unOffset,unTamanio,buffer);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "crear") == 0){
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  //crearSegmentoPrograma(t_programa Programa);  acordarse que la ubicacion virtual es aleatoria
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
				   algoritmo();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando, "compactacion") == 0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				 //compactar();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando,"dump") ==0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				 //dump();
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

