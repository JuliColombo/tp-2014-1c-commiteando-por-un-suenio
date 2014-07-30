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
	if (SEG_flag == 1)
			return;
		printf("llamasSinRetorno\n");
		char** partes = string_split(etiqueta, "\n");
		etiqueta = partes[0];
		//Se arma estructura para solicitar envio de bytes a la UMV
		t_struct_env_bytes * send_punteros = malloc(sizeof(t_struct_env_bytes));
		send_punteros->base = var_seg_stack;
		send_punteros->offset = temp_cursor_stack + var_tamanio_contexto * 5;
		send_punteros->tamanio = 2 * sizeof(int);
		void * temp_buffer = malloc(2 * sizeof(int));
		int offset;
		memcpy(temp_buffer, &temp_cursor_stack, offset = sizeof(int));
		memcpy(temp_buffer + offset, &temp_counter, sizeof(int));
		send_punteros->buffer = temp_buffer;
		//Se envia solicitud de envio de bytes a la UMV
		socket_enviar(sockUMV, D_STRUCT_ENV_BYTES, send_punteros);

		void * structRecibido;
		t_tipoEstructura tipoStruct;
		//Recibe respuesta de la UMV
		socket_recibir(sockUMV, &tipoStruct, &structRecibido);
		//Valida respuesta
		if (tipoStruct != D_STRUCT_NUMERO) {
			printf("Respuesta en desreferenciar incorrecta\n");
			//return 0;
		}
		int recepcion = ((t_struct_numero*) structRecibido)->numero;
		excepcion_UMV(recepcion);
		temp_cursor_stack = temp_cursor_stack + (var_tamanio_contexto * 5) + 8;

		irAlLabel(etiqueta); //le asignamos al program_counter la proxima ejecucion a ejecutar dentro del procedimiento

		dictionary_clean_and_destroy_elements(dicc_variables, free);
		var_tamanio_contexto = 0;
		//Se libera espacio alocado
		free(temp_buffer);
		free(send_punteros);
		free(partes[0]);
		free(partes[1]);
		free(partes);

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
	if (SEG_flag == 1)
			return;
		char** partes = string_split(etiqueta, "\n");
		etiqueta = partes[0];
		free(partes);
		//Se arma estructura para solicitar envio de bytes a la UMV
		t_struct_env_bytes * send_punteros = malloc(sizeof(t_struct_env_bytes));
		send_punteros->base = var_seg_stack;
		send_punteros->offset = temp_cursor_stack + var_tamanio_contexto * 5;
		send_punteros->tamanio = 3 * sizeof(int);
		void* temp_buffer = malloc(3 * sizeof(int));
		int offset;
		//temp_counter=temp_counter+1;
		memcpy(temp_buffer, &temp_cursor_stack, offset = sizeof(int));
		memcpy(temp_buffer + offset, &temp_counter, sizeof(int));
		memcpy(temp_buffer + 2 * offset, &donde_retornar, sizeof(int));
		send_punteros->buffer = temp_buffer;
		//Se envia solicitud de envio de bytes a la UMV
		socket_enviar(sockUMV, D_STRUCT_ENV_BYTES, send_punteros);

		void * structRecibido;
		t_tipoEstructura tipoStruct;
		//Se recibe respuesta de la UMV
		socket_recibir(sockUMV, &tipoStruct, &structRecibido);
		//Se valida la respuesta
		if (tipoStruct != D_STRUCT_NUMERO) {
			printf("Respuesta en desreferenciar incorrecta\n");
			//return 0;
		}
		int recepcion = ((t_struct_numero*) structRecibido)->numero;
		excepcion_UMV(recepcion);
		temp_cursor_stack = temp_cursor_stack + (var_tamanio_contexto * 5) + 3 * sizeof(int);
		irAlLabel(etiqueta);
		dictionary_clean_and_destroy_elements(dicc_variables, free);
		var_tamanio_contexto = 0;
		printf("llamarConRetorno\n");
		//Se libera espacio alocado
		free(send_punteros);
		free(temp_buffer);
}


void finalizar() {
	if (SEG_flag == 1)
			return;
		printf("finalizar\n");

		uint32_t prog_counter_ant; //vamos a pedir los 8 bytes que guardamos en el stack para volver al cntxt anterior.
		uint32_t cursor_contexto_ant;
		uint32_t cant_vars_ant;
		//Se checkea el cursor del stack
		if (temp_cursor_stack == 0) {
			//Si es 0
			int cant_var = var_tamanio_contexto;
			int i = 0;
			int incremento = 0;
			while (i < cant_var) {						//aca deberiamos imprimir las variables con sus valores.
				//Se arma estructura de solicitar bytes para la UMV
				t_struct_sol_bytes * sol_var = malloc(sizeof(t_struct_sol_bytes));

				sol_var->base = var_seg_stack;
				sol_var->offset = 0 + incremento;
				sol_var->tamanio = sizeof(char) + sizeof(int);
				//Se envia solicitar bytes a la UMV
				socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, sol_var);

				void * structRecibido;
				t_tipoEstructura tipoStruct;
				//Se recibe respuesta de la UMV
				socket_recibir(sockUMV, &tipoStruct, &structRecibido);
				//Se valida la respuesta
				if (tipoStruct != D_STRUCT_RESPUESTA_UMV) {
					printf("Respuesta en finalziar (por parte de UMV) incorrecta\n");
					//return 0;
				}
				int offset;
				int* valor_var = malloc(sizeof(int));
				char* nombre_var = malloc(sizeof(char));

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

				memcpy(nombre_var, ((t_struct_respuesta_umv*) structRecibido)->buffer, offset = sizeof(char));
				memcpy(valor_var, ((t_struct_respuesta_umv*) structRecibido)->buffer + offset, sizeof(int));

				char key = *nombre_var;
				char keyaImprimir[3] = { key, ':', '\0' };
				int valoraImprimir = *valor_var;

				imprimirTexto(keyaImprimir);
				imprimir(valoraImprimir);

				i++;
				incremento = incremento + (sol_var->tamanio);
				//Se libera espacio alocado
				free(((t_struct_respuesta_umv*) structRecibido)->buffer);
				free(structRecibido);
				free(sol_var);
				free(valor_var);
				free(nombre_var);

			}

			//Enviamos el PCB actualizado
			t_struct_pcb * PCB_finalizado = malloc(sizeof(t_struct_pcb));
			PCB_finalizado->pid = temp_id;
			PCB_finalizado->c_stack = temp_cursor_stack;
			PCB_finalizado->index_codigo = temp_ind_codigo;
			PCB_finalizado->index_etiquetas = var_ind_etiquetas;
			PCB_finalizado->program_counter = 0;
			PCB_finalizado->codigo = temp_seg_codigo;
			PCB_finalizado->stack = var_seg_stack;
			PCB_finalizado->tamanio_contexto = var_tamanio_contexto;
			PCB_finalizado->tamanio_indice = var_tamanio_etiquetas;
			//PCB_finalizado->estado_finalizacion = 1; TODO: ver esto!!!

			socket_enviar(sockKernel, D_STRUCT_PCB, PCB_finalizado);
			free(PCB_finalizado);
			fin_PCB = 1;
			log_escribir(archLog, "END: El PCB id: %d ha finalizado su ejecucion",INFO, temp_id);
		}

		else {

			//prueba fallida de invocacion a retornar, no parece funcionar por q el finalizar tiene 2 punteros a retornar, el retornar tiene 3
			//int no_return = -1;  // verificar si -1 es un posible valor de retorno en ansisop, de ser posible... buscar otro valor que sirve como filtro

			//retornar(no_return);   // pasando como para metro un null, evito la asignacion del retornar, y como aparte de eso, el retornar haria exactamente lo mismo, evito la repeticion de codigo.

			//Se arma estructura para solicitar bytes en la UMV
			t_struct_sol_bytes * punts_contx_ant = malloc(sizeof(t_struct_sol_bytes));
			punts_contx_ant->base = var_seg_stack;
			//punts_contx_ant->offset=(temp_cursor_stack-8)-var_seg_stack; //restamos el cursor actual - 8(xq es el tamaño de los punteros q guarde en la umv) - el inicio del stack.
			punts_contx_ant->offset = (temp_cursor_stack - (2 * sizeof(uint32_t)));		//var_tamanio_contexto*5;
			punts_contx_ant->tamanio = 2 * sizeof(uint32_t);
			//Se envia la solicitud de bytes a la UMV
			socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, punts_contx_ant);
			void * structRecibido;
			t_tipoEstructura tipoStruct;
			//Se recibe la respuesta de la UMV
			socket_recibir(sockUMV, &tipoStruct, &structRecibido);
			//Se valida la respuesta
			if (tipoStruct != D_STRUCT_RESPUESTA_UMV) {
				printf("%d\n", tipoStruct);
			}

			int tmanio_respuesta, offset;

			tmanio_respuesta = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer;		//esto esta al pedo

			memcpy(&cursor_contexto_ant, ((t_struct_respuesta_umv*) structRecibido)->buffer, offset = sizeof(uint32_t));
			memcpy(&prog_counter_ant, ((t_struct_respuesta_umv*) structRecibido)->buffer + offset, sizeof(uint32_t));
			free(((t_struct_respuesta_umv*) structRecibido)->buffer);
			free(structRecibido);
			dictionary_clean_and_destroy_elements(dicc_variables, free);  // debemos limpiar el diccionario y rellenarlo con las variables del contexto anterior

			cant_vars_ant = ((punts_contx_ant->offset) - cursor_contexto_ant) / 5;

			int i = 0;
			int incremento = 0;
			int posicion = 1;

			while (i < cant_vars_ant)  // con esto volvemos a llenar el dicc_variables con las variables del contexto anterior
			{
				int valor_var;
				char nombre_var;

				t_struct_sol_bytes * sol_var_ant = malloc(sizeof(t_struct_sol_bytes));
				sol_var_ant->base = var_seg_stack;
				sol_var_ant->offset = cursor_contexto_ant + incremento;
				sol_var_ant->tamanio = sizeof(char) + sizeof(int);
				socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, sol_var_ant);

				void * structRecibido2;
				t_tipoEstructura tipoStruct2;

				socket_recibir(sockUMV, &tipoStruct2, &structRecibido2);
				if (tipoStruct2 != D_STRUCT_RESPUESTA_UMV) {
					printf("%d\n", tipoStruct2);
				}
				void * bufferAux = ((t_struct_respuesta_umv*) structRecibido2)->buffer;
				memcpy(&nombre_var, bufferAux, 1);
				memcpy(&valor_var, bufferAux + 1, 4);  // no es necesario este valor

				char key = nombre_var;
				char keyABuscar[2] = { key, '\0' };

				int* posicion_variable = malloc(sizeof(int));
				*posicion_variable = posicion;

				dictionary_put(dicc_variables, keyABuscar, posicion_variable);

				incremento = incremento + (sol_var_ant->tamanio);
				i++;
				posicion++;
				free(sol_var_ant);
				free(structRecibido2);
				free(bufferAux);
				//free(posicion_variable);
			}

			temp_counter = prog_counter_ant; //aca ya cargamos PCounter del cntxt que guradamos en la UMV.
			temp_cursor_stack = cursor_contexto_ant; //aca cargo elcursor del cntxt anterior q se guardo en la UMV.
			var_tamanio_contexto = cant_vars_ant;
			free(punts_contx_ant);

		}

}


void retornar(t_valor_variable retorno) {

	if (SEG_flag == 1)
			return;
		printf("retornar\n");
		uint32_t prog_counter_ant; //vamos a pedir los 12 bytes que guardamos en el stack para volver al cntxt anterior.
		uint32_t cursor_contexto_ant;
		uint32_t direccion_retornar;
		uint32_t cant_vars_ant;

		t_struct_sol_bytes * punts_contx_ant = malloc(sizeof(t_struct_sol_bytes));
		punts_contx_ant->base = var_seg_stack;
		punts_contx_ant->offset = (temp_cursor_stack - (3 * sizeof(uint32_t))); //-var_seg_stack;estoy casi seguro que no se le resta la base, porq el cursor de contexto ya es el offset de la base  //restamos el cursor actual - 12(xq es el tamaño de los punteros q guarde en la umv) - el inicio del stack.
		punts_contx_ant->tamanio = 3 * sizeof(uint32_t);

		socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, punts_contx_ant);
		void * structRecibido;
		t_tipoEstructura tipoStruct;

		socket_recibir(sockUMV, &tipoStruct, &structRecibido);
		if (tipoStruct != D_STRUCT_RESPUESTA_UMV) {
			printf("%d\n", tipoStruct);
		}

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

		int tmanio_respuesta, offset;
		tmanio_respuesta = ((t_struct_respuesta_umv*) structRecibido)->tamano_buffer; //esto esta al pedo

		memcpy(&cursor_contexto_ant, ((t_struct_respuesta_umv*) structRecibido)->buffer, offset = sizeof(uint32_t));
		memcpy(&prog_counter_ant, ((t_struct_respuesta_umv*) structRecibido)->buffer + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(&direccion_retornar, ((t_struct_respuesta_umv*) structRecibido)->buffer + offset, sizeof(uint32_t));
		free(((t_struct_respuesta_umv*) structRecibido)->buffer);
		free(structRecibido);
		dictionary_clean_and_destroy_elements(dicc_variables, free);  // debemos limpiar el diccionario y rellenarlo con las variables del contexto anterior

		cant_vars_ant = ((punts_contx_ant->offset) - cursor_contexto_ant) / 5;

		int i = 0;
		int incremento = 0;
		int posicion = 1;

		while (i < cant_vars_ant) {		// con esto volvemos a llenar el dicc_variables con las variables del contexto anterior

			int valor_var;
			char nombre_var;

			t_struct_sol_bytes * sol_var_ant = malloc(sizeof(t_struct_sol_bytes));
			sol_var_ant->base = var_seg_stack;
			sol_var_ant->offset = cursor_contexto_ant + incremento;
			sol_var_ant->tamanio = sizeof(char) + sizeof(int);
			socket_enviar(sockUMV, D_STRUCT_SOL_BYTES, sol_var_ant);

			void * structRecibido2;
			t_tipoEstructura tipoStruct2;

			socket_recibir(sockUMV, &tipoStruct2, &structRecibido2);
			if (tipoStruct2 != D_STRUCT_RESPUESTA_UMV) {
				printf("%d\n", tipoStruct2);
			}
			memcpy(&nombre_var, ((t_struct_respuesta_umv*) structRecibido2)->buffer, offset = sizeof(char));
			memcpy(&valor_var, ((t_struct_respuesta_umv*) structRecibido2)->buffer + offset, sizeof(int));		// no es necesario este valor

			char key = nombre_var;
			char keyABuscar[2] = { key, '\0' };

			int* posicion_variable = malloc(sizeof(int));
			*posicion_variable = posicion;

			dictionary_put(dicc_variables, keyABuscar, posicion_variable);

			incremento = incremento + (sol_var_ant->tamanio);
			i++;
			posicion++;
			free(sol_var_ant);
			free(((t_struct_respuesta_umv*) structRecibido2)->buffer);
			free(structRecibido2);
			//free(posicion_variable);
		}

		temp_counter = prog_counter_ant; //aca  cargamos PCounter del cntxt que guradamos en la UMV.
		temp_cursor_stack = cursor_contexto_ant; //aca cargo elcursor del cntxt anterior q se guardo en la UMV.
		//direccion_retornar=retorno; // esto no creo q sea asi, me parece que deberia hacerse una asignacion del valor de "reotorno" en la "direccion_retornar". lO hice mas arriba
		var_tamanio_contexto = cant_vars_ant; // recuperamos nuestro tamanio de contexto anterior cortesia de la variable cant_vars_ant creada para el recupero del diccionario de variables ;)

		if (retorno != -1) {
			asignar(direccion_retornar, retorno);		// el if es para reutilizar el reotornar en el finalizar omitiendo la asignacion
		}

		free(punts_contx_ant);
		printf("retornar\n");
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

