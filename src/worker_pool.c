/******************************** INCLUDE FILES *******************************/
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/prctl.h>  /* prctl(), PR_SET_NAME */
#include <unistd.h>

#include "tcp_server_classes.h"

#include "hexdump.h"
#include "ipc_fd.h"
#include "worker_pool.h"

#include "log.h"
/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME    "worker_pool"

/********************************* TYPEDEF ************************************/
typedef struct _worker_t {
    pid_t     pid;      /* process ID */
    int       pipefd;   /* parent's stream pipe to/from child */
    int       status;   /* 0 = ready */
    long      count;    /* # connections handled */
} worker_t;

struct _worker_pool_t {
    /* Workers to dispatch */
    int workers_n;
    int avail_workers_n;

    /* array of worker structures */
    worker_t   *worker;
};

/************************* LOCAL FUNCTIONS DEFINITIONS ************************/

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/

/*************************** INTERFACE FUNCTIONS ******************************/

//  --------------------------------------------------------------------------
// Constructor
worker_pool_t *
worker_pool_new (int workers_n)
{
    worker_pool_t *self = (worker_pool_t *) smalloc (sizeof (worker_pool_t));
    assert (self);

    self->workers_n = workers_n;
    self->avail_workers_n = self->workers_n;

    self->worker = (worker_t *) smalloc(self->workers_n * sizeof(worker_t));
    if (self->worker == NULL)
    {
        LOG_MSG(ERR, "Memory allocation failed!");
        sfree(self);
        return NULL;
    }
    LOG_MSG(TRACE, "Worker pool created [%p]", self);

    return self;
}


//  --------------------------------------------------------------------------
// Destructor
void
worker_pool_destroy (worker_pool_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        worker_pool_t *self = *self_p;
        /*
         * Free class properties here
         */
        LOG_MSG(TRACE, "Destroying worker poll [%p]", self);

        sfree(self->worker);

        //  Free object itself
        sfree (self);

        *self_p = NULL;
    }
}

pid_t
worker_pool_dispatch_worker(worker_pool_t *self_p,
                            int worker_id,
                            int listenfd,
                            voidIntVoid_ptr_t callback,
                            void *ctx)
{
    int sockfd[2] = {0, 0};
    pid_t pid = 0;
    char cmd;
    int conn = -1;
    ssize_t n = 0;

    socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
    if ((pid = fork()) > 0)
    {
        close(sockfd[1]);

        self_p->worker[worker_id].pid = pid;
        self_p->worker[worker_id].pipefd = sockfd[0];
        self_p->worker[worker_id].status = 0;

        /* parent */
        return pid;
    }

    const char *worker_ps_name = "worker";
    prctl(PR_SET_NAME, (unsigned long) worker_ps_name);

    /* child's stream pipe to parent */
    dup2(sockfd[1], STDERR_FILENO);

    close(sockfd[0]);
    close(sockfd[1]);

    /* child does not need this open */
    close(listenfd);

    /* never returns - workers routine */
    LOG_MSG(INFO, "Worker %ld starting ...", (long) getpid());

    for (;;)
    {
        if ((n = ipc_fd_read(STDERR_FILENO, &cmd, 1, &conn)) == 0)
        {
            LOG_MSG(ERR, "read_fd returned 0");
            break;
        }

        /* Received stop from the sig handler to break execution */
        if (cmd == 's')
        {
            break;
        }

        if (conn < 0)
        {
            //LOG_MSG(WARNING, "Invalid connection received on ipc_fd_read");
            continue;
        }
        else
        {
            /*
             *  processing
             */
            callback(conn, ctx);

            close(conn);
        }

        LOG_MSG(INFO, "done");
        /* tell the server we are done */
        write(STDERR_FILENO, "d", 1);
    }

    exit(0);
}

void
worker_pool_worker_fd_set(worker_pool_t *self_p,
                          int worker_id,
                          fd_set *fd)
{
    FD_SET(self_p->worker[worker_id].pipefd, fd);
}

int
worker_pool_worker_fd_get(worker_pool_t *self_p,
                          int worker_id)
{
    return self_p->worker[worker_id].pipefd;
}

int
worker_pool_workers_avail_get(worker_pool_t *self_p)
{
    return self_p->avail_workers_n;
}

int
worker_pool_workers_submit_conn(worker_pool_t *self_p, int conn)
{
    int n = 0;
    int i = 0;

    for (i = 0; i < self_p->workers_n; i++)
    {
        if (self_p->worker[i].status == 0)
        {
            /* available */
            break;
        }
    }

    if (i == self_p->workers_n)
    {
        LOG_MSG(ERR, "no available children");
    }

    self_p->worker[i].status = 1;   /* mark child as busy */
    self_p->worker[i].count++;
    self_p->avail_workers_n--;

    n = ipc_fd_write(self_p->worker[i].pipefd, "p", 1, conn);

    return n;
}

int
worker_pool_workers_find_free(worker_pool_t *self_p, int nsel, fd_set *rset)
{
    int i = 0;
    int n = 0;
    char rc = 0;

    for (i = 0; i < self_p->workers_n; i++)
    {
        if (FD_ISSET(self_p->worker[i].pipefd, rset))
        {
            if ((n = read(self_p->worker[i].pipefd, &rc, 1)) == 0)
            {
                LOG_MSG(ERR, "child %d terminated unexpectedly", i);
                // exit(-1);
                break;
            }

            /* job done
             * LOG_MSG(DEBUG, "Worker reported %c", rc);
             */
            self_p->worker[i].status = 0;
            self_p->avail_workers_n++;
            if (--nsel == 0)
            {
                break;  /* all done with select() results */
            }
        }
    }
}

int
worker_pool_workers_terminate(worker_pool_t *self_p)
{
    int i = 0;

    /* terminate all workers */
    for (i = 0; i < self_p->workers_n; i++)
    {
        //LOG_MSG(INFO, "Stopping worker %ld", self_p->worker[i].pid);
        write(self_p->worker[i].pipefd, "s", 1);
    }

    while (wait(NULL) > 0)
    {
        /* wait for all worker children */
        ;
    }
}

void worker_pool_test(bool verbose)
{
    LOG_MSG(TRACE, "Running %s", __func__);

    worker_pool_t *self =  worker_pool_new(5);

    worker_pool_destroy(&self);
}
