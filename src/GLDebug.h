#ifndef OPENGL_STUDY_GLDEBUG_H
#define OPENGL_STUDY_GLDEBUG_H

#include "glad/glad.h"

const char* GetGLSourceString(GLenum source);
const char* GetGLTypeString(GLenum type);
const char* GetGLSeverityString(GLenum severity);

void APIENTRY DebugOpenGLErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
);

#endif
