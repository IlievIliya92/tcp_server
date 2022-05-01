/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "log.h"

/*********************************** DEFINES **********************************/
#define LOG_BUFFER_SIZE         1024

#define RED()    (fprintf(stdout, "\033[1;31m"))
#define GREEN() (fprintf(stdout, "\033[0;32m"))
#define YELLOW() (fprintf(stdout, "\033[1;33m"))
#define BLUE() (fprintf(stdout, "\033[0;34m"))
#define PURPLE() (fprintf(stdout, "\033[0;35m"))
#define CYAN() (fprintf(stdout, "\033[0;36m"))
#define WHITE() (fprintf(stdout, "\033[0;37m"))
#define RESET()  (fprintf(stdout, "\033[0m"))

#define COL(X)   (fprintf(stdout, color_value[X]))

/*********************************** TYPEDEFS *********************************/
typedef enum _col_t
{
  COL_RED = 0,
  COL_GREEN,
  COL_YELLOW,
  COL_BLUE,
  COL_PURPLE,
  COL_CYAN,
  COL_WHITE,
  COL_RESET,
  COLORS
} col_t;

/******************************* DATA *****************************************/
static const char *log_lvl_str[LVLS] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "CRITICAL"
};

static const char *color_value[COLORS] = {
  "\033[1;31m",
  "\033[0;32m",
  "\033[1;33m",
  "\033[0;34m",
  "\033[0;35m",
  "\033[0;36m",
  "\033[0;37m",
  "\033[0m"
};

/***************************** LOCAL FUNCTIONS ********************************/
static
void log_col_line_reset(log_lvl_t level)
{
    switch(level)
    {
        /* Fall through */
        case INFO:
        case WARNING:
        case ERR:
        case CRITICAL:
            RESET();
            break;

        default:
            break;
    }

    return;
}

static
void log_col_line(log_lvl_t level)
{
    switch(level)
    {
        /* Fall through */
        case TRACE:
        case DEBUG:
            WHITE();
            break;

        case INFO:
            WHITE();
            break;

        case WARNING:
            YELLOW();
            break;

        case ERR:
        case CRITICAL:
            RED();
            break;

        default:
            RESET();
            break;
    }

    return;
}

/************************* INTERFACE FUNCTIONS ********************************/
void log_msg(const char *module, int line, log_lvl_t level, const char *format, ...)
{
    char timestamp[100];

    char buffer[LOG_BUFFER_SIZE];
    ssize_t buffer_len;

    int size1 = 0;
    int size = 0;
    time_t now = 0;

    now = time(0);
    strftime(timestamp, 100, "%d/%m/%Y %H:%M.%S", localtime(&now));

    // Write first part of log message
    memset(buffer, 0x00, LOG_BUFFER_SIZE);
    size1 = sprintf(buffer, " [%s] %s ", timestamp, log_lvl_str[level]);
    size = size1 + sprintf(buffer + size1, "(%d) %s: ", line, module);

    va_list argp;
    va_start(argp, format);
    size += vsnprintf(buffer + size, LOG_BUFFER_SIZE - size, format, argp);
    va_end(argp);

    log_col_line(level);
    /* print to stdout */
    fprintf(stdout, "%s\n", buffer);
    log_col_line_reset(level);

    /* Send to logging service */
    buffer_len = strlen(buffer);
    snprintf(buffer, buffer_len - 1, "%s", buffer);

    return;
}
