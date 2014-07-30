/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivas.h"




t_puntero definirVariable(t_nombre_variable identificador_variable) {
	if (SEG_flag == 1)
		return 0;
	printf("definirVariable\n");
	int recepcion;
	//Se arma la estructura de definir_variable con lo recibido
	t_struct_env_bytes * definir_variable = malloc(sizeof(t_struct_env_bytes));
	definir_variable->base = var_seg_stack;
	definir_variable->offset = temp_cursor_stack + 5 * var_tamanio_contexto;
	definir_variable->tamanio = sizeof(t_nombre_variable) + sizeof(t_valor_variable);
	//Se agrega la variable en el diccionario
	char key = identificador_variable;
	char keyABuscar[2] = { key, '\0' };

	definir_variable->buffer = &(keyABuscar[0]);
	key = *((char*) definir_variable->buffer);
	socket_enviar(sockUMV, D_STRUCT_ENV_BYTES, definir_variable);

	int* posicion_variable = malloc(sizeof(int));
	*posicion_variable = var_tamanio_contexto + 1;

	dictionary_put(dicc_variables, keyABuscar, posicion_variable);

	/*probamos si el valor que guardo en el diccionario de variables sea el correcto
	 int direccion;
	 void* posicion2=malloc(sizeof(int));
	 posicion2=dictionary_get(dicc_variables,&identificador_variable);
	 memcpy(&direccion,posicion2,sizeof(int));
	 printf("la variable definida esta en la posicion # :%d\n",direccion);
	 */

	free(definir_variable);
	//free(posicion_variable);
	void * structRecibido;
	t_tipoEstructura tipoStruct;

	socket_recibir(sockUMV, &tipoStruct, &structRecibido);

	if (tipoStruct != D_STRUCT_NUMERO) {
		printf("Respuesta en desreferenciar incorrecta\n");
		return 0;
	}
	recepcion = ((t_struct_numero*) structRecibido)->numero;
	excepcion_UMV(recepcion);
	var_tamanio_contexto++; //aumentamos el tamanio de contexto
	free(structRecibido);
	return obtenerPosicionVariable(keyABuscar[0]); //mejor que devuelva el desplazamiento dentro del stack y no la direccion por que el stack esta en otro proceso
}


t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	if (SEG_flag == 1)
			return 0;
		//Busca la variable en el diccionario
		printf("obtenerPosicionVariable\n");
		char key = identificador_variable;
		char keyABuscar[2] = { key, '\0' };

		int posicion_variable;
		//void* posicion=malloc(sizeof(int));
		int * posicion = dictionary_get(dicc_variables, keyABuscar);
	//	posicion_variable=(temp_cursor_stack + orden_var*5)-4; // aca sacamos el desplazamiento dentro del stack para el valor de una variable

		posicion_variable = ((*posicion) * 5) - 4;

		if (posicion_variable >= 0)
			return posicion_variable + temp_cursor_stack;
		else
			return -1;
}


t_valor_variable dereferenciar(t_puntero direccion_variable) {
	if (SEG_flag == 1)
			return 0;
		int valor;
		t_struct_sol_bytes * desreferenciar = malloc(sizeof(t_struct_sol_bytes));
		//Arma la estructura para la solicitud de bytes
		desreferenciar->base = var_seg_stack;
		desreferenciar->offset = direccion_variable;
		desreferenciar->tamanio = sizeof(int);

		socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, desreferenciar);

		void * structRecibido;
		t_tipoEstructura tipoStruct;

		socket_recibir(sockUMV, &tipoStruct, &structRecibido);
		//Valida la respuesta de la umv
		if (tipoStruct != D_STRUCT_RESPUESTA_UMV) {
			printf("Respuesta en dereferenciar incorrecta\n");
			return 0;
		}

		int temp_tamanio;
		void * temp_buffer;
		//Arma una estructura temporal con lo recibido de la umv
		temp_buffer = ((t_struct_respuesta_umv*) structRecibido)->buffer;
		temp_tamanio = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer;

		memcpy(&valor, temp_buffer, temp_tamanio);
		int tamanio_instruccion;
		tamanio_instruccion = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer;

		if (tamanio_instruccion == sizeof(int)) {
			int*respuesta = malloc(sizeof(int));
			memcpy(respuesta, ((t_struct_respuesta_umv*) structRecibido)->buffer, tamanio_instruccion);
			int valor1 = *respuesta;
			if (valor1 < 0) {
				excepcion_UMV(0);
				return 0;
			}

		}
		printf("dereferenciar\n");
		//Se libera espacio alocado
		free(desreferenciar);
		free(temp_buffer);
		free(structRecibido);
		return valor;

}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {

	if (SEG_flag == 1)
			return;
		t_valor_variable temp_valor = valor;
		int recepcion;
		//Arma y envia un envio de bytes a la umv
		t_struct_env_bytes * asignar = malloc(sizeof(t_struct_env_bytes));
		asignar->base = var_seg_stack;
		asignar->offset = direccion_variable;
		asignar->tamanio = sizeof(int);
		void* temp_buffer = malloc(sizeof(t_valor_variable));
		memcpy(temp_buffer, &temp_valor, sizeof(int));
		asignar->buffer = temp_buffer;

		socket_enviar(sockUMV, D_STRUCT_ENV_BYTES, asignar);
		//Recibe la respuesta de la umv
		void * structRecibido;
		t_tipoEstructura tipoStruct;

		socket_recibir(sockUMV, &tipoStruct, &structRecibido);
		//Valida la respuesta de la umv
		if (tipoStruct != D_STRUCT_NUMERO) {
			printf("Respuesta en asignar incorrecta\n");
		}
		recepcion = ((t_struct_numero*) structRecibido)->numero;
		excepcion_UMV(recepcion);
		//Se libera espacio alocado
		free(asignar);
		free(temp_buffer);
		free(structRecibido);
		printf("asignar\n");
}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {

	if (SEG_flag == 1)
			return 0;
		int valorComp;
		char** partes = string_split(variable, "\n");
		variable = partes[0];
		//Se envia una solicitud de obtener valor al kernel
		t_struct_string * nombreVarComp = malloc(sizeof(t_struct_string));
		nombreVarComp->string = strdup(variable);
		socket_enviar(sockKernel, D_STRUCT_OBTENER_VALOR, nombreVarComp); // envio solicitud de valor con nombre de variable
		//Se recibe la respuesta del kernel
		void* structRecibido;
		t_tipoEstructura tipoRecibido;
		socket_recibir(sockKernel, &tipoRecibido, &structRecibido); // recibo el valor
		//Se valida la respuesta
		if (tipoRecibido != D_STRUCT_NUMERO) {
			printf("%d\n", tipoRecibido);
			return 0;
		}
		valorComp = ((t_struct_numero*) structRecibido)->numero;
		printf("obtenerValorCompartida\n");
		//Se libera el espacio alocado
		free(nombreVarComp);
		free(structRecibido);
		free(partes[0]);
		free(partes[1]);
		free(partes);
		return valorComp;


}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

	if (SEG_flag == 1)
			return 0;
		char** partes = string_split(variable, "\n");
		variable = partes[0];
		//Arma estructura para solicitar asignar valor al kernel
		t_struct_int_char * asignarValor = malloc(sizeof(t_struct_int_char));
		asignarValor->numero = valor;
		asignarValor->string = variable;
		asignarValor->tamano = strlen(variable) + 1;
		//Envia solicitud de asignar valor al kernel
		socket_enviar(sockKernel, D_STRUCT_ASIGNAR_VALOR, asignarValor);

		void* structRecibido;
		t_tipoEstructura tipoRecibido;
		socket_recibir(sockKernel, &tipoRecibido, &structRecibido); // recibo el valor
		//Recibe respuesta del kernel
		printf("asignarValorCompartida\n");
		//Se libera el espacio alocado
		free(asignarValor);
		free(partes[0]);
		free(partes[1]);
		free(partes);
		return valor;
}

void irAlLabel(t_nombre_etiqueta etiqueta) {
	if (SEG_flag == 1)
			return;
		char** partes = string_split(etiqueta, "\n");
		etiqueta = partes[0];
		//Controla si el diccionario es vacio
		if (dicc_etiquetas == NULL ) {
			//Si es vacio, solicita el indice de etiquetas a la umv
			t_struct_sol_bytes * sol_ind_etiquetas = malloc(sizeof(t_struct_sol_bytes)); //primero debe solicitar nuevamente el indice de etiquetas a la UMV
			sol_ind_etiquetas->base = var_ind_etiquetas;
			sol_ind_etiquetas->offset = 0;
			sol_ind_etiquetas->tamanio = var_tamanio_etiquetas;
			socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, sol_ind_etiquetas);
			free(sol_ind_etiquetas);
			void * structRecibido;
			t_tipoEstructura tipoStruct;
			//Recibe respuesta de la UMV
			socket_recibir(sockUMV, &tipoStruct, &structRecibido);
			//Valida respuesta de la UMV
			if (tipoStruct != D_STRUCT_RESPUESTA_UMV) {
				printf("Tipo incorrecto\n");
				free(structRecibido);
				return;
			}
			uint32_t temp_tamanio;
			void * temp_buffer;
			//Arma el diccionario con lo recibido de la UMV
			temp_buffer = ((t_struct_respuesta_umv*) structRecibido)->buffer;
			temp_tamanio = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer;
			dicc_etiquetas = malloc(temp_tamanio);
			int tamanio_instruccion;
			tamanio_instruccion = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer;

			if (tamanio_instruccion == sizeof(int)) {
				int*respuesta = malloc(sizeof(int));
				memcpy(respuesta, ((t_struct_respuesta_umv*) structRecibido)->buffer, tamanio_instruccion);
				int valor1 = *respuesta;
				if (valor1 < 0) {
					excepcion_UMV(0);
					return;
				}

			}

			memcpy(dicc_etiquetas, temp_buffer, temp_tamanio); // aca dicc_etiquetas me cargo con el char* IMPRIMIR nada mas
			free(temp_buffer);
			free(structRecibido);
		}													 //NO TENDRIA QUE SER TODAS LAS ETIQUETAS?

		printf("irAlLabel\n");

		temp_counter = metadata_buscar_etiqueta(etiqueta, dicc_etiquetas, var_tamanio_etiquetas); //se asigna al program counter
		temp_counter = temp_counter - 1;
		//Se libera espacio alocado
		free(partes[0]);
		free(partes[1]);
		free(partes);
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
//
//	reservarContextoSinRetorno();
//
//	int posicionAPushear =  top_index +1;
//	//*pcb->c_stack = posicionAPushear;
//	cursor = posicionAPushear;
//
//	irAlLabel(etiqueta);
//
//	pcb->tamanio_contexto = 0;
//
//	esConRetorno = 0;

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
//
//	reservarContextoConRetorno(donde_retornar);
//
//	int posicionAPushear = top_index +1;
//	//*pcb->c_stack = posicionAPushear;
//	cursor = posicionAPushear;
//
//	irAlLabel(etiqueta);
//
//	pcb->tamanio_contexto = 0;
}


void finalizar() {
//	t_puntero c_stack_viejo;
//
//	if(!esPrimerContexto()) {
//
//	recuperarPosicionDeDirecciones();
//	volverAContextoAnterior(&c_stack_viejo);
//
//	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);
//
//	//*pcb->c_stack = c_stack_viejo;
//	cursor = c_stack_viejo;
//
//	regenerarDiccionario(tamanio);
//	}
//
//	if(esPrimerContexto()) {
//		termino = DONE;
//
//		log_escribir(archLog, "Ejecucion", INFO, "Finalizo ejecucion");
//
//	}
}


void retornar(t_valor_variable retorno) {

//	recuperarPosicionDeDirecciones();
//
//	t_puntero direccionRetorno;
//	recuperarDireccionRetorno(&direccionRetorno);
//
//	t_puntero c_stack_viejo;
//	volverAContextoAnterior(&c_stack_viejo);
//
//	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);
//
//	//*pcb->c_stack = c_stack_viejo;
//	cursor = c_stack_viejo;
//
//	regenerarDiccionario(tamanio);
//
//	t_puntero posicionAsignacion = direccionRetorno + 1;
//
//	t_struct_push* estructura = malloc(sizeof(t_struct_push));
//	estructura->posicion=posicionAsignacion;
//	estructura->valor = retorno;
//	estructura->stack_base = pcb->stack;
//	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
//	free(estructura);
//
//	chequearSiHuboSF();
//
//	esConRetorno = 0;
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se retorna el valor %d",retorno);

}

void imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernnel, para que termine siendo mostrado en la consola del Programa en ejecución.

//	//DESPUES SE HACE FREE DE ESTO?
//	char* valor_variable = string_itoa(valor_mostrar);
//
//	t_struct_nombreMensaje* estructura = malloc(sizeof(t_struct_nombreMensaje));
//	estructura->mensaje = valor_variable;
//	estructura->pid = pcb->pid;
//	socket_enviar(sockKernel,D_STRUCT_NOMBREMENSAJE,estructura);
//	free(estructura);
//
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %d para imprimirlo por pantalla",valor_mostrar);

}

void imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.
//
//	t_struct_nombreMensaje* estructura = malloc(sizeof(t_struct_nombreMensaje));
//	estructura->mensaje = texto;
//	estructura->pid = pcb->pid;
//	socket_enviar(sockKernel,D_STRUCT_NOMBREMENSAJE,estructura);
//	free(estructura);
//
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %s para imprimirlo por pantalla",texto);

}

void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
//
//	termino = IO;
//
//	t_struct_pcb_io* pcb_actualizada=malloc(sizeof(t_struct_pcb_io));
//
//	pcb_actualizada->c_stack=pcb->c_stack;
//	pcb_actualizada->codigo=pcb->codigo;
//	pcb_actualizada->index_codigo=pcb->index_codigo;
//	pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
//	pcb_actualizada->pid=pcb->pid;
//	pcb_actualizada->program_counter=pcb->program_counter;
//	pcb_actualizada->stack=pcb->stack;
//	pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
//	pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
//	pcb_actualizada->tiempo = tiempo;
//	pcb_actualizada->dispositivo = dispositivo;
//
//	socket_enviar(sockKernel,D_STRUCT_PCBIO,pcb_actualizada);
//
//	free(pcb_actualizada);
//	free(pcb);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se conecto %s por %d tiempo",dispositivo,tiempo);

}

/****************************** OPERACIONES DE KERNEL ************************************************/

void wait_ansisop(t_nombre_semaforo identificador_semaforo) {

//	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
//	estructura->nombre_semaforo = identificador_semaforo;
//	socket_enviar(sockKernel, D_STRUCT_WAIT, estructura);
//	free(estructura);
//
//	controlarBloqueo(sockKernel, termino);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (wait)",identificador_semaforo);

}

void signal_ansisop(t_nombre_semaforo identificador_semaforo) {
//
//	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
//	estructura->nombre_semaforo = identificador_semaforo;
//	socket_enviar(sockKernel, D_STRUCT_SIGNALSEMAFORO, estructura);
//	free(estructura);
//
//	controlarBloqueo(sockKernel, termino);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (signal)",identificador_semaforo);
}

