#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "logstream.h"

static pthread_mutex_t * globalStdioMutex;

void Pl_LogStreamInit(void) {
	globalStdioMutex = malloc(sizeof(pthread_mutex_t));
	if(globalStdioMutex == NULL) {
		return;
	}
	pthread_mutex_init(globalStdioMutex,NULL);
	return;
}

void Pl_LogStream(const char * context, const char * fmt, ...) {
	va_list arg;
	va_start(arg,fmt);

	pthread_mutex_lock(globalStdioMutex);
	printf("\x1B[1m[\x1B[38;5;70m%8s\x1B[0m\x1B[1m]\x1B[0m ",context);
	vprintf(fmt,arg);
	pthread_mutex_unlock(globalStdioMutex);
	return;
}

void Pl_LogStreamEnd(void) {
	pthread_mutex_destroy(globalStdioMutex);
	free(globalStdioMutex);
	return;
}
