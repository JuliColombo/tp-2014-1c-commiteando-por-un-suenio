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

t_buffer* solicitarDesdePosicionDeMemoria(uint32_t base,uint32_t offset, uint32_t longitud){
	t_buffer* buffer;
	uint32_t posicionReal;
	posicionReal = tablaDeSegmentos[programaEnUso][base].ubicacionMP + offset;
	buffer = obtenerBytesDesdeHasta(posicionReal,longitud);
	return buffer;
}
t_buffer obtenerBytesDesdeHasta(uint32_t posicionReal,uint32_t longitud){
	t_buffer buffer;

	return buffer;
}

void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud,t_buffer buffer){
	if (validarSolicitud(longitud)){
		uint32_t posicionReal = tablaDeSegmentos[programaEnUso][base].ubicacionMP + offset;
		uint32_t nuevaPosicionReal = asignarFisicamente(posicionReal,buffer); /*va a retornar la direccion fisica segun WF o FF - necesita solo buffer?*/
		tablaDeSegmentos[programaEnUso][base].ubicacionMP = posicionReal;
		puts("resultadodelaasignacion");
		} else {
			puts("No se pudo realizar la asignacion");
	    }
}

int asignarFisicamente(){

	return 0; //ojo que va a retornar la dir fisica
}

/*************************    Logica de validacion de solicitudes ***************************/
//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion - REVISAR
_Bool validarSolicitud(uint32_t longitud){
	if(hayEspacioEnMemoriaPara(longitud)){
		return true;
	} else{
		puts("No alcanza el espacio en memoria:");
		if(/*segmentationFault(longitud)*/ 1){
			return false;
		} else { if(/*memoryOverload(longitud)*/ 1){
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
	typedef struct aux{
		int programa;
		int numSegDesc;
		int inicio;
		int tamanio;
		int ubicacionMP;
	}aux;

	aux getDatosSegmentDescriptorDe(int segmento);

	//Obtengo primer posicion libre en MP
		int i=0;
		while (MP[i]!=NULL) i++;
		posicionDeDestino= i;
		sigSegmento=i;

	while (sigSegmento != tamanioMP){
		if (sigSegmento == NULL){
			sigSegmento++;
		} else{
			aux datos=getDatosSegmentDescriptorDe(sigSegmento);
			int programaDuenio = datos.programa;
			int numSegDescDelProgramaDuenio = datos.numSegDesc;
			int tamanio= tablaDeSegmentos[programaDuenio][numSegDescDelProgramaDuenio].tamanio; //o datos.tamanio si va a quedar asi

			//desplazar (MP[sigSegmento] hasta MP[sigSegmento+tamanio]) a MP[posicionDeDestino]

			tablaDeSegmentos[programaDuenio][numSegDescDelProgramaDuenio].ubicacionMP = posicionDeDestino;

			sigSegmento= sigSegmento+tamanio+1;
			posicionDeDestino= MP[posicionDeDestino+tamanio+1];
		}
	}

		aux getDatosSegmentDescriptorDe(int unSegmento){
			aux datos;

			//Recorrer la tablaDeSegmentos comparando la .ubicacionMP hasta encontrarlo?

			return datos;
			}
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
	int posicionDeDestino;
	int i=0;
			//Recorro la memoria principal
		while(i<tamanioMP){
			int aux=0;
			//Obtengo primer posicion libre en MP
			while (MP[i]!=NULL && i<tamanioMP) i++;
			posicionDeDestino= i;
			//Checkeo la cantidad de posiciones libres hasta llegar a
			//una posicion ocupada, o el final de la memoria
			while(MP[i] != NULL && i<tamanioMP){
				aux++;
				i++;
			}
			//Checkeo si la cantidad de posiciones es mayor o igual al tamanio
			//solicitado
			if(aux>=tamanio){
			//Devuelve la posicion de ubicacion del segmento
			return posicionDeDestino;
			}
	}
			//Si llega hasta acá es porque no encontró una posición posible para el segmento
			//Le pongo que retorne -1 para mostrar este error
			return -1;
}

int escogerUbicacionW(int tamanio){
	int posicionDeDestino;
	int max=0;
	//La inicializo en -1, para que al final, si no se encontro una posicion libre
	//retorne este valor de error
	int posicionFinal=-1;
	int i=0;
	//Recorro la memoria principal
	while(i<tamanioMP){
				int aux=0;
			//Obtengo primer posicion libre en MP
				while (MP[i]!=NULL && i<tamanioMP) i++;
				posicionDeDestino= i;
			//Checkeo la cantidad de posiciones libres hasta llegar a
			//una posicion ocupada, o el final de la memoria
				while(MP[i] != NULL && i<tamanioMP){
					aux++;
					i++;
				}
			//Checkeo si la cantidad de posiciones es mayor o igual al tamanio
			//solicitado
				if(aux>=tamanio){
			//Checkeo si esta cantidad es mayor que el maximo numero de posiciones
			//libres encontradas
						if(max>aux){
			//Reasigno el maximo y la posicionFinal
							max=aux;
							posicionFinal=posicionDeDestino;
						}
				}
		}

	//Devuelve la posicionFinal, que valdrá -1 de no encontrar espacio
		return posicionFinal;
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


		configuracion_UMV.ip_kernel = config_get_string_value(config,
					"Direccion IP para conectarse al Kernel");


		configuracion_UMV.algoritmo = config_get_int_value(config,
					"Algoritmo de seleccion de ubicacion de segmento");

}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Tamanio de memoria Principal: %d\n", configuracion_UMV.memSize);
	printf("Puerto para conexiones con CPUs: %d\n", configuracion_UMV.puerto_cpus);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_UMV.puerto_kernel);
	printf("IP del Kernel: %s\n", configuracion_UMV.ip_kernel);
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
	int sock_cpu=socket_crearServidor(configuracion_UMV.ip_kernel, configuracion_UMV.puerto_cpus);

	if(socket_cerrarConexion(sock_cpu)<1){
	//Error cerrando el socket
	}
	return;
}

void crear_hilo_por_cpu(void){
	pthread_t atender_pedido;
	pthread_create(&atender_pedido, NULL, (void*) &atender_cpu, NULL);	//Crea un hilo para atender cada conexion de cpu
	pthread_join(atender_pedido, NULL);	//Espera a que termine de ejecutarse la atencion del pedido
}

void atender_cpu(void){
	//Aca habria que atender el pedido de una cpu
}


void core_conexion_kernel(void){
	int sock_kernel=socket_crearCliente();

	if(socket_cerrarConexion(sock_kernel)<1){
	//Error cerrando el socket
	}
	return;
}

void atender_kernel(void){
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

