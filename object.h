#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#define PL_GL_VBO_NORMAL
#define PL_GL_VBO_VERTEX

struct Pl_ObjectVertex {
	float x;
	float y;
	float z;
};

struct Pl_ObjectVBO {
	int type;
	struct Pl_ObjectVertex * data;
	size_t size;
};

struct Pl_ObjectVAO {
	void ** attributes;
	size_t size;
};

struct Pl_Object {
	struct Pl_Dynarray * vertices;
	struct Pl_ObjectVBO vbo;
	struct Pl_ObjectVAO vao;
};

struct Pl_Object * Pl_WavefrontObjRead(const char * file);
int Pl_ObjectToVBO(struct Pl_Object * obj);
int Pl_ObjectToOpenGL(struct Pl_Object * obj);

#endif // OBJECT_H_INCLUDED
