/*
    =========================================================================
    =========================================================================
*/

/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <sys/socket.h> /* socket */
#include <arpa/inet.h> /* endiaanes ntoh, hton utils */

#include <linux/if_packet.h>

#include "tcp_server_classes.h"

#include "generic_t.h"

#include "worker_pool.h"
#include "utils.h"
#include "log.h"
/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME  "tcp_server_server_core"

#define TCP_SERVER_BACKLOG  10
/*********************************** TYPEDEFS *********************************/
//  Structure of our class

struct _tcp_server_t {
    int sock_fd;

    int workers_n;
    worker_pool_t *wp;

    /* select mechanism */
    fd_set masterset;

    int maxfdp;
    int nready;
    struct timeval select_tv;
};

/************************* LOCAL FUNCTIONS DEFINITIONS ************************/

/********************************* LOCAL DATA *********************************/
/* State (volatile since we are stopping this from keyboard isr)*/
static volatile bool SERVER_STOP = true;
/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void tcp_server_sig_hndlr(int signum)
{
    SERVER_STOP = true;

    LOG_MSG(INFO, "Server shuting down...");

}
/***********************************  METHODS *********************************/

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
 *      None (void)
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

        worker_pool_destroy(&self->wp);

        LOG_MSG(TRACE, "Destroying TCP Server [%p]", self);
        //  Free object itself
        sfree (self);

        *self_p = NULL;
    }
}


int
tcp_server_sock_set_rcv_timeout(tcp_server_t *self_p,
                                int timeout)
{
    assert(self_p);

}

int
tcp_server_sock_set_send_timeout(tcp_server_t *self_p,
                                int timeout)
{
    assert(self_p);

}

/**
 *
 * Initialize properties of a tcp_server object.
 *
 * Parameters:
 *      self_p (tcp_server_t *): reference to a tcp_server_t object
 *      server_iface (const char *): name of the network interface to which the server
 *                                   will bind.
 *      server_port (int): listening port of the server
 *      workers_n (int): number of workers to dispatch
 *
 * Returns:
 *      On 0 on success, -1 on failiure.
 */
int
tcp_server_init(tcp_server_t *self_p,
                const char *server_iface,
                int server_port,
                int workers_n
                )
{
    assert(self_p);
    assert(server_iface);

    int ret = 0;
    struct sockaddr_in saddr;
    ip_parser_t sipv4;

    ret = net_get_ipv4_from_iface(server_iface, &sipv4);
    if (ret == -1)
    {
        LOG_MSG(ERR, "Failed to obtain IP address for %s", server_iface);
        return -1;
    }

    self_p->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (self_p->sock_fd == -1)
    {
        LOG_MSG(ERR, "Failed to create TCP server socket! (%s)", strerror(errno));
        return -1;
    }

    /* reuse sock */
    int optval = 1;
    ret = setsockopt(self_p->sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret == -1)
    {
        LOG_MSG(ERR, "Failed to set sock option SO_REUSEADDR! (%s)", strerror(errno));
        close(self_p->sock_fd);
        return -1;
    }

    /* Bind the socket */
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(sipv4.ip_str);
    saddr.sin_port = htons(server_port);
    ret = bind(self_p->sock_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        LOG_MSG(ERR, "bind failed! (%s)", strerror(errno));
        close(self_p->sock_fd);
        return -1;
    }

    ret = listen(self_p->sock_fd, TCP_SERVER_BACKLOG);
    if (ret == -1)
    {
        LOG_MSG(ERR, "listen failed! (%s)", strerror(errno));
        close(self_p->sock_fd);
        return -1;
    }
    /* Clear the descriptor set */
    FD_ZERO(&self_p->masterset);
    FD_SET(self_p->sock_fd, &self_p->masterset);
    self_p->maxfdp = self_p->sock_fd + 1;

    self_p->workers_n = workers_n;

    return 0;
}

/**
 *
 * Dispatch workers
 *
 * Parameters:
 *      self_p (tcp_server_t *): reference to a tcp_server_t object
 *      callback (voidVoid_ptr_t): connection callback
 *      ctx (void *): connection context
 *
 * Returns:
 *      On 0 on success, -1 on failiure.
 */
int
tcp_server_dispatch(tcp_server_t *self_p,
                    voidIntVoid_ptr_t callback,
                    void *ctx)
{
    int i = 0;

    /* Create worker pool */
    self_p->wp = worker_pool_new(self_p->workers_n);

    /* Prefork all the workers */
    for (i = 0; i < self_p->workers_n; i++)
    {
        /* parent returns */
        worker_pool_dispatch_worker(self_p->wp, i, self_p->sock_fd, callback, ctx);
        worker_pool_worker_fd_set(self_p->wp, i, &self_p->masterset);
        self_p->maxfdp = max(self_p->maxfdp, worker_pool_worker_fd_get(self_p->wp, i));
    }


    /* Register the sig handler */
    signal(SIGINT, tcp_server_sig_hndlr);

    return 0;
}

/**
 *
 * Server main processing loop.
 *
 * Parameters:
 *      self_p (tcp_server_t *): reference to a tcp_server_t object
 *
 * Returns:
 *      None (void)
 */
void
tcp_server_run(tcp_server_t *self_p)
{
    assert(self_p);
    assert(self_p->wp);

    int i = 0;
    int nsel = 0;
    int conn = -1;

    fd_set rset;

    struct sockaddr_in src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    /* Start the server loop */
    SERVER_STOP = false;
    for (;;)
    {
        rset = self_p->masterset;

        if (worker_pool_workers_avail_get(self_p->wp) <= 0)
        {
            /* turn off if no available children */
            FD_CLR(self_p->sock_fd, &rset);
        }

        self_p->select_tv.tv_sec = 2;
        self_p->select_tv.tv_usec = 0;

        nsel = select(self_p->maxfdp, &rset, NULL, NULL, NULL);

        if (SERVER_STOP)
        {
            FD_ZERO(&rset);

            break;
        }

        /* Check for new connections */
        if (FD_ISSET(self_p->sock_fd, &rset))
        {
            conn = accept(self_p->sock_fd, (struct sockaddr *)&src_addr, &src_addr_len);
            LOG_MSG(INFO, "New connection from: %s:%d",
                   inet_ntoa(src_addr.sin_addr),
                   ntohs(src_addr.sin_port));

            worker_pool_workers_submit_conn(self_p->wp, conn);
            close(conn);
            if (--nsel == 0)
            {
                continue;   /* all done with select() results */
            }
        }
        /* Find any newly-available children */
        worker_pool_workers_find_free(self_p->wp, nsel, &rset);
    }

    worker_pool_workers_terminate(self_p->wp);
}

//  --------------------------------------------------------------------------
static void server_connection_cb(int conn, void *ctx)
{
    int rx_bytes = 0;
    char rx_buffer[1500];

    rx_bytes = read(conn, rx_buffer, 1500);
    hexdump("RX Buffer", rx_buffer, rx_bytes);

    /* Just loopback */
    write(conn, rx_buffer, rx_bytes);
}

void
tcp_server_test (bool verbose)
{
    LOG_MSG(TRACE, "Running %s", __func__);

    const char *iface = "lo";
    int port = 9080;
    int workers = 2;

    /* Create new instance of a server */
    tcp_server_t *self = tcp_server_new();
    tcp_server_init(self, iface, port, workers);
    tcp_server_dispatch(self, server_connection_cb, NULL);

    tcp_server_run(self);

    /* Destroy server */
    tcp_server_destroy(&self);
}
