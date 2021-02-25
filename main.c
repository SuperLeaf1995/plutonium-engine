#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>
#include "math.h"
#include "context.h"
#include "object.h"
#include "dynarray.h"
#include "logstream.h"

/* Function to poll events */
void * Pl_EventLoop(void * data) {
	float angle = 0.0f;
	/*
	float rad, precision;
	int sphere_size = 0;
	*/

	Pl_Context * ctx = (Pl_Context *)data;

	glXMakeCurrent(ctx->dpy,ctx->win,ctx->glCtx);

	/*
	GLuint lst = glGenLists(1);
	glNewList(1,GL_COMPILE);
	rad = 1.0f;
	precision = 360.0f/6;
	glBegin(GL_TRIANGLE_STRIP);
	for(float phi = 0.0f; phi < 360.0f; phi += precision) {
		for(float theta = 0.0f; theta < 180.0f; theta += precision) {
			glColor4f((double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,0.5f);
			glVertex3f(
				rad*Pl_MathSin(theta*(M_PI/180))*Pl_MathSin(phi*(M_PI/180)),
				rad*Pl_MathCos(theta*(M_PI/180)),
				rad*Pl_MathSin(theta*(M_PI/180))*Pl_MathCos(phi*(M_PI/180))
			);
			glColor4f((double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,0.5f);
			glVertex3f(
				rad*Pl_MathSin((theta+(precision))*(M_PI/180))*Pl_MathSin((phi+(precision))*(M_PI/180)),
				rad*Pl_MathCos((theta+(precision))*(M_PI/180)),
				rad*Pl_MathSin((theta+(precision))*(M_PI/180))*Pl_MathCos((phi+(precision))*(M_PI/180))
			);
		}
	}
	glEnd();
	glEndList();
	*/

	struct Pl_Object * obj = Pl_WavefrontObjRead("/home/superleaf1995/src/Plutonium-Engine/bin/Debug/cash_register.obj");
	Pl_ObjectToVBO(obj);

	/*
	GLuint lst = glGenLists(1);
	glNewList(1,GL_COMPILE);
	glPushMatrix();
	for(size_t i = 0; i < Pl_DynArraySize(obj->vertices); i += 3) {
		struct Pl_ObjectVertex * vertex;
		glColor3b(127,rand()%127,rand()%127);

		glBegin(GL_TRIANGLE_FAN);

		vertex = Pl_DynarrayAt(obj->vertices,i+0);
		glVertex3f(vertex->x,vertex->y,vertex->z);

		vertex = Pl_DynarrayAt(obj->vertices,i+1);
		glVertex3f(vertex->x,vertex->y,vertex->z);

		vertex = Pl_DynarrayAt(obj->vertices,i+2);
		glVertex3f(vertex->x,vertex->y,vertex->z);

		glEnd();
	}
	glPopMatrix();
	glEndList();
	*/

	const int dimension = 3;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,-(4/1.5f),-(16500*3.5f));
	while(1) {
		angle += 1.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#pragma omp for
		for(int j = 0; j < dimension; j++) {
			for(int i = 0; i < dimension; i++) {
				for(int k = 0; k < dimension; k++) {
					glPushMatrix();
					glTranslated(j*12192.0f,i*12192.0f,k*12192.0f);
					glRotatef(angle*1.0f,0.1f,0.0f,0.0f);
					glRotatef(angle*1.5f,0.3f,0.1f,0.0f);
					glRotatef(angle*2.5f,0.0f,0.0f,1.0f);
					Pl_ObjectToOpenGL(obj);
					glPopMatrix();
				}
			}
		}
		glRotatef(0.25f,0.25f,0.25f,0.25f);
		Pl_NextFrame(ctx);
	}
	return NULL;
}

int main(int argc, char ** argv) {
	Pl_Context * ctx;

	ctx = Pl_ContextCreate(NULL);

	pthread_t threadEventLoop;
	pthread_create(&threadEventLoop,NULL,&Pl_EventLoop,ctx);
	pthread_join(threadEventLoop,NULL);

	Pl_ContextDestroy(ctx);
	return 0;
}
