#ifndef DYNARRAY_H_INCLUDED
#define DYNARRAY_H_INCLUDED

#include <stddef.h>

struct Pl_Dynarray {
	void ** data;
	size_t reserved;
	size_t used;
};

struct Pl_Dynarray * Pl_DynarrayCreate(size_t initial);
void * Pl_DynarrayAddElement(struct Pl_Dynarray * dyn, void * element, size_t size);
size_t Pl_DynArraySize(struct Pl_Dynarray * dyn);
void * Pl_DynarrayAt(struct Pl_Dynarray * dyn, size_t it);
void Pl_DynarrayDestroy(struct Pl_Dynarray * dyn);

#endif // DYNARRAY_H_INCLUDED
