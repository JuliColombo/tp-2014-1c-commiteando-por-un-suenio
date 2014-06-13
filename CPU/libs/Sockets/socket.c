/*
 * socket.c
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */
#include "socket.h"

/*
 * Nombre: socket_crearCliente/0
 * Argumentos:
 * 		- NINGUNO
 *
 * Devuelve:
 * 		int (Descriptor al socket creado), en caso de error, devuelve -1.
 *
 *
 * Funcion: Crea el socket para un cliente.
 */
int socket_crearCliente(void){

	int sockfd;

	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Error al crear socket");//Crear log para este error.
		return -1;
	}

	return sockfd;

}

/*Nombre: socket_conectarCliente/3
 * Argumentos:
 * 		- sockfd (int), (descriptor del socket cliente).
 * 		- serverIp (char*),(IP del server a conectar)
 * 		- serverPort (int), (puerto del server a conectar)
 *
 * Devuelve:
 * 		int (Descriptor al socket que se va a conectar, devuelve -1 si hay error).
 *
 * Funcion: Conectarme a un server con su IP y puerto.
 *
 */
int socket_conectarCliente(int sockfd,char *serverIp, int serverPort){

		struct sockaddr_in socketInfo;

		//INICIALIZACION DE SOCKETINFO
		socketInfo.sin_family = AF_INET;
		socketInfo.sin_port = htons(serverPort); //host to network short
		socketInfo.sin_addr.s_addr = inet_addr(serverIp);
		memset(&(socketInfo.sin_zero),'\0',8); // PONGO A 0 EL RESTO DE LA ESTRUCTURA
		// ME CONECTO CON LA DIRECCIÓN DE SOCKETINFO
		//SIEMPRE VERIFICANDO QUE NO DEN -1 LAS FUNCIONES O 0 EN CASO DE RECV() -- SOLO PARA SERVER IGUAL :)

		if(connect(sockfd , (struct sockaddr *)&socketInfo , sizeof(socketInfo)) == -1){
			perror("Falló la conexión"); // Cambiar esto por un log.
			return -1;
		}

		return sockfd;
}

/*Nombre: socket_crearYConectarCliente/2
 * Argumentos:
 * 		- serverIp (char*),(IP del server a conectar)
 * 		- serverPort (int), (puerto del server a conectar)
 *
 * Devuelve:
 * 		int (Descriptor al socket que se va a conectar).
 *
 * Funcion: Crear y conectar un nuevo cliente a un server con su IP y puerto.
 *
 */
int socket_crearYConectarCliente(char *serverIp, int serverPort){
	int sockfd;
	sockfd = socket_crearCliente();
	if (sockfd < 0)
		return -1;

	sockfd = socket_conectarCliente( sockfd,(char*)serverIp, serverPort);

	return sockfd;
}

/*Nombre: socket_crearServidor/2
 * Argumentos:
 * 		- serverIp (char*),(IP del server)
 * 		- serverPort (int), (puerto del server)
 *
 * Devuelve:
 * 		int (Descriptor al socket del server).
 *
 * Funcion: Crear un nuevo servidor.
 *
 */
int socket_crearServidor(char *ip, int port){
	int socketEscucha;
	struct sockaddr_in miSocket;//ESTE ES EL SOCKET CON LA DRECCION IP

	if((socketEscucha = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Error al crear socket");
		return -1;
	}

	miSocket.sin_family = AF_INET;
	miSocket.sin_port = htons(port);
	miSocket.sin_addr.s_addr = inet_addr(ip);
	memset(&(miSocket.sin_zero),'\0',8); // PONGO A 0 EL RESTO DE LA ESTRUCTURA

	int yes = 1;
	if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if(bind(socketEscucha,(struct sockaddr*)&miSocket, sizeof(miSocket)) == -1){
		perror ("Error al bindear el socket escucha");
		return -1;
	}

	if (listen(socketEscucha, MAX_CONNECTION_SERVER) == -1){
		perror("Error en la puesta de escucha");
		return -1;
	}

	return socketEscucha;

}


int socket_crearServidorPuertoRandom(char *ip, int * port){
	int socketEscucha;
	struct sockaddr_in miSocket;//ESTE ES EL SOCKET CON LA DRECCION IP

	if((socketEscucha = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Error al crear socket");
		return -1;
	}

	miSocket.sin_family = AF_INET;
	miSocket.sin_port = htons(0);
	miSocket.sin_addr.s_addr = inet_addr(ip);
	memset(&(miSocket.sin_zero),'\0',8); // PONGO A 0 EL RESTO DE LA ESTRUCTURA

	int yes = 1;
	if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if(bind(socketEscucha,(struct sockaddr*)&miSocket, sizeof(miSocket)) == -1){
		perror ("Error al bindear el socket escucha");
		return -1;
	}

	if (listen(socketEscucha, MAX_CONNECTION_SERVER) == -1){
		perror("Error en la puesta de escucha");
		return -1;
	}

	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(socketEscucha, (struct sockaddr *)&sin, &len) == -1){
		perror("getsockname");
		return -1;
	}

	*port = ntohs(sin.sin_port);

	return socketEscucha;
}

/*Nombre: socket_aceptarCliente/1
 * Argumentos:
 * 		- socketEscucha (int),(descriptor del socket del server para escuchar conexiones)
 *
 * Devuelve:
 * 		int (Descriptor al socket de la nueva conexión).
 *
 * Funcion: Aceptar un cliente que está siendo previamente escuchado.
 *
 */
int socket_aceptarCliente(int socketEscucha){
	int socketNuevaConexion;
	unsigned int size_sockAddrIn;

	struct sockaddr_in suSocket;

	size_sockAddrIn = sizeof(struct sockaddr_in);
	socketNuevaConexion = accept(socketEscucha, (struct sockaddr *)&suSocket, &size_sockAddrIn);

	if(socketNuevaConexion < 0) {

		perror("Error al aceptar conexion entrante");
		return -1;

	}

	return socketNuevaConexion;

}


/*
 * Nombre: socket_enviar/3
 * Argumentos:
 * 		- socketReceptor
 * 		- tipo: (unsigned char) tipo de socket
 * 		- estructura (void *) (lo que quiero enviar)
 * 		- tipoEstructura (int que define qué estructura quiero enviar)
 *
 * Devuelve:
 * 		int (1->si se envio o false->error al envio).
 * 		--> convierte la estructura a un buffer transferible, y lo envia.
 *
 * Funcion: paquetiza y envia la estructura, convierte la estructura a un buffer transferible y la envia
 */
int socket_enviar(int socketReceptor, t_estructura tipoEstructura, void* estructura){
	int cantBytesEnviados;

	t_stream * paquete = serializar(tipoEstructura, estructura);

	cantBytesEnviados = send(socketReceptor, paquete->buffer, paquete->length, 0);
	free(paquete->buffer);
	free(paquete);
	if( cantBytesEnviados == -1){
		perror("Server no encontrado\n");
		return 0;
	}
	else {
		return 1;
	}
}

/*
 * Nombre: socket_recibir/3
 * SINTAXIS CORRECTA: socket_recibir(soquetEmisor, &tipoRecibido, &PunteroAEstructuraRecibida)
 * NOTA: El segudno y tercer parametro son por referencia. Los modifica en la funcion.
 * Admite que se mande NULL en cualquiera de los dos, si no interesa uno de los datos.
 * Argumentos:
 * 		- socketEmisor
 * 		- tipoEstructura: (t_tipoEstructura *) puntero a la variable tipo del paquete
 * 		- estructura (void **) puntero a una variable tipo void*
 *
 * Devuelve:
 * 		int (1-> se recibio ok, 0-> si hubo problemas).
 *
 * Funcion: recibir y despaquetizar, convierte el paquete recibido a la estructura que corresponda.
 */
int socket_recibir(int socketEmisor, t_estructura * tipoEstructura, void** estructura){
	int cantBytesRecibidos;
	t_header header;
	char* buffer;
	char* bufferHeader;

	bufferHeader = malloc(sizeof(t_header));

	cantBytesRecibidos = recv(socketEmisor, bufferHeader, sizeof(t_header), MSG_WAITALL);	//Recivo por partes, primero el header.
	if(cantBytesRecibidos == -1){
		free(bufferHeader);
		perror("Error al recibir datos\n");
		return 0;
	}

	header = despaquetizarHeader(bufferHeader);
	free(bufferHeader);

	if (tipoEstructura != NULL) {
		*tipoEstructura = header.Type;
	}

	if(header.Lenght == 0){	//Si recivo mensaje con length 0 retorno 1 y *estructura NULL.
		if (estructura != NULL) {
			*estructura = NULL;
		}
		return 1;
	}

	buffer = malloc(header.Lenght);
	cantBytesRecibidos = recv(socketEmisor, buffer, header.Lenght, MSG_WAITALL);	//Recivo el resto del mensaje con el tamaño justo de buffer.
	if(cantBytesRecibidos == -1){
		free(buffer);
		perror("Error al recibir datos\n");
		return 0;
	}

	if(estructura != NULL) {
		*estructura = deserializar(header.Type, buffer, header.Lenght);
	}

	free(buffer);

	return 1;
}




char* socket_ip(char* direccionCompleta){
	char * dir = string_duplicate(direccionCompleta);
	string_append(&dir,"basura"); // Le agrego al final cualquier cosa, cuestion de que si me mandan "127.0.0.1:", pueda dividirlo correctamente
	char * resultado = ( string_split(dir, ":") )[0]; //Divido en el ":", en un array de char* y digo que me de la primera parte.
	free(dir);
	return resultado;
}

int socket_puerto(char* direccionCompleta){
	char * dir = string_duplicate("basura");
	string_append(&dir,direccionCompleta); // Le agrego al principio cualquier cosa, cuestion de que si me mandan ":2532", pueda dividirlo correctamente
	int resultado = atoi(( string_split(dir, ":") )[1]);	//Divido en el ":", en un array de char* y digo que me de la segunda parte en forma de int.
	free(dir);
	return resultado;
}

char* socket_unirDireccion(char* ip, int puerto){
	return string_from_format("%s:%d", ip, puerto);
}

int socket_enviarSignal(int socketReceptor, t_signal signal){
	struct_signal *structSignal = malloc(sizeof(struct_signal));

	structSignal->signal = signal;

	int seEnvio = socket_enviar(socketReceptor, STRUCT_SIGNAL, (void*)structSignal);

	free(structSignal);
	return seEnvio;
}

int socket_recibirSignal(int socketEmisor, t_signal *signal){
	void * estructuraRecibida;
	t_estructura tipoRecibido;

	int recibio = socket_recibir(socketEmisor,&tipoRecibido, &estructuraRecibida);
	if(!recibio) {
		*signal = S_ERROR;
		return 0;
	}

	if (tipoRecibido != STRUCT_SIGNAL){
		*signal = S_ERROR;
		return 0;
	}

	*signal = ((struct_signal*) estructuraRecibida)->signal;
	free(estructuraRecibida);
	return 1;
}

/*
 * Nombre: socket_cerrarConexion/1
 *
 * Argumentos:
 * 		- socket
 *
 * Devuelve:
 * 		int (-1-> si se cerro ok, 0-> si hubo problemas).
 *
 * Funcion: cerrar el socket
 */
int socket_cerrarConexion(int socket){
	return close(socket);
}

/****************************** SOCKETS DE CPU ******************************************/

void socket_and_push(int sockAjeno,t_puntero posicionEnPila, int valorAPushear) {
	//En UMV PUSH_SIZE_CHECK(&valorAPushear,pila,posicionEnPila)
	struct_push* estructura = crear_struct_push(posicionEnPila,valorAPushear);
	socket_enviar(sockAjeno,STRUCT_PUSH,estructura);
	free(estructura);
}

void socket_and_pop_position(int sockAjeno, t_puntero posicionDePila) {
	//En UMV POP_SIZE_CHECK(pila, posicionDePila)
	struct_pop_position* estructura = crear_struct_pop_position(posicionDePila);
	socket_enviar(sockAjeno,STRUCT_POP_POSITION,estructura);
	free(estructura);
}
