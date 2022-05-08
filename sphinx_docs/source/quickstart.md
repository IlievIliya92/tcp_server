# Quickstart

```{article-info}
:avatar-outline: muted
:date: May 8, 2022
:read-time: 4 min read
```

This page describes the required steps to clone and build the libtcp_server.so shared lib.

## Dependencies

* cmake (>= 3.9) -- build system
* python3 -- client side test application

## Initial Setup

1. Clone the project using git

```{tab} SSH
    $ git clone git@github.com:IlievIliya92/tcp_server.git
```

```{tab} HTTPS
    $ git clone https://github.com/IlievIliya92/tcp_server.git
```

2. Build

The currently supported build system is CMake based. The build process is pretty standard
for a CMake project. To build the .so lib, tests and example binaries follow
the instructions below:

```console
$ cd tcp_server
$ mkdir build
$ cd build
$ cmake ..
$ make
```

````{tip}
You can also use the following sequence of commands for quick setup:

```console
$ source configure
$ run_cmake build
```
````

3. Validate the build

In order to validate the build use the *tcp_server_selftest* utility, located
in the build dorectory.

```
$ cd build
$ ./tcp_server_selftest
```

```{note}
    By default the selftest starts the TCP server on port '9080', binding the server
    on the 'lo' (loopback) interface. The default number of pre-forked workers is 2.
```

Congratulations! You made it this far. If the set up wasn't as smooth as described
above report any issues using the [support page](./support). If you have managed
to build and run the selftest reward yourself with a smile and remember a smile is a curve
that sets things straight.

![](https://cdn-icons-png.flaticon.com/512/7420/7420425.png)
