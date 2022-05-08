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
The example callback function simply reads the inbound data and writes it back
to the connected client. (loopback). The '1500' size of the *rx_buffer* is the standard
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

## Client - Server Example

The client - server example illustrates the server handling multiple client
connections spawned from multiple threads.

```{note}
The source code of the client-server example is located in the ``examples/``
directory of the project.
```

In order to run the example code the client and the server side should be started
within a seprate shell (terminal) session.

````{tab} Starting Server
```console
 $ cd build/examples
 $ ./tcp_server_example
```
````

````{tab} Running Client
```console
 $ cd examples/client
 $ python3 client.py
```
````

The default console output should look similar to the one shown below:

````{tab} Server
```console
 [08/05/2022 23:05.36] DEBUG (106) main: Using default interface for the TCP server: "lo"
 [08/05/2022 23:05.36] WARNING (119) main: Invalid number of workers (0)!
 [08/05/2022 23:05.36] INFO (120) main: Applying default number of workers: "5"
 [08/05/2022 23:05.36] WARNING (127) main: Invalid port number (0)!. Port range [1024;49151]
 [08/05/2022 23:05.36] INFO (131) main: Applying default port number: "9080"
 [08/05/2022 23:05.36] INFO (136) main: Server bind interface: lo
 [08/05/2022 23:05.36] INFO (137) main: Server port: 9080
 [08/05/2022 23:05.36] INFO (138) main: Workers: 5
 [08/05/2022 23:05.36] TRACE (80) tcp_server_server_core: TCP Server created [0x55f9f45b5b80]
 [08/05/2022 23:05.36] TRACE (60) worker_pool: Worker pool created [0x55f9f45b5c30]
 [08/05/2022 23:05.36] INFO (127) worker_pool: Worker 112741 starting ...
 [08/05/2022 23:05.36] INFO (127) worker_pool: Worker 112742 starting ...
 [08/05/2022 23:05.36] INFO (127) worker_pool: Worker 112743 starting ...
 [08/05/2022 23:05.36] INFO (127) worker_pool: Worker 112744 starting ...
 [08/05/2022 23:05.36] INFO (127) worker_pool: Worker 112745 starting ...
 [08/05/2022 23:05.40] INFO (293) tcp_server_server_core: New connection from: 127.0.0.1:39886
 [08/05/2022 23:05.40] INFO (293) tcp_server_server_core: New connection from: 127.0.0.1:39888
RX Buffer
0x000000: 44 61 74 61 20 66 72 6f 6d 20 63 6c 69 65 6e 74 Data from client
0x000010: 20 31 21                                         1!
 [08/05/2022 23:05.40] INFO (157) worker_pool: done
RX Buffer
0x000000: 44 61 74 61 20 66 72 6f 6d 20 63 6c 69 65 6e 74 Data from client
0x000010: 20 30 21                                         0!
 [08/05/2022 23:05.40] INFO (157) worker_pool: done
 [08/05/2022 23:05.40] INFO (293) tcp_server_server_core: New connection from: 127.0.0.1:39890
RX Buffer
0x000000: 44 61 74 61 20 66 72 6f 6d 20 63 6c 69 65 6e 74 Data from client
0x000010: 20 32 21                                         2!
 [08/05/2022 23:05.40] INFO (157) worker_pool: done
```
````

````{tab} Client
```console
Server address: 127.0.0.1:9080
Client Threads: 3
Starting client thread: 0.
Starting client thread: 1.
__client_1__ Sending...
Starting client thread: 2.
__client_0__ Sending...
__client_0__ Received:
b'Data from client 0!'
__client_1__ Received:
__client_2__ Sending...
Client thread 0 done
b'Data from client 1!'
__client_2__ Received:
Client thread 1 done
b'Data from client 2!'
Client thread 2 done
```
````

```{tip}
For changing the server/client network routing (ipv4/server iface/port)
use the respective binaries arguments (run ``./tcp_server_example --help`` and
``python3 client.py --help`` for more information)
```