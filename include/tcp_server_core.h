#ifndef _TCP_SERVER_CORE_H_
#define _TCP_SERVER_CORE_H_

/******************************** INCLUDE FILES *******************************/
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
TCP_SERVER_EXPORT tcp_server_t *
    tcp_server_new(void);

TCP_SERVER_EXPORT void
    tcp_server_destroy(tcp_server_t **self_p);

TCP_SERVER_EXPORT int
    tcp_server_init(tcp_server_t *self_p,
                    const char *server_iface,
                    int server_port,
                    int workers_n);


TCP_SERVER_EXPORT int
tcp_server_dispatch(tcp_server_t *self_p,
                    void (*cb) (int, void*),
                    void *ctx);

TCP_SERVER_EXPORT void
    tcp_server_run(tcp_server_t *self_p);

TCP_SERVER_EXPORT void
    tcp_server_test (bool verbose);

#ifdef __cplusplus
}
#endif
#endif /* _TCP_SERVER_H_ CORE_*/
