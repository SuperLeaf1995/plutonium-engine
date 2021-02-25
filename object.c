#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include <GL/gl.h>
#include "object.h"
#include "dynarray.h"

/** Create a object's vertices into VBO information, deletes old vertices Dynamic Array */
int Pl_ObjectToVBO(struct Pl_Object * obj) {
	size_t i, j;

	j = Pl_DynArraySize(obj->vertices);

	obj->vbo.size = j;
	obj->vbo.data = malloc(sizeof(float)*j*3);
	if(obj->vbo.data == NULL) {
		return 1;
	}

	#pragma omp for
	for(i = 0; i < j; i++) {
		struct Pl_ObjectVertex * vertex;
		vertex = Pl_DynarrayAt(obj->vertices,i);
		obj->vbo.data[i].x = vertex->x;
		obj->vbo.data[i].y = vertex->y;
		obj->vbo.data[i].z = vertex->z;
	}
	Pl_DynarrayDestroy(obj->vertices);
	return 0;
}

/** Convert object to OpenGL calls */
int Pl_ObjectToOpenGL(struct Pl_Object * obj) {
	#pragma omp for
	for(size_t i = 0; i < obj->vbo.size; i++) {
		struct Pl_ObjectVertex vertex;
		glColor3b(rand()%127,rand()%127,rand()%127);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(obj->vbo.data[i].x,obj->vbo.data[i].y,obj->vbo.data[i].z);
		i++;
		glVertex3f(obj->vbo.data[i].x,obj->vbo.data[i].y,obj->vbo.data[i].z);
		i++;
		glVertex3f(obj->vbo.data[i].x,obj->vbo.data[i].y,obj->vbo.data[i].z);
		glEnd();
	}
	return 0;
}
