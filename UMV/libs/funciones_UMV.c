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
	if(MP==NULL){
		log_escribir(archLog, "Error en tamaño de memoria principal", ERROR, "No hay memoria suficiente");
		abort();
	}
	return MP;
}

void log_error_socket(void){
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
	pthread_mutex_unlock(mutex_log);
}

_Bool segmentationFault(uint32_t base,uint32_t offset){/*// TODO Revisar bien esto y el memOverload de abajo
	if ( > tamanioMP) {
	    printf("Segmentation Fault al intentar acceder a posicion %d \n", base+offset);
		return true;
	} else{
		return false;
	}*/
	return false; //pongo esto para que no se queje
}

_Bool memoryOverload(uint32_t base,uint32_t offset, uint32_t longitud){/*
	if (> tamanioMP) {
		    printf("Memory Overload al intentar escribir %d bytes en la posicion %d \n", longitud,base+offset);
			return true;
		} else{
			return false;
		}*/
	return false; //pongo esto para que no se queje
}

//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
/*_Bool solicitarMemoria(t_programa programa){
	return true;
}         esto quedo colgado no????*/

// ***********************************Solicitar bytes en memoria*******************

t_buffer solicitarDesdePosicionDeMemoria(uint32_t base,uint32_t offset, uint32_t longitud){
	t_buffer buffer;
	uint32_t posicionReal;
	//posicionReal = tablaDeSegmentos[programaEnUso][base].ubicacionMP + offset;
	buffer = obtenerBytesDesdeHasta(posicionReal,longitud);
	return buffer;
}
t_buffer obtenerBytesDesdeHasta(uint32_t posicionReal,uint32_t longitud){
	t_buffer buffer;

	return buffer;
}

//****************************************enviarBytes*************************************

void asignarFisicamenteDesde(int posicionReal,int longitud, t_buffer buffer){
	int i=0;
	while (i<longitud){
		MP[posicionReal]=buffer[0];
		i++;
		posicionReal++;
	}
	if (sizeof(buffer) != i) printf("El buffer no se envio completamente"); //No se si es necesario, pero capaz ayuda
}
int ubicarEnTabla(int inicio){
	int i=0;
	int procesoDelHilo;
	while(i < sizeof(tablaDeSegmentos[procesoDelHilo].segmentos)){
		if (tablaDeSegmentos[procesoDelHilo].segmentos[i].inicio==inicio) return i;
		else i++;
	}
	//Si llega aca no se encontro un segmento que inicie en: inicio
	printf("La posicion de base no se encuentra en la tabla de segmentos");
	return -1;
}

void enviarBytes(int base,int offset,int longitud,t_buffer buffer){
	int procesoDelHilo; //Donde lo declaramos??
	int segmentoBase= ubicarEnTabla (base);
	if (segmentoBase!= -1){
		if (validarSolicitud(longitud)){
			int posicionReal= tablaDeSegmentos[procesoDelHilo].segmentos[segmentoBase].ubicacionMP+offset;
			asignarFisicamenteDesde(posicionReal,longitud,buffer);
			puts("resultadodelaasignacion");
			} else puts("No se pudo realizar la asignacion");
		}
}


void cambioDeProcesoEnElHilo(int id_prog){
 //TODO
}

/*************************    Logica de validacion de solicitudes ***************************/
//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion - REVISAR
_Bool validarSolicitud(uint32_t longitud){
	if(hayEspacioEnMemoriaPara(longitud)){
		return true;
	} else{
		printf("No alcanza el espacio en memoria:");
		/*if(segmentationFault(longitud)){
			return false;
		}else{
			if(memoryOverload(longitud)){
				return false;
			}else{
				//printf("Excepcion Desconocida"); ???
				return false;
				}
			}*/
		return false; //pongo esto para que no joda
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
				while (MP[aux] == NULL && contador < longitud){
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

void retardoFunc(int valorRetardoEnMilisegundos){ //Cantidad de ms que debe esperar UMV para responder una solicitud
	retardo= valorRetardoEnMilisegundos;
}

void algoritmo(void){//Cambiar entre Worst fit y First fit
	if(configuracion_UMV.algoritmo==worstfit){
		configuracion_UMV.algoritmo=firstfit;
		printf("El algoritmo se cambio a: firstfit\n");
	}
	else{
		configuracion_UMV.algoritmo=worstfit;
		printf("El algoritmo se cambio a: worstfit\n");
	}
}






//****************************************Compactacion*****************************************

void compactar(){/*
	int sigSegmento=0;
	int posicionDeDestino=0;
	//Estructura auxiliar para obtener los datos de cada segmento en MP
	typedef struct{
		int posicion;
		int numSegDesc;
	}aux;

	//Obtengo primer posicion libre en MP
		int i=0;
		while(MP[i]!=NULL){
		i++;
		posicionDeDestino= i;
		sigSegmento=i;
		}
	while (sigSegmento != tamanioMP){
		if(MP[sigSegmento] == NULL){
			sigSegmento++;
		}else{
			aux* datos = getDatosSegmentDescriptorDe(sigSegmento);
			int tamanio= tablaDeSegmentos[datos->posicion].segmentos[datos->numSegDesc].tamanio;

			//desplazar (MP[sigSegmento] hasta MP[sigSegmento+tamanio]) a MP[posicionDeDestino]
			int a = 0;
			while (a <= tamanio){
				MP[posicionDeDestino] = MP[sigSegmento+a];
				MP[sigSegmento+a] = NULL;
				a++;
			}

			//tablaDeSegmentos[datos->posicion].segmentos[datos->numSegDesc].ubicacionMP = posicionDeDestino;
			sigSegmento= sigSegmento+tamanio+1;
			posicionDeDestino= MP[posicionDeDestino+tamanio+1];
		}
	}

		aux getDatosSegmentDescriptorDe(int ubicacion){//Recorrer la tablaDeSegmentos comparando la .ubicacionMP hasta encontrarlo
			aux datos;

			//Creo la estructura auxiliar para que list_size me devuelva el tamaño de la tablaDeSegmentos
			t_list listaAux;
			int contador;
			listaAux.head= tablaDeSegmentos;
			listaAux.elements_count= contador;
			int i,j=0;
			int tamanioTablaS = list_size(listaAux);

			while(i < tamanioTablaS){//Recorro los programas

				//Hago lo mismo pero para los segmentos de ese programa
				t_list listaAux2;
				int contador2;
				listaAux2.head= tablaDeSegmentos[i].segmentos;
				listaAux2.elements_count= contador2;
				int tamanioSegmentosi = list_size(listaAux2);

				while(j < tamanioDeSegmentosi){ //Recorro sus segmentos
					if(tablaDeSegmentos[i].segmentos[j].ubicacionMP== ubicacion){ //Cargo en el de datos y return eso si es asi
						datos.posicion= i;
						datos.numSegDesc= j;
						return datos;
					}else{
						j++;
					}
				}
				j=0;
			}
			//Excepcion::: no se encuentra el segmento
			datos.numSegDesc=-1;
			datos.posicion=-1;
			return datos;
		}*/
}





void dump(){
	//obtenerDatosDeMemoria() y mostrar (y,opcional, guardar en archivo)
}

int validarSegmentoDisponibleEn(int pos, int j) {
	/*int ultimoSegmentoDelProg;	//hay que ver como conseguirlo!!!
	while (j < ultimoSegmentoDelProg){
	if (tablaDeSegmentos[pos].segmentos[j].inicio == i) {
		break;
	}
	}*/
	return 1; //pongo esto para que no se queje
}

void crearSegmentoPrograma(int id_prog, int tamanio){
	int ubicacion;
	segmentDescriptor aux;
	int i;
	//tipoSegmento segmento;
	//Escoge la ubicacion en base al algoritmo de config
	if(validarSolicitud(tamanio)){
		if(configuracion_UMV.algoritmo == firstfit){
			ubicacion = escogerUbicacionF(tamanio);
		}
		if(configuracion_UMV.algoritmo == worstfit){
			ubicacion = escogerUbicacionW(tamanio);
		}
		}else{
			printf("No hay espacio disponible para %d",id_prog);
			return;
		}

	//segmento=malloc(sizeof(tamanio));
	int pos=getPosTablaSeg(id_prog);
		if (pos==-1){
			//Excepcion, el programa al que se le quiere crear el segmento no esta en la tabla
		}
	i=rand();
	while(!validarSegmentoDisponibleEn(pos,i)) rand();//Recorrer la tabla de segmentos validando que ninguno ocupe entre la posicion y la posicion y el tamanio
	//aux.segmento=segmento;
	aux.ubicacionMP=ubicacion;
	aux.inicio=i;
	aux.tamanio=tamanio;

	tablaDeSegmentos[pos].segmentos[i]=aux;


}

int getPosTablaSeg(int id_prog){
	int i;
	int posFinalTablaSeg; //Hay que ver como conseguirla, o validar que ya se recorrieron todos los segmentos de otra forma
	while (tablaDeSegmentos[i].id_prog != id_prog && i<= posFinalTablaSeg) i++;
	if (tablaDeSegmentos[i].id_prog != id_prog){
		return i;
	} else return -1;
}

int escogerUbicacionF(int tamanio){
	int posicionDeDestino;
	int i=0;
			//Recorro la memoria principal
		while(i<tamanioMP){
			int aux=0;			//Obtengo primer posicion libre en MP
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
				while(MP[i]!= NULL && i<tamanioMP){
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

void destruirSegmentos(int id_prog){
	int pos= getPosTablaSeg(id_prog);
	liberarMP(pos);
	eliminarSegmentos(pos);
	return;

}

void liberarMP(int pos){
	int i,j,tam;
	i=0;
	while(i<tamanioMP){
		//Recorro las posiciones ocupadas de la tabla de segmentos y obtengo la
		//ubicacionMP y tamanio
			j=tablaDeSegmentos[pos].segmentos[i].ubicacionMP;
			tam=tablaDeSegmentos[pos].segmentos[i].tamanio;
		//Recorro las posiciones de MP en base a ubicacionMP y tamanio
		// y las igualo a NULL
			while(tam!=0){
				MP[j]=NULL;
				j++;
				tam--;
			}
			i++;
	}

}

void eliminarSegmentos(int pos){
	int i,ultimaPos;
	i=0;
	//ultimaPos=ultimoSeg(pos);
	//Recorro la tabla de segmentos del id_prog
	while(i<ultimaPos){
		//Por cada posicion ocupada, libero el espacio de memoria
		//free(tablaDeSegmentos[pos].segmentos[i]);
		i++;
	}
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
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		pthread_mutex_unlock(mutex_log);
	}
	else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	procesosActivos = malloc(gradoDeMultiprogramacion);
	}
}

//****************************************Atender Conexiones de Kernel/CPU*******************

void core_conexion_cpu(void){
	int sock;
	pthread_t atender_pedido;

	if((sock_cpu=socket_crearServidor("127.0.0.1", configuracion_UMV.puerto_cpus))>0){
	printf("Hilo de CPU \n");
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Escuchando en el socket de CPU's", INFO, "");
	pthread_mutex_unlock(mutex_log);

	}


	while(1){
	if((sock=socket_aceptarCliente(sock_cpu))>0){
			printf("Acepta conexion");
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Se acepta la conexion de una CPU", INFO, "");
			pthread_mutex_unlock(mutex_log);
			pthread_create(&atender_pedido, NULL, (void*) &atender_cpu, NULL);	//Crea un hilo para atender cada conexion de cpu

			/*t_tipoEstructura tipoRecibido;
					void* structRecibida;
					int j=socket_recibir(sock,&tipoRecibido,&structRecibida);
					if(j==1){
					printf("Se recibio envio bien el paquete\n");
					t_struct_numero* k = ((t_struct_numero*)structRecibida);
					printf("%d\n", k->numero);
					}*/

			t_tipoEstructura tipoRecibido;
				void* structRecibida;
				int j=socket_recibir(sock,&tipoRecibido,&structRecibida);
				if(j==1){
				printf("Se recibio envio bien el paquete\n");
				t_struct_string* k = ((t_struct_string*)structRecibida);
				printf("%s\n", k->string);
				}

				/*t_tipoEstructura tipoRecibido;
				void* structRecibida;
				int j=socket_recibir(sock,&tipoRecibido,&structRecibida);
				if(j==1){
				printf("Se recibio envio bien el paquete\n");
				t_struct_char* k = ((t_struct_char*)structRecibida);
				printf("%c\n", k->letra);
				}*/
	}

	}


	if(socket_cerrarConexion(sock_cpu)==0){
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Se trata de cerrar el socket de CPU", ERROR, "Hay problemas para cerrar el socket");
		pthread_mutex_unlock(mutex_log);
		//Error cerrando el socket
	} else {
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Se cierra el socket de CPU", INFO, "No hay problemas para cerrar el socket");
		pthread_mutex_unlock(mutex_log);
	}
	return;
}


void atender_cpu(void){
	/*UNSOLVED:
	 * int programaEnHilo;
	 * void* estructura;
	 * t_tipoEstructura tipo_estructura;
	 * socket_recibir(sock, &tipo_estructura, &estructura);
	 * ejecutar(&tipo_estructura, &estructura);		//ejecutaria lo correspondiente y crearia la estructura a enviar
	 * send(sock, &tipo_estructura, &estructura);
	 */

}



void core_conexion_kernel(void){
	if((sock_kernel_servidor=socket_crearServidor("127.0.0.1",configuracion_UMV.puerto_kernel))>0){
	printf("Hilo de Kernel\n");
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Escuchando en el socket de Kernel", INFO, "");
	pthread_mutex_unlock(mutex_log);
	}
	int sock_aceptado;
	if((sock_aceptado=socket_aceptarCliente(sock_kernel_servidor))>0){
			printf("Acepta conexion");
			pthread_mutex_lock(mutex_log);
			log_escribir(archLog, "Se acepta la conexion del Kernel", INFO, "");
			pthread_mutex_unlock(mutex_log);
		}
		int* tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sock_aceptado,&tipoRecibido,&structRecibida);
		if(j==1){
		printf("Se recibio envio bien el paquete\n");
		int* k = ((int*)structRecibida);
		printf("%d\n", *k);
		}
		while(1){
		}
	if(socket_cerrarConexion(sock_kernel_servidor)==0){
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Se trata de cerrar el socket de Kernel", ERROR, "Hay problemas para cerrar el socket");
		pthread_mutex_unlock(mutex_log);
		//Error cerrando el socket
	} else {
		pthread_mutex_lock(mutex_log);
		log_escribir(archLog, "Se cierra el socket de Kernel", INFO, "No hay problemas para cerrar el socket");
		pthread_mutex_unlock(mutex_log);
	}

	return;
}

void atender_kernel(void){
	/*UNSOLVED:
	 * int programaEnHilo;
	 * void* estructura;
	 * t_tipoEstructura tipo_estructura;
	 * socket_recibir(sock, &tipo_estructura, &estructura);
	 * ejecutar(&tipo_estructura, &estructura);		//ejecutaria lo correspondiente y crearia la estructura a enviar
	 * send(sock, &tipo_estructura, &estructura);
	 */
}


//***********************************************Inicializacion de semaforos************************************


void inicializarSemaforos(void){
	mutex=malloc(sizeof(pthread_mutex_t));
	mutex_log=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex,NULL);
	pthread_mutex_init(mutex_log, NULL);
}

//***********************************************Inicializacion y espera de hilos************************************

void inicializarHilos(void){
	pthread_create(&CONSOLA, NULL, (void*) &core_consola, NULL);
	pthread_create(&KERNEL, NULL, (void*) &core_conexion_kernel, NULL);
	pthread_create(&CPU, NULL, (void*) &core_conexion_cpu, NULL);
}

void esperarHilos(void){
	pthread_join(CONSOLA,NULL);
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
	int procesoDelHilo,unaBase,unOffset,unTamanio,modo;
	t_buffer buffer;
	puts("Ingrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	scanf("%s",&comando);
	while(estaEnDicOP(comando)== 0){
		puts("\nOperacion erronea, escriba la operacion de nuevo");
		scanf("%s",&comando);
	}

	while(strcmp(comando, "exit") != 0){
			if(strcmp(comando, "operacion") == 0){
				puts("Ingrese el processID de programa a usar");
				scanf("%d",&procesoDelHilo);
				if(procesoDelHilo != procesoEnUso){
					//cambioDeProcesoActivo(nuevoPrograma);
				}else{}
				char tipoOperacion[32];
				puts("\nDesea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?");
				scanf("%s",&tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0){
						puts("\nTipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						scanf("%s",&tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					  puts("\n Ingrese Base");
					  scanf("%d",&unaBase);
					  puts("\n Ingrese Offset");
					  scanf("%d",&unOffset);
					  puts("\n Ingrese Tamanio de segmento");
					  scanf("%d",&unTamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  solicitarDesdePosicionDeMemoria(unaBase,unOffset,unTamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					 puts("\n Ingrese Base");
					 scanf("%d",&unaBase);
					 puts("\n Ingrese Offset");
					 scanf("%d",&unOffset);
					 puts("\n Ingrese Tamanio de segmento");
					 scanf("%d",&unTamanio);
					 puts("\n Ingrese Buffer");
					 scanf("%s",&buffer);
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
				   printf("Ingrese modo de dump (1 = enabled, 0 = disabled)\n");
				   scanf("%d",&modo);
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				 //dump(modo);
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			}

		puts("\nEscriba la siguiente operacion");
		scanf("%s",&comando);
		while(estaEnDicOP(comando)== 0){
				puts("\nOperacion erronea, escriba la operacion de nuevo");
				scanf("%s",&comando);
			}

		}
	if(strcmp(comando,"exit") ==0){
									  	destruirTodosLosSegmentos();
									   	free(MP);

									   	socket_cerrarConexion(sock_kernel_servidor);
									   	socket_cerrarConexion(sock_cpu);
									   	matarHilos();
									   	if(pthread_kill(CPU,0)==0){
									   		printf("Muere el hilo cpu\n");
									   	}
									   	if(pthread_kill(KERNEL,0)==0){
									   		printf("Muere el hilo Kernel\n");
									   	}
						   }

}

void matarHilos(void){
	pthread_cancel(CPU);
	pthread_cancel(KERNEL);

}

void destruirTodosLosSegmentos(void){
	return;
}





