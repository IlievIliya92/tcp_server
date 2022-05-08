# TCP Server

The purpouse of this documentation is to provide guidance on how to install and
use the [tcp pre-forked server](https://github.com/IlievIliya92/tcp_server)

The project itself implements a TCP preforked server with only the parent process
calling accept and "passing" the connected socket to a free child worker.

The software architecture is based on the guidance provided by the
!(Unix Network Programming, Volume 1: The Sockets Networking API (3rd Edition))[https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551] - *Chapter 30. Client-Server Design Alternatives*

```{toctree}
:caption: TCP Server
:hidden:

quickstart
tcp_server
```