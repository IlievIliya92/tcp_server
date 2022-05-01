/******************************** INCLUDE FILES *******************************/
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <net/if.h>
#include <sys/ioctl.h>

/******************************** LOCAL DEFINES *******************************/

/*************************** INTERFACE FUNCTIONS ******************************/
int net_get_iface_index(int socket_fd, const char *iface_name)
{
    struct ifreq ifr;

    memset(&ifr, 0x00, sizeof(struct ifreq));

    strncpy((char *)ifr.ifr_name, iface_name, IFNAMSIZ);
    if((ioctl(socket_fd, SIOCGIFINDEX, &ifr)) == -1)
    {
        fprintf(stderr, "Failed to get interface index: %s", strerror(errno));
        return -1;
    }

    return ifr.ifr_ifindex;
}
