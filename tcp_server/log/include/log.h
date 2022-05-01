#ifndef _LOG_H_
#define _LOG_H_
/******************************** INCLUDE FILES *******************************/
#include <stdarg.h>
#include <string.h>

/*********************************** DEFINES **********************************/
typedef enum _log_lvl_t
{
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERR,
  CRITICAL,
  LVLS
} log_lvl_t;

#define LOG_MSG(level, msg, ...) log_msg(MODULE_NAME, __LINE__, level, msg, ## __VA_ARGS__)

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
void log_msg(const char *module, int line, log_lvl_t level, const char *format_str, ...);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* _LOG_H_ */

