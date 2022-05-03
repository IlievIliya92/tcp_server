# Simple TCP pre-forked server

Example implementation of a TCP preforked server with only the parent process
calling accept and "passing" the connected socket to a free child worker.

The server is wrapped in shared object lib (libtcp_server.so.1.0.0)
exporting the class & public methods making it easier to integrate with
external application software.

## Getting Started

1. Build

```console
$ git clone https://github.com/IlievIliya92/tcp_server.git
$ cd tcp_server
$ source configure
$ run_cmake build
```

2. Test the server

```console
$ start_tcp_server_selftest
```

You should see similar output to the one below:

```text
Running tcp_server test 'tcp_server'...
 [03/05/2022 13:17.02] TRACE (305) tcp_server_server_core: Running tcp_server_test
 [03/05/2022 13:17.02] TRACE (80) tcp_server_server_core: TCP Server created [0x559038712970]
 [03/05/2022 13:17.02] TRACE (60) worker_pool: Worker pool created [0x559038712a20]
 [03/05/2022 13:17.02] INFO (127) worker_pool: Worker 230170 starting ...
 [03/05/2022 13:17.02] INFO (127) worker_pool: Worker 230171 starting ...
```

## Examples

Run the examples

**Server side**

```console
$ cd tcp_server/build
$ ./examples/tcp_server_example
```

**Note:** Check *./examples/tcp_server_example --help* for available input args.

**Client side**

```console
$ cd tcp_server/examples/client
$ python3 client.py
```

**Note:** Check *python3 client.py --help* for available input args.
