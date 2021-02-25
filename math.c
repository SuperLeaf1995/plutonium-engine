#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "logstream.h"

/* Old school tricks, used to speed up calculations in the ol' days of the x87 FPU */
long double * cosTable;
long double * sinTable;
long double * tanTable;

int tableSize;

void Pl_MathInit(int precision) {
	/* Populate cosine and sine tables */
	long double step;
	int i;

	if(!precision) {
		/* Use default page size */
		tableSize = sysconf(_SC_PAGESIZE);
	} else {
		/* Do by precision */
		tableSize = pow(180,precision);
	}

	/* Allocate COS/SIN/TAN tables */
	cosTable = malloc(tableSize*sizeof(long double));
	if(cosTable == NULL) {
		return;
	}
	sinTable = malloc(tableSize*sizeof(long double));
	if(sinTable == NULL) {
		return;
	}
	tanTable = malloc(tableSize*sizeof(long double));
	if(tanTable == NULL) {
		return;
	}

	/* Fill table as mostly as possible */
	step = 0.0f;
	for(i = 0; i < tableSize; i++) {
		cosTable[i] = (long double)cos(step);
		sinTable[i] = (long double)sin(step);
		tanTable[i] = (long double)tan(step);
		step += (180.0f/tableSize);
	}
	return;
}

void Pl_MathEnd(void) {
	free(cosTable);
	free(sinTable);
	free(tanTable);
	return;
}
