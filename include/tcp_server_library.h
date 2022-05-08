/*  =========================================================================
    tcp_server - layer of public API

    =========================================================================
*/

#ifndef _TCP_SERVER_LIBRARY_H_INCLUDED_
#define _TCP_SERVER_LIBRARY_H_INCLUDED_

//  TCP_SERVER version macros for compile-time API detection
#define TCP_SERVER_VERSION_MAJOR 1
#define TCP_SERVER_VERSION_MINOR 0
#define TCP_SERVER_VERSION_PATCH 0

#define TCP_SERVER_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define TCP_SERVER_VERSION \
    TCP_SERVER_MAKE_VERSION(TCP_SERVER_VERSION_MAJOR, TCP_SERVER_VERSION_MINOR, TCP_SERVER_VERSION_PATCH)

#include <stdbool.h>
#include <stddef.h>

#define TCP_SERVER_PRIVATE __attribute__ ((visibility ("hidden")))
#define TCP_SERVER_EXPORT __attribute__ ((visibility ("default")))

//  Opaque class structures to allow forward references
typedef struct _tcp_server_t tcp_server_t;
#define TCP_SERVER_CORE_DEFINED

//  Public classes, each with its own header file
#include "tcp_server_core.h"

#endif /* _TCP_SERVER_LIBRARY_H_INCLUDED_ */
