#ifndef LOGSTREAM_H_INCLUDED
#define LOGSTREAM_H_INCLUDED

#include <stdarg.h>

void Pl_LogStreamInit(void);
void Pl_LogStream(const char * context, const char * fmt, ...);
void Pl_LogStreamEnd(void);

#endif // LOGSTREAM_H_INCLUDED
