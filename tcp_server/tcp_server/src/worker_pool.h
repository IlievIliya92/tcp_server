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

pid_t
worker_pool_dispatch_worker(worker_pool_t *self_p,
                            int worker_id,
                            int listenfd);

void
worker_pool_test(bool verbose);

#ifdef __cplusplus
}
#endif
#endif /* _WORKER_POOL_H_ */
