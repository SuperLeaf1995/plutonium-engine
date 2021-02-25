#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "context.h"
#include "logstream.h"

static pthread_t * gProcList;

void * Pl_RenderThread(void * data) {

	return NULL;
}

/* Rendering with threads makes stuff faster */
int Pl_RenderInit(Pl_Context * ctx, int n_thread) {
	if(!n_thread) {
		n_thread = sysconf(_SC_NPROCESSORS_ONLN);
		if(!n_thread) {
			Pl_LogStream("PL","Unable to create rendering threads\n");
			return 1;
		}
		Pl_LogStream("PL","Creating %i rendering threads\n",n_thread);
	}
	gProcList = malloc(sizeof(pthread_t)*n_thread);
	if(gProcList == NULL) {
		Pl_LogStream("PL","Unable to allocate memory for rendering proclist\n");
		return 2;
	}

	for(int i = 0; i < n_thread; i++) {
		pthread_create(gProcList[i],NULL,NULL,NULL);
	}
	return 0;
}
