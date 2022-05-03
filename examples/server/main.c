/******************************** INCLUDE FILES *******************************/
#include <stdlib.h>
#include <argp.h>

#include <unistd.h>

#include "tcp_server.h"  /* tcp server classes */

#include "utils.h"
#include "log.h"
/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME     "main"

/* Default values */
#define TCP_SERVER_DFLT_IFACE     "lo"
#define TCP_SERVER_DFLT_PORT      9080
#define TCP_SERVER_DFLT_WORKERS_N 5

#define TCP_PORT_MIN 1024
#define TCP_PORT_MAX 49151
/******************************* LOCAL TYPEDEFS ******************************/
typedef struct _serverArgs_t
{
    /* Host network interface */
    const char *iface;
    /* server port */
    int port;
    /* Server workers */
    int workers;
} serverArgs_t;

/********************************* LOCAL DATA *********************************/
/* Input args table */
static struct argp_option options[] = {
    {"iface", 'i', "iface", 0, "Host network interface to bind the server on"},
    {"port", 'p', "port", 0, "TCP server port"},
    {"workers", 'w', "workers", 0, "Number of preforked TCP server workers"},
    { 0 }
};

/******************************* LOCAL FUNCTIONS ******************************/
static
void usage(const char *exec_name)
{
    LOG_MSG(INFO, "%s -i [iface] -p [port] -w [workers] ", ( char *)exec_name);
    LOG_MSG(INFO, "Run '%s --help' for more information", ( char *)exec_name);

    return;
}

/*********************** INPUT ARGS PARSE CHECK FUNCS *************************/
static
error_t parse_option( int key, char *arg, struct argp_state *state )
{
     serverArgs_t *arguments = state->input;

    switch (key) {
        case 'i':
            arguments->iface = arg;
            break;
        case 'p':
            arguments->port = atoi(arg);
            break;
        case 'w':
            arguments->workers = atoi(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
static struct argp argp = {options, parse_option};

/*************************** SERVER CONNECTION CB *****************************/
static void my_connection_callback(void *args)
{
    int conn = -1;
    int rx_bytes = 0;
    char rx_buffer[1500];

    conn = *(int *)args;
    rx_bytes = read(conn, rx_buffer, 1500);
    hexdump("RX Buffer", rx_buffer, rx_bytes);
    /* Just loopback */
    write(conn, rx_buffer, rx_bytes);
}

/********************************** MAIN **************************************/
int main(int argc, char *argv[])
{
    int i = 0;
    int ret = 0;
    serverArgs_t args = { 0 };
    tcp_server_t *tcp_server = NULL;

    if(0 != argp_parse(&argp, argc, argv, 0, 0, &args))
    {
        usage(argv[0]);
        return -1;
    }

    /* Validate input args */
    if (args.iface == NULL)
    {
        LOG_MSG(DEBUG, "Using default interface for the TCP server: \"%s\"", TCP_SERVER_DFLT_IFACE);
        args.iface = TCP_SERVER_DFLT_IFACE;
    }

    /* Check if the interface is up  */
    if (!net_is_iface_up(args.iface))
    {
        LOG_MSG(ERR, "The interface \"%s\" apeears to be down or not configured properly", args.iface);
        return -1;
    }

    if(args.workers <= 0)
    {
        LOG_MSG(WARNING, "Invalid number of workers (%d)!", args.workers);
        LOG_MSG(INFO, "Applying default number of workers: \"%d\"", TCP_SERVER_DFLT_WORKERS_N);
        args.workers = TCP_SERVER_DFLT_WORKERS_N;
    }

    /* Lets keep ourselves in the user/apps ports range */
    if(args.port < TCP_PORT_MIN || args.port > TCP_PORT_MAX)
    {
        LOG_MSG(WARNING, "Invalid port number (%d)!. Port range [%d;%d]",
                          args.port,
                          TCP_PORT_MIN,
                          TCP_PORT_MAX);
        LOG_MSG(INFO, "Applying default port number: \"%d\"", TCP_SERVER_DFLT_PORT);
        args.port = TCP_SERVER_DFLT_PORT;
    }

    /* TCP server params */
    LOG_MSG(INFO, "Server bind interface: %s", args.iface);
    LOG_MSG(INFO, "Server port: %d", args.port);
    LOG_MSG(INFO, "Workers: %d", args.workers);


    /* Create new instance of a server */
    tcp_server = tcp_server_new();
    tcp_server_init(tcp_server, args.iface, args.port, my_connection_callback, args.workers);

    tcp_server_run(tcp_server);

    /* Destroy server */
    tcp_server_destroy(&tcp_server);

    return ret;
}
