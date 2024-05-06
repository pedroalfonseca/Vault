#ifndef LOG_H
#define LOG_H

#include "types.h"

#include <stdio.h>
#include <stdarg.h>

// --------------------------------------------------------------------------------

typedef enum _text_color {
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_COUNT
} text_color;

// --------------------------------------------------------------------------------

void _log(FILE *stream, const char *prefix, const char *fmt, text_color color, ...) {
    local const char *text_color_table[TEXT_COLOR_COUNT] = {
        "\x1b[31m", // TEXT_COLOR_RED
        "\x1b[32m", // TEXT_COLOR_GREEN
        "\x1b[33m", // TEXT_COLOR_YELLOW
    };

    char fmt_buff[128];
    sprintf(fmt_buff, "%s%s:\033[0m %s\n", text_color_table[color], prefix, fmt);

    local char buff[128];
    va_list args;
    va_start(args, color);
    vsprintf(buff, fmt_buff, args);
    va_end(args);

    fputs(buff, stream);
}

// --------------------------------------------------------------------------------

extern FILE *_log_file;

// --------------------------------------------------------------------------------

#ifdef DEBUG_MODE
    #define log_trace(fmt, ...)   _log(stderr, "TRACE", fmt, TEXT_COLOR_GREEN, ##__VA_ARGS__)
    #define log_warning(fmt, ...) _log(stderr, "WARNING", fmt, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
    #define log_error(fmt, ...)   _log(stderr, "ERROR", fmt, TEXT_COLOR_RED, ##__VA_ARGS__)
#else
    #define log_trace(fmt, ...)   _log(_log_file, "TRACE", fmt, TEXT_COLOR_GREEN, ##__VA_ARGS__)
    #define log_warning(fmt, ...) _log(_log_file, "WARNING", fmt, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
    #define log_error(fmt, ...)   _log(_log_file, "ERROR", fmt, TEXT_COLOR_RED, ##__VA_ARGS__)
#endif // DEBUG_MODE

// --------------------------------------------------------------------------------

#endif // LOG_H