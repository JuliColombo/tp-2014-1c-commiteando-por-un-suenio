/*
 * log.c
 *
 *  Created on: 08/11/2012
 *      Author: utnso
 */

#include "log.h"

log_t* log_crear(char* dirLog) {

	char pathLog[80];

	log_t* log = malloc(sizeof(log_t));
	log->mode = M_CONSOLEANDFILE;
	log->pid = getpid();

	sprintf(pathLog,"%s PI.[%d].log",dirLog,log->pid);

	if ((log->file = fopen(pathLog, "a")) == NULL) {
		free(log);
		return NULL;
	}


	return log;
}

void log_destruir(log_t *log) {
	if (log != NULL) {
		fclose(log->file);
		free(log);
	}
}

int log_escribir(log_t *log, const char *program_name, e_message_type type,
		const char* format, ...) {

	va_list args_list;
	va_start(args_list, format);
	return log_vwrite(log, program_name, type, format, args_list);

}

int log_vwrite(log_t *log, const char *program_name, e_message_type type,
		const char* format, va_list args_list) {

	time_t log_time;
	struct tm *log_tm;
	struct timeb tmili;
	char str_time[128];
	//unsigned int thread_id = pthread_self(); Esto esta hardcodeado porque la funcion pthread_self() da un error.
	unsigned int thread_id = 2020;
	char *str_type = log_errorTypeAsString(type);
	char logbuff[MAX_MESSAGE_LENGHT + 100];
	char msgbuff[MAX_MESSAGE_LENGHT];

	if (log->mode == M_NONE) //Debugger: Segmentation Fault
		return 1;

	vsprintf(msgbuff, format, args_list); //Debugger dice que no encuentra un archivo, no se puede ejecutar esta línea

	if ((log_time = time(NULL)) == -1) { //Debugger: Can't find a source file at "../sysdeps/unix/sysv/linux/time.c"
		puts("[[CRITICAL]] :: Error, no se pudo obtener la hora\n");
		return 0;
	}

	log_tm = localtime(&log_time); //Debugger: Can't find a source file at "localtime.c"
	strftime(str_time, 127, "%y-%m-%d %H:%M:%S", log_tm); //Debugger: Can't find a source file at "strftime.c"

	if (ftime(&tmili)) { //Debugger: Can't find a source file at "../sysdeps/unix/bsd/ftime.c"
		puts("[[CRITICAL]] :: No se han podido obtener los milisendos\n");
		return 0;
	}

	sprintf(logbuff, "[%s] - [%s - pid: %d] [Thread: %d] [%s]: %s\r\n", //Debugger: Segmentation Fault
			str_time, program_name, log->pid, thread_id, str_type, msgbuff);

	if ((log->mode == M_FILE) || (log->mode == M_CONSOLEANDFILE)) {
		fprintf(log->file, "%s", logbuff);
		fflush(log->file);
	}
	if ((log->mode == M_CONSOLE) || (log->mode == M_CONSOLEANDFILE)) {
		printf("%s", logbuff);
	}

	return 1;

}

char *log_errorTypeAsString(e_message_type type) {
	if (type == ERROR)
		return "ERROR";
	if (type == WARNING)
		return "WARNING";
	if (type == INFO)
		return "INFO";
	if (type == DEBUG)
		return "DEBUG";
	if (type == LSCH)
		return "LSCH";
	return "UNKNOW_MSG_TYPE";
}

/*
 * @NAME: log_setPrintMode
 * @DESC: Activa o descativa la impresion por consola
 */
void log_setPrintMode(log_t* log, e_print_mode mode) {
	log->mode = mode;
}

/*
 * @NAME: log_getPrintMode
 * @DESC: Devuelve el modo de impresion de la consola
 */
e_print_mode log_getPrintMode(log_t* log) {
	return log->mode;
}
