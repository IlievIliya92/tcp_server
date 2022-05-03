#!/usr/bin/python

# --- Imports --- #
import sys
import socket
import argparse

# --- Constatns --- #
# Defaults
DFLT_SERVER_IPV4_ADDR = "127.0.0.1"
DFLT_SERVER_PORT = 9080

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
    args = parser.parse_args()

    printout("Sending test data to %s:%d" % (args.ipv4, args.port), CYAN)

    ret = 0
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((args.ipv4, args.port))

        data = 'Hello from py client!'
        s.sendall(data.encode())
        data = s.recv(1024)

        print('Received', repr(data))
        s.close()
    except Exception as e:
        printout(str(e), RED)
        ret = -1
        raise e
    finally:
        return ret

if __name__ == "__main__":
    ret = main()
    sys.exit(ret)
