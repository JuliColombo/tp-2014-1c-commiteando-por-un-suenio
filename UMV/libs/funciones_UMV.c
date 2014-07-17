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
	int* aux_MP;
	aux_MP = malloc(tamanioMP);
	if(aux_MP==NULL){
		log_escribir(archLog, "Error en tamaño de memoria principal", ERROR, "No hay memoria suficiente");
		abort();
	}
	return aux_MP;
}

void log_error_socket(void){
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
	pthread_mutex_unlock(mutex_log);
}
_Bool validacionSegFault(int base, int offset,int longitud){

	int numSeg=traducirPosicion(base);
	if (tablaDeSegmentos[procesoEnUso].segmentos[numSeg].inicio == NULL) return true;
	if (tablaDeSegmentos[procesoEnUso].segmentos[numSeg].tamanio < longitud) return true;
	return false;
}
_Bool segmentationFault(int base,int offset,int longitud){// TODO Revisar bien esto y el memOverload de abajo
	if (validacionSegFault(base,offset,longitud) ) {
		log_escribir(archLog, "SegmentationFault", ERROR, "Segmentation fault al intentar acceder a la posicion");
		return true;
	} else return false;
}

_Bool memoryOverload(int longitud){
	int tamanioLibre= tamanioMP - list_count_satisfying(MP, &MP != NULL);
	if (longitud < tamanioLibre) {
		    log_escribir(archLog, "Memory Overload", ERROR, "Memory Overload al intentar acceder a la posicion");
			return true;
		} else return false;
}


// ***********************************Solicitar bytes en memoria*******************

t_buffer solicitarBytes(int base,int offset, int longitud){
	t_buffer buffer;
	buffer = malloc(sizeof(longitud*sizeof(int)));
	int i=0;
	int j;
	j=traducirPosicion(base)+offset;
	while (i < longitud){
		buffer[j]= MP[j];
		j++;
		i++;
	}
	if (sizeof(buffer) != longitud){
		printf("El buffer no se obtuvo completamente. Rechazarlo? S/N");//No se si es necesario, pero capaz ayuda
		char respuesta;
		scanf("%c",&respuesta);
		if (respuesta=='N') return buffer;
		else{
			buffer=NULL;
			return buffer;
		}
	}
	return buffer;
}

int traducirPosicion(int base){
	int i=0,j;
	while(i<cant_tablas){
		j=0;
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].inicio == base){
						return tablaDeSegmentos[i].segmentos[j].ubicacionMP;
					} else {
						j++;
					}
		}
		i++;
	}
	log_escribir(archLog, "Solicitar memoria", ERROR, "Posicion erronea");
	return -1;
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

void enviarBytes(int base,int offset,int longitud,t_buffer buffer){
	int i=0;
	int j,aux;
		if (validarSolicitud(base,offset,longitud)){
			aux=traducirPosicion(base);
			if(aux==-1){
							printf("La direccion base es erronea");
							return;
						}
			j=traducirPosicion(base)+offset;
			printf("La posicion Virtual es: %d y la Real es : %d\n", base, j);
			printf("El resultado de la asignacion es:\n");
			while(i<longitud){
				MP[j]= buffer[i];
				printf("Posicion %d de memoria principal = %d\n", j, MP[j]);
				j++;
				i++;
			}
			} else puts("No se pudo realizar la asignacion");
}


void cambioDeProcesoEnElHilo(int id_prog){
 //TODO
}

/*************************    Logica de validacion de solicitudes ***************************/
//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion - REVISAR
_Bool validarSolicitud(int base, int offset,int longitud){
	if(hayEspacioEnMemoriaPara(longitud)){
		return true;
	} else{
		if(segmentationFault(base,offset,longitud)){
			return false;
		}else{
			if(memoryOverload(longitud)){
				return false;
			}else{
				//log("Excepcion Desconocida"); ???
				return false;
				}
			}
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

_Bool tamanioSuficienteEnMemoriaPara(uint32_t longitud){ //Esto se puede reemplazar con las funciones de las commons si no funciona
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

/*************************Handshake*************************/

void hacerHandshake(int id_prog, tipo_handshake tipo){
	if(lista_handshakes.cantidad == 0){
		inicializarYAgregar(id_prog, tipo);
	} else { agregarHandshake(id_prog, tipo);
	}
}

void inicializarListaHandshakes(void){
	lista_handshakes.cantidad=0;
	lista_handshakes.handshakes=NULL;
}


void inicializarYAgregar(int id_prog, tipo_handshake tipo){
	handshake* aux_lista;
	handshake aux_handshake;
	aux_lista = malloc(sizeof(handshake));
	if(aux_lista==NULL){
					log_escribir(archLog, "Error en la lista de handshakes", ERROR, "No hay memoria suficiente");
					abort();
				}
	lista_handshakes.cantidad ++;
	aux_handshake.id_prog = id_prog;
	aux_handshake.tipo = tipo;
	lista_handshakes.handshakes[0] = aux_handshake;

}

void agregarHandshake(int id_prog, tipo_handshake tipo){
	handshake* aux_lista;
	handshake aux_handshake;
	int aux=lista_handshakes.cantidad;
	aux_lista = realloc(lista_handshakes.handshakes,(sizeof(lista_handshakes.handshakes)+sizeof(handshake)));
	if(aux_lista==NULL){
					log_escribir(archLog, "Error en la lista de handshakes", ERROR, "No hay memoria suficiente");
					abort();
				}
	lista_handshakes.handshakes = aux_lista;
	aux_handshake.id_prog=id_prog;
	aux_handshake.tipo=tipo;
	lista_handshakes.handshakes[aux] = aux_handshake;
	lista_handshakes.cantidad++;
	return;
}

/*************************Comandos de Consola:*************************/

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

			tablaDeSegmentos[datos->posicion].segmentos[datos->numSegDesc].ubicacionMP = posicionDeDestino;
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

				while(j < tamanioSegmentosi){ //Recorro sus segmentos
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


/*************************Dump: *************************/


void dump(){
	FILE* archivo;
	int i=0;
	archivo = fopen("/home/utnso/dump_config", "w");
	if (archivo==NULL) {
		fputs ("File error",stderr); exit (1);
	}
	//Va escribiendo en el archivo el contenido de las posiciones de la MP
	fprintf(archivo, "%s", "El estado de la memoria principal:\n");
	while(i<tamanioMP){
		fprintf(archivo, "%s", "La posicion ");
		fprintf(archivo, "%d", i);
		fprintf(archivo, "%s", " contiene ");
		fprintf(archivo, "%d", MP[i]);
		fprintf(archivo, "%s", " \n");
		i++;
	}
	//obtenerDatosDeMemoria() y mostrar (y,opcional, guardar en archivo)

	fclose(archivo);
}



int validarPosicionVirtual(int posVirtual) {
	int i=0,j=0;
	while(i<cant_tablas){
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].inicio == posVirtual){
						return 0;
					} else {
						j++;
					}
		}
		i++;
	}
	return 1;
}

/*
 * Nombre: crearSegmentoPrograma
 * Argumentos: id_prog, tamanio
 *
 *
 * Devuelve: void
 *
 * Funcion: Dado un id_prog y un tamanio, reserva espacio en MP para el programa
 */

void crearSegmentoPrograma(int id_prog, int tamanio){
	int ubicacion=-1;
	segmentDescriptor aux;
	int i,num_segmento;
	//Escoge la ubicacion en base al algoritmo de config
	if(configuracion_UMV.algoritmo == firstfit)ubicacion = escogerUbicacionF(tamanio);
	if(configuracion_UMV.algoritmo == worstfit)ubicacion = escogerUbicacionW(tamanio);
	reservarEspacioMP(ubicacion, tamanio);
	int pos=inicializarTabla(id_prog);
	i=rand();
	while(!validarPosicionVirtual(i)) rand();//Recorrer la tabla de segmentos validando que ninguno ocupe entre la posicion y la posicion y el tamanio
	//Armado del segmento creado
	aux.ubicacionMP=ubicacion;
	aux.inicio=i;
	aux.tamanio=tamanio;
	//Asignación de los campos de la tabla de segmentos correspondiente
	tablaDeSegmentos[pos].id_prog = id_prog;
	num_segmento=tablaDeSegmentos[pos].cant_segmentos;
	tablaDeSegmentos[pos].cant_segmentos++;
	tablaDeSegmentos[pos].segmentos[num_segmento]=aux;
	printf("El programa es : %d\n", tablaDeSegmentos[pos].id_prog);
	printf("La cantidad de segmentos es: %d\n", tablaDeSegmentos[pos].cant_segmentos);
	printf("La posicion real es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].ubicacionMP);
	printf("La posicion virtual es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].inicio);
	printf("El tamanio es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].tamanio);
}

void reservarEspacioMP(int ubicacion, int tamanio){
	int i=0;
	int aux=tamanio;
	while(i<=tamanioMP && aux>0){
			MP[ubicacion+i]=-1;
			aux--;
			i++;
		}
}

int inicializarTabla(int id_prog){
	int i=0;
	tablaSeg* aux_tabla;
	segmentDescriptor* aux_segmentos;
	//Verifico si hay tablas

	if(cant_tablas==0){
		//Como no hay tablas, la inicializo y asigno la cantidad de segmentos a 0
		aux_tabla = malloc(sizeof(tablaSeg));
			if(aux_tabla==NULL){
				log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
				abort();
			}
		tablaDeSegmentos = aux_tabla;
		tablaDeSegmentos[i].cant_segmentos=0;
		aux_segmentos = malloc(sizeof(segmentDescriptor));
						if(aux_segmentos==NULL){
							log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
							abort();
						}
		tablaDeSegmentos[i].segmentos = aux_segmentos;
		cant_tablas++;
		printf("La cant de tablas es: %d\n", cant_tablas);
		return i;
	}
	else{
		//Como hay tablas, busco si ya hay una para el prog correspondiente
	while (tablaDeSegmentos[i].id_prog != id_prog && i<= cant_tablas) i++;
	if (tablaDeSegmentos[i].id_prog != id_prog){
		//Si no encuentra una tabla para el programa, agrego una tabla e incremento la cantidad total de tablas
		//FIXME: no funciona bien este realloc
		aux_tabla =realloc(tablaDeSegmentos, (sizeof(tablaSeg)*(cant_tablas+1)));
		if(aux_tabla==NULL){
						log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
						abort();
					}
		tablaDeSegmentos = aux_tabla;
		i=cant_tablas;
		aux_segmentos = malloc(sizeof(segmentDescriptor));
								if(aux_segmentos==NULL){
									log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
									abort();
								}
		tablaDeSegmentos[i].segmentos = aux_segmentos;
		tablaDeSegmentos[i].cant_segmentos=0;
		cant_tablas++;
		printf("La cant de tablas es: %d\n", cant_tablas);
		return i;
		} else {
			//FIXME: no funciona bien este realloc
			aux_segmentos = realloc(tablaDeSegmentos[i].segmentos, ((tablaDeSegmentos[i].cant_segmentos+1)*sizeof(segmentDescriptor)));
									if(aux_segmentos==NULL){
										log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
										abort();
									}
			tablaDeSegmentos[i].segmentos = aux_segmentos;
			printf("La cant de tablas es: %d\n", cant_tablas);
			return i;
		}
	}
}

int escogerUbicacionF(int tamanio){
	int posicionDeDestino;
	int i=0;
	int aux;
			//Recorro la memoria principal
		while(i<tamanioMP){
			 aux=0;			//Obtengo primer posicion libre en MP
			while (MP[i]!=NULL && i<tamanioMP) i++;
			posicionDeDestino= i;
			//Checkeo la cantidad de posiciones libres hasta llegar a
			//una posicion ocupada, o el final de la memoria
			while(MP[i] == NULL && i<tamanioMP){
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
				while(MP[i]== NULL && i<tamanioMP){
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

/*
 * Nombre: destruirSegmentosPrograma
 * Argumentos: id_prog
 *
 *
 * Devuelve: void
 *
 * Funcion: Dado un id_prog, libera el espacio reservado para el programa
 */

void destruirSegmentosPrograma(int id_prog){
	int pos= getPosTabla(id_prog);
	printf("La posicion es: %d\n", pos);
	liberarMP(pos);
	printf("Se libera el espacio de memoria\n");
	eliminarSegmentos(pos);
	printf("Se libera el espacio de los segmentos\n");
	return;

}

int getPosTabla(int id_prog){
	int i=0;
	while(tablaDeSegmentos[i].id_prog != id_prog) i++;
	return i;
}


void liberarMP(int pos){
	int i,ubicacion,tam;
	i=0;
	while(i<tamanioMP){

		//Recorro las posiciones ocupadas de la tabla de segmentos y obtengo la
		//ubicacionMP y tamanio
			ubicacion=tablaDeSegmentos[pos].segmentos[i].ubicacionMP;
			tam=tablaDeSegmentos[pos].segmentos[i].tamanio;
		//Recorro las posiciones de MP en base a ubicacionMP y tamanio
		// y las igualo a NULL
			while(tam!=0){
				MP[ubicacion]=NULL;
				ubicacion++;
				tam--;
			}
			i++;
	}

}

void eliminarSegmentos(int pos){
	int i,ultimaPos;
	i=0;
	ultimaPos= (tablaDeSegmentos[pos].cant_segmentos-1);
	//Recorro la tabla de segmentos del id_prog
	while(i<=ultimaPos){
		//Por cada posicion ocupada, libero el espacio de memoria
		free(tablaDeSegmentos[pos].segmentos);
		tablaDeSegmentos[pos].cant_segmentos=0;
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
		printf("Se recibio bien el paquete\n");
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


void inicializarMutex(void){
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
	int procesoDelHilo,unaBase,unOffset,unTamanio;
	t_buffer buffer;
	puts("Ingrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	scanf("%s",&comando);
	while(estaEnDicOP(comando)== 0){
		puts("\nOperacion erronea, escriba la operacion de nuevo");
		scanf("%s",&comando);
	}

	while(strcmp(comando, "exit") != 0){
			if(strcmp(comando, "operacion") == 0){

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
					  solicitarBytes(unaBase,unOffset,unTamanio);
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
					  puts("Ingrese el processID de programa a usar");
				 	  scanf("%d",&procesoDelHilo);
					  puts("Ingrese el tamaño del segmento");
					  int tamanio;
					  scanf("%d",&tamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  crearSegmentoPrograma(procesoDelHilo,tamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "destruir") == 0){
					  puts("Ingrese el processID de programa a usar");
					  scanf("%d",&procesoDelHilo);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  destruirSegmentosPrograma(procesoDelHilo);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
			}

	else { if (strcmp(comando, "retardo") == 0){
				  puts("Ingrese el valor de retardo en Milisegundos");
				  int valorRetardo;
				  scanf("%d", valorRetardo);
				  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				  retardo= valorRetardo;
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
				   dump();
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
			free(tablaDeSegmentos);
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

void destruirTodosLosSegmentos(void){/*
	int i=sizeof(tablaDeSegmentos);

	return;*/
}




