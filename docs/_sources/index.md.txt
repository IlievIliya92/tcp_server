---
hide-toc: true
---

# TCP Server

The purpouse of this documentation is to provide guidance on how to install and
use the [tcp pre-forked server](https://github.com/IlievIliya92/tcp_server)

The project itself implements a TCP preforked server with only the parent process
calling accept and "passing" the connected socket to a free child worker.

The software architecture is based on the guidance provided by the
[Unix Network Programming, Volume 1: The Sockets Networking API (3rd Edition)](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551) - *Chapter 30. Client-Server Design Alternatives*

The repository [layout](./contributing/internals) is heavily influenced by the [zproject](https://github.com/zeromq/zproject).
Big credits to those guys from [iMatix](https://github.com/imatix) for making C code scalable, easy to understand and reusable!

If you need any help or assistance regarding any of the topics realted to the project
feel free to to address your inquiries using the [support page](./support).

```{toctree}
:caption: Usage
:hidden:

quickstart
tcp_server
examples
```

```{toctree}
:caption: Development
:hidden:

contributing/index
changelog
license
```

```{toctree}
:caption: Support
:hidden:

support
```
