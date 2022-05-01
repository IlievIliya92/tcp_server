/*  =========================================================================
    =========================================================================
*/

/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <sys/socket.h> /* socket */
#include <arpa/inet.h> /* endiaanes ntoh, hton utils */

#include <linux/if_packet.h>

#include "tcp_server_classes.h"

#include "utils.h"
#include "log.h"
/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME  "tcp_server_server_core"

/*********************************** TYPEDEFS *********************************/
//  Structure of our class

struct _tcp_server_t {
    int sock_fd;
};

/************************* LOCAL FUNCTIONS DEFINITIONS ************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/

/******************************* PRIVATE METHODS ******************************/

//  --------------------------------------------------------------------------
// Constructor
/**
 *
 * Create new tcp_server object.
 *
 * Returns:
 *     On success new tcp_server object, or NULL if the new tcp server could not be created.
 */
tcp_server_t *
tcp_server_new (void)
{
    tcp_server_t *self = (tcp_server_t *) smalloc (sizeof (tcp_server_t));
    assert (self);

    LOG_MSG(TRACE, "TCP Server created [%p]", self);

    return self;
}


//  --------------------------------------------------------------------------
// Destructor
/**
 *
 * Destroy tcp_server object. You must use this for any tcp server created via the
 * tcp_server_new method.
 *
 * Parameters:
 *      self_p (tcp_server_t **): pointer to tcp_server_t object reference,
 *                               so the destructor can nullify it
 *
 * Returns:
 *      On success new tcp_server object, on failiure NULL pointer.
 */
void
tcp_server_destroy (tcp_server_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        tcp_server_t *self = *self_p;
        /*
         * Free class properties here
         */

        if (self->sock_fd)
        {
            close(self->sock_fd);
        }

        LOG_MSG(TRACE, "Destroying TCP Server [%p]", self);
        //  Free object itself
        sfree (self);

        *self_p = NULL;
    }
}

int
tcp_server_init(tcp_server_t *self_p,
                const char *server_iface)
{
    assert(self_p);
    assert(server_iface);

    int ret = 0;

    return ret;
}

int
tcp_server_register_cb(tcp_server_t *self_p)
{
    assert(self_p);

    int ret = 0;

    return ret;
}

void
tcp_server_process(tcp_server_t *self_p)
{
    assert(self_p);
}

//  --------------------------------------------------------------------------
#include <signal.h>

/* State (volatile since we are stopping this from keyboard isr)*/
static volatile int SERVER_STOP;
static void server_sig_hndlr(int signum)
{
    SERVER_STOP = true;
}

void
tcp_server_test (bool verbose)
{
    LOG_MSG(TRACE, "Running %s", __func__);

    /* Create new instance of a server */
    tcp_server_t *server = tcp_server_new();

    /* Destroy server */
    tcp_server_destroy(&server);
}
