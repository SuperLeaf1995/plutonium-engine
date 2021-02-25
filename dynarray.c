#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "dynarray.h"

struct Pl_Dynarray * Pl_DynarrayCreate(size_t initial) {
	struct Pl_Dynarray * dyn;

	dyn = malloc(sizeof(struct Pl_Dynarray));
	if(dyn == NULL) {
		return NULL;
	}

	if(!initial) {
		initial = sysconf(_SC_PAGESIZE);
	}

	dyn->data = malloc(sizeof(void *)*initial);
	if(dyn->data == NULL) {
		free(dyn);
		return NULL;
	}

	dyn->reserved = initial;
	dyn->used = 0;
	return dyn;
}

void * Pl_DynarrayAddElement(struct Pl_Dynarray * dyn, void * element, size_t size) {
	void * new;

	if(element == NULL || !size) {
		return NULL;
	}

	/* Expand dynamic array if needed */
	if(dyn->used >= dyn->reserved) {
		/* Increment till we are bigger */
		while(dyn->used >= dyn->reserved) {
			dyn->reserved += sysconf(_SC_PAGESIZE);
		}

		dyn->data = realloc(dyn->data,sizeof(void *)*dyn->reserved);
		if(dyn->data == NULL) {
			return NULL;
		}
	}

	/* Create copy of element */
	new = malloc(size);
	if(new == NULL) {
		return NULL;
	}
	memcpy(new,element,size);
	dyn->data[dyn->used] = new;
	dyn->used++;
	return new;
}

size_t Pl_DynArraySize(struct Pl_Dynarray * dyn) {
	return dyn->used;
}

void * Pl_DynarrayAt(struct Pl_Dynarray * dyn, size_t it) {
	if(it > dyn->used) {
		return NULL;
	}
	return dyn->data[it];
}

void Pl_DynarrayDestroy(struct Pl_Dynarray * dyn) {
	for(int i = 0; i < Pl_DynArraySize(dyn); i++) {
		free(dyn->data[i]);
	}
	free(dyn);
	return;
}
