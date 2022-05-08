# Examples

This page provides some basic examples on how to use the classes & methods
provided by the [tcp server lib](./tcp_server.rst)

## Dependencies

* libtcp_server

```C
#include "tcp_server.h"  /* tcp server classes */
```

```{hint}
You can also use the *utils* and *log* libs in your project. Check the *examples*
directory for reference usage of these libs.
```

## Create a new instance of the TCP server

```C
tcp_server_t *tcp_server = NULL;
tcp_server = tcp_server_new();
```

## Initialize the TCP server

In order to initialize the TCP server it is required to provide a callback
function to process the TCP inbound connection.

The callback function should be the following type:

```C
void (*callback) (void *);
```

Example callback function:

```C
static void my_connection_callback(void *args)
{
    int conn = -1;
    int rx_bytes = 0;
    char rx_buffer[1500];

    conn = *(int *)args;

    rx_bytes = read(conn, rx_buffer, 1500);
    /* Just loopback */
    write(conn, rx_buffer, rx_bytes);
}
```

```{note}
The example callback function simple reads the inbound data and writes it back
to the connection. (loopback). The '1500' size of the *rx_buffer* is the standard
MTU size of a standard packet sent over an Ethernet v2.
```

Initialize the server:

```C
const char *iface = "eth0";
int port = 9010;
int workers = 5;

tcp_server_init(tcp_server, iface, port, my_connection_callback, workers);
```

The following example initializes the server with 5 workers. The port of the server
is *9010* and it is bind on the *eth0* interface. The connection callback
is the *my_connection_callback* function shown above.

## Run the TCP server

Run the server loop by invocking the *tcp_server_run* function, passing
the *tcp_server* initialized tcp server object to it.

```C
tcp_server_run(tcp_server);
```

Beyond this point the server loop takes control of the program execution,
no code will be executed until Ctrl+C (SIGINT) interrupt is received.

## Destroy the TCP server

Deallocate the server resources with the *tcp_server_destroy* function

```C
tcp_server_destroy(&tcp_server);
```


## Complete example

```C
/******************************** INCLUDE FILES *******************************/
#include <stdlib.h>
#include <unistd.h>

#include "tcp_server.h"  /* tcp server classes */

/******************************* LOCAL FUNCTIONS ******************************/
static void my_connection_callback(void *args)
{
    int conn = -1;
    int rx_bytes = 0;
    char rx_buffer[1500];

    conn = *(int *)args;
    rx_bytes = read(conn, rx_buffer, 1500);
    /* Just loopback */
    write(conn, rx_buffer, rx_bytes);
}

/********************************** MAIN **************************************/
int main(int argc, char *argv[])
{
    const char *iface = "eth0";
    int port = 9010;
    int workers = 5;

    /* Create new instance of a server */
    tcp_server = tcp_server_new();

    /* Initialize the server */
    tcp_server_init(tcp_server, iface, port, my_connection_callback, workers);

    /* Server loop */
    tcp_server_run(tcp_server);

    /* Destroy server */
    tcp_server_destroy(&tcp_server);

    return 0;
}
```

```{seealso}
For a fully functional client-server example check the examples directory.
```
