/*
 * funcionesInterprete.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */


#include "funcionesInterprete.h"
#include "log.h"

extern log_t* archLog;

char* leerScript(off_t* fileSize,char* path) {

	struct stat file_info;
	int control = lstat(path, &file_info);
	char* buffer = NULL;

	if (control == -1) {
		log_escribir(archLog, "Leer Script", ERROR, "El archivo no existe");
		*fileSize = 0;
	}

	else {

		*fileSize = file_info.st_size;
		FILE* script = fopen(path, "r");
		buffer = malloc(*fileSize);
		fread(buffer, *fileSize, sizeof(char), script);
		buffer[*fileSize - 1] = '\0';
		fclose(script);

	}

	return buffer;
}
