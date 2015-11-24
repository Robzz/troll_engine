#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>

void check_gl_errors();
const char* parse_error(GLenum err);

#ifdef DEBUG
void wrap_gl_call();
#define GL(gl_call) ( \
    if((GLenum e = glGetError())) { \
        std::cerr << "GL Error!\n\tCall : " #gl_call "\n\tCause : " << parse_error(e) << std::endl;\
        } \
)
#else
#define GL(gl_call) (gl_call)

#endif

#endif
