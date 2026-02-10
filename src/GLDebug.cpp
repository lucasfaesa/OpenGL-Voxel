#include "GLDebug.h"
#include "Logger.h"

const char* GetGLSourceString(GLenum source) {
    switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APP";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "UNKNOWN";
    }
}

const char* GetGLTypeString(GLenum type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "UNKNOWN";
    }
}

const char* GetGLSeverityString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "INFO";
        default: return "UNKNOWN";
    }
}

void APIENTRY DebugOpenGLErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei,
    const GLchar* message,
    const void*
) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;

    LOG_ERROR(
        "[GL %s] [%s] [%s] (id=%u)\n    %s",
        GetGLSeverityString(severity),
        GetGLTypeString(type),
        GetGLSourceString(source),
        id,
        message
    );
}
