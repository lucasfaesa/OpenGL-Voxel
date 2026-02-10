#ifndef OPENGL_STUDY_LOGGER_H
#define OPENGL_STUDY_LOGGER_H

#include <cstdio>

#ifndef __FILE_NAME__
    #define __FILE_NAME__ __FILE__
#endif

#ifdef _WIN32
    #define LOG_COLOR_INFO  ""
    #define LOG_COLOR_WARN  ""
    #define LOG_COLOR_ERROR ""
    #define LOG_COLOR_RESET ""
#else
    #define LOG_COLOR_INFO  "\033[32m"
    #define LOG_COLOR_WARN  "\033[33m"
    #define LOG_COLOR_ERROR "\033[31m"
    #define LOG_COLOR_RESET "\033[0m"
#endif

#ifdef NDEBUG
    #define LOG_INFO(...)  do {} while (0)
    #define LOG_WARN(...)  do {} while (0)
    #define LOG_ERROR(...) do {} while (0)
#else
    #define LOG_INFO(...) do { \
    printf(LOG_COLOR_INFO "[INFO] [%s:%d:%s] ", __FILE_NAME__, __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf(LOG_COLOR_RESET "\n"); \
    } while (0)

    #define LOG_WARN(...) do { \
    printf(LOG_COLOR_WARN "[WARN] [%s:%d:%s] ", __FILE_NAME__, __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf(LOG_COLOR_RESET "\n"); \
    } while (0)

    #define LOG_ERROR(...) do { \
    printf(LOG_COLOR_ERROR "[ERROR] [%s:%d:%s] ", __FILE_NAME__, __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf(LOG_COLOR_RESET "\n"); \
    } while (0)
#endif

#endif
