#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>

typedef struct {
	Display * dpy;
	XVisualInfo * vi;
	Window win;
	Colormap cmap;
	GLXContext glCtx;
	GC gc;
	XSetWindowAttributes swa;
	void (*glRedrawFunc)(void * data);
	int canMultithread;
	Atom wmDeleteWindow;
}Pl_Context;

Pl_Context * Pl_ContextCreate(const char * query);
int Pl_NextFrame(Pl_Context * ctx);
void Pl_ContextDestroy(Pl_Context * ctx);

#endif // CONTEXT_H_INCLUDED
