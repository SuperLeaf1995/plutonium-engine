#include <stdlib.h>
#include <X11/Xatom.h>
#include "context.h"
#include "logstream.h"
#include "math.h"

static void Pl_GLRedrawSingleBuffer(void * data) {
	glFlush();
	return;
}

static void Pl_GLRedrawDoubleBuffer(void * data) {
	Pl_Context * ctx = (Pl_Context *)data;
	glXSwapBuffers(ctx->dpy,ctx->win);
	return;
}

Pl_Context * Pl_ContextCreate(const char * query) {
	Pl_Context * ctx;
	int temp;
	int doubleBuffer[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		16,
		GLX_DOUBLEBUFFER,
		None
	};
	int singleBuffer[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		16,
		GLX_DOUBLEBUFFER,
		None
	};

	Pl_LogStreamInit();
	ctx = malloc(sizeof(Pl_Context));
	if(ctx == NULL) {
		return NULL;
	}

	/* Initialize threading */
	if(!XInitThreads()) {
		ctx->canMultithread = 0;
	}

	/* Connect to the X11 server */
	ctx->dpy = XOpenDisplay(NULL);
	if(ctx->dpy == NULL) {
		return NULL;
	}

	/* Check if X11 supports GLX extension */
	if(!glXQueryExtension(ctx->dpy,&temp,&temp)) {
		return NULL;
	}

	/* Choose a nice visual, set drawing function and use single buffer as fallback */
	ctx->vi = glXChooseVisual(ctx->dpy,DefaultScreen(ctx->dpy),doubleBuffer);
	ctx->glRedrawFunc = &Pl_GLRedrawDoubleBuffer;
	if(ctx->vi == NULL) {
		ctx->vi = glXChooseVisual(ctx->dpy,DefaultScreen(ctx->dpy),singleBuffer);
		if(ctx->vi == NULL) {
			return NULL;
		}
		ctx->glRedrawFunc = &Pl_GLRedrawSingleBuffer;
	}

	ctx->cmap = XCreateColormap(ctx->dpy,RootWindow(ctx->dpy,ctx->vi->screen),ctx->vi->visual,AllocNone);
	ctx->swa.colormap = ctx->cmap;
	ctx->swa.border_pixel = 0;
	ctx->swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyReleaseMask | StructureNotifyMask;
	ctx->win = XCreateWindow(ctx->dpy,RootWindow(ctx->dpy,ctx->vi->screen),0,0,1200,800,0,ctx->vi->depth,InputOutput,ctx->vi->visual,( CWBorderPixel | CWColormap | CWEventMask ),&ctx->swa);
	XSetStandardProperties(ctx->dpy,ctx->win,"Plutonium Engine Demo","Plutonium Engine Demo",None,NULL,0,NULL);
	ctx->wmDeleteWindow = XInternAtom(ctx->dpy,"WM_DELETE_WINDOW",False);
	XSetWMProtocols(ctx->dpy,ctx->win,&ctx->wmDeleteWindow,1);

	/* Map window onto screen */
	XMapWindow(ctx->dpy,ctx->win);

	/* Events? ALL OF IT */
	XSelectInput(ctx->dpy,ctx->win,( KeyPressMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyReleaseMask | StructureNotifyMask ));

	/* Create new GL context */
	ctx->glCtx = glXCreateContext(ctx->dpy,ctx->vi,None,GL_TRUE);
	if(ctx->glCtx == NULL) {
		return NULL;
	}

	/* Bind GL context to window */
	glXMakeCurrent(ctx->dpy,ctx->win,ctx->glCtx);

	/* Set up depth buffering */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	/* Set redraw clear color to complete black */
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	/* Set projection transformation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0f,1.0f,-1.0f,1.0f,1.0f,99999.0f);
	glViewport(0,0,1200,800);

	ctx->gc = XCreateGC(ctx->dpy,ctx->win,0,NULL);
	Pl_MathInit(0);
	return ctx;
}

/** Do a single "step" (draw frames, handle events, etc) */
int Pl_NextFrame(Pl_Context * ctx) {
	XLockDisplay(ctx->dpy);

	/* Get next event in queue*/
	while(XPending(ctx->dpy)) {
		XEvent event;
		XNextEvent(ctx->dpy,&event);
		switch(event.type) {
		case ConfigureNotify:
			glViewport(0,0,event.xconfigure.width,event.xconfigure.height);
			break;
		/* Request to resize window */
		case ResizeRequest:
			glViewport(0,0,event.xresizerequest.width,event.xresizerequest.height);
			/* Remap window */
			XUnmapWindow(ctx->dpy,ctx->win);
			XMapWindow(ctx->dpy,ctx->win);
			break;
		/* Expose event, when part of the window region is damaged and needs to redraw */
		case Expose:
			(*ctx->glRedrawFunc)(ctx);
			break;
		case ClientMessage:
			/* Request to close application */
			if((Atom)event.xclient.data.l[0] == ctx->wmDeleteWindow) {
				Pl_ContextDestroy(ctx);
				return 1;
			}
			break;
		default:
			break;
		}
	}

	/* Redraw GL */
	(*ctx->glRedrawFunc)(ctx);

	/* Flush requests to server */
	XFlushGC(ctx->dpy,ctx->gc);
	XFlush(ctx->dpy);

	XUnlockDisplay(ctx->dpy);
	return 0;
}

void Pl_ContextDestroy(Pl_Context * ctx) {
	Pl_MathEnd();
	glXDestroyContext(ctx->dpy,ctx->glCtx);
	XUnmapWindow(ctx->dpy,ctx->win);
	XFreeGC(ctx->dpy,ctx->gc);
	XFreeColormap(ctx->dpy,ctx->cmap);
	XFree(ctx->vi);
	XDestroyWindow(ctx->dpy,ctx->win);
	XCloseDisplay(ctx->dpy);
	free(ctx);
	Pl_LogStreamEnd();
	return;
}
