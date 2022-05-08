TCP Server
==========

Constructor
-----------

.. autocfunction:: tcp_server_core.c::tcp_server_new

Destructor
-----------

.. autocfunction:: tcp_server_core.c::tcp_server_destroy


TCP Server methods
------------------

.. autocfunction:: tcp_server_core.c::tcp_server_init

.. note::

   The connection callback function should be provided by the user application. The callback should be a function pointer of type ``void (*callbcak) (void *args)``

.. autocfunction:: tcp_server_core.c::tcp_server_run
