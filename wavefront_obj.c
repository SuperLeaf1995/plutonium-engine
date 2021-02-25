#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "object.h"
#include "dynarray.h"

struct Pl_Object * Pl_WavefrontObjRead(const char * file) {
	struct Pl_Dynarray * vertexList;
	struct Pl_Object * obj;
	char * buffer;
	FILE * fp;

	fp = fopen(file,"rb");
	if(!fp) {
		return NULL;
	}

	buffer = malloc(256);
	if(buffer == NULL) {
		fclose(fp);
		return NULL;
	}

	obj = malloc(sizeof(struct Pl_Object));
	if(obj == NULL) {
		free(buffer);
		fclose(fp);
		return NULL;
	}

	obj->vertices = Pl_DynarrayCreate(0);
	if(obj->vertices == NULL) {
		free(obj);
		free(buffer);
		fclose(fp);
		return NULL;
	}

	vertexList = Pl_DynarrayCreate(0);
	if(vertexList == NULL) {
		Pl_DynarrayDestroy(obj->vertices);
		free(obj);
		free(buffer);
		fclose(fp);
		return NULL;
	}

	/* Parse by line */
	while(fgets(buffer,254,fp)) {
		struct Pl_ObjectVertex vertex;
		unsigned int vIndex[64], vtIndex[64], vnIndex[64];
		const char * params;

		switch(buffer[0]) {
		/* comment */
		case '#':
			break;
		/* vertex */
		case 'v':
			params = buffer+1;
			sscanf(params,"%f %f %f",&vertex.x,&vertex.y,&vertex.z);
			Pl_DynarrayAddElement(vertexList,&vertex,sizeof(struct Pl_ObjectVertex));
			break;
		/* face */
		case 'f':
			params = buffer+1;
			sscanf(params,"%u/%u/%u %u/%u/%u %u/%u/%u",&vIndex[0],&vtIndex[0],&vnIndex[0],&vIndex[1],&vtIndex[1],&vnIndex[1],&vIndex[2],&vtIndex[2],&vnIndex[2]);
			for(int j = 0; j < 3; j++) {
				Pl_DynarrayAddElement(obj->vertices,Pl_DynarrayAt(vertexList,vIndex[j]-1),sizeof(struct Pl_ObjectVertex));
			}
			break;
		/* other */
		default:
			break;
		}
	}

	Pl_DynarrayDestroy(vertexList);
	free(buffer);
	fclose(fp);
	return obj;
}
