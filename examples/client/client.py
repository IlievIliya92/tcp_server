#!/usr/bin/python

# --- Imports --- #
import sys
import socket
import argparse
import threading

# --- Constatns --- #
# Defaults
DFLT_SERVER_IPV4_ADDR = "127.0.0.1"
DFLT_SERVER_PORT = 9080
DFLT_N_THREADS = 3

# for color output
BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)

# --- Print Utils --- #
def __has_colours(stream):
    if not hasattr(stream, "isatty"):
        return False
    if not stream.isatty():
        return False # auto color only on TTYs
    try:
        import curses
        curses.setupterm()
        return curses.tigetnum("colors") > 2
    except:
        # guess false in case of error
        return False

TERM_HAS_COLOURS = __has_colours(sys.stdout)
def printout(text, colour=BLACK):
    if TERM_HAS_COLOURS:
        seq = "\x1b[1;%dm" % (30+colour) + text + "\x1b[0m\n"
        sys.stdout.write(seq)
    else:
        print(text)

def client_thread(name, ipv4, port):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((ipv4, port))

        data = f'Data from client {name}!'
        printout('__client_%s__ Sending...' % name, YELLOW)
        s.sendall(data.encode())
        data = s.recv(1024)

        printout('__client_%s__ Received:' % name, YELLOW)
        printout(repr(data), YELLOW)
        s.close()
    except Exception as e:
        printout(str(e), RED)
        ret = -1
        raise e

# --- Main entry point --- #
def main():
    parser = argparse.ArgumentParser()
    """default arguments"""
    parser.add_argument(
        "--ipv4",
        help="Server IPv4 address",
        type=str,
        default=DFLT_SERVER_IPV4_ADDR
        )

    parser.add_argument(
        "-p",
        "--port",
        help="Server port number",
        type=int,
        default=DFLT_SERVER_PORT
    )

    parser.add_argument(
        "-n",
        "--n_threads",
        help="Number of client threads",
        type=int,
        default=DFLT_N_THREADS
    )

    args = parser.parse_args()

    printout("Server address: %s:%d" % (args.ipv4, args.port), CYAN)
    printout("Client Threads: %d" % (args.n_threads), CYAN)

    ret = 0
    cli_threads = list()
    try:

        for index in range(args.n_threads):
            printout("Starting client thread: %d." % index, MAGENTA)
            thread_id = threading.Thread(target=client_thread, args=(index, args.ipv4, args.port))
            cli_threads.append(thread_id)
            thread_id.start()

        for index, thread in enumerate(cli_threads):
            thread.join()
            printout("Client thread %d done" % index, MAGENTA)

    except Exception as e:
        printout(str(e), RED)
        ret = -1
    finally:
        return ret

if __name__ == "__main__":
    ret = main()
    sys.exit(ret)
