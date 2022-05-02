#ifndef _WORKER_POOL_H_
#define _WORKER_POOL_H_

/******************************** INCLUDE FILES *******************************/
#include <stddef.h>
#include <stdbool.h>

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/
typedef struct _worker_pool_t worker_pool_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
// Constructor
worker_pool_t *
worker_pool_new (int workers_n);

//  --------------------------------------------------------------------------
// Destructor
void
worker_pool_destroy (worker_pool_t **self_p);

void
worker_pool_worker_fd_set(worker_pool_t *self_p,
                          int worker_id,
                          fd_set *fd);

int
worker_pool_worker_fd_get(worker_pool_t *self_p,
                          int worker_id);

pid_t
worker_pool_dispatch_worker(worker_pool_t *self_p,
                            int worker_id,
                            int listenfd);

int
worker_pool_workers_avail_get(worker_pool_t *self_p);

int
worker_pool_workers_submit_conn(worker_pool_t *self_p, int conn);

int
worker_pool_workers_find_free(worker_pool_t *self_p, int nsel, fd_set *rset);

int
worker_pool_workers_terminate(worker_pool_t *self_p);

void
worker_pool_test(bool verbose);

#ifdef __cplusplus
}
#endif
#endif /* _WORKER_POOL_H_ */
