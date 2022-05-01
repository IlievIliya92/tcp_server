/******************************** INCLUDE FILES *******************************/
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <net.h>

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

int net_parse_ipv4(const char *ip_str, ip_parser_t *ip_out)
{
  struct in_addr ipv4;

  if (inet_aton(ip_str, &ipv4) != 0)
  {
    /* IPv4 success */
    ip_out->ipv = 4;
    ip_out->dwords[0] = ipv4.s_addr;
    strcpy(ip_out->ip_str, ip_str);
  }
  else
  {
    /* Parsing failed */
    ip_out->ipv = 0;
    return -1;
  }

  return 0;
}

int net_get_ipv4_from_iface(const char *iface, ip_parser_t *ipv4)
{
    int ret = -1;
    int fd = -1;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        fprintf(stderr, "[%s] Opening socket failed! (%s)", __func__, strerror(errno));
        return -1;
    }

    /* Get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
    {
        fprintf(stderr, "[%s] Ioctl operation failed! (%s)", __func__, strerror(errno));
        ret = -1;
        goto exit;
    }

    if (!net_parse_ipv4(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr),
                         ipv4))
    {
        fprintf(stderr, "[%s] Failed to obtain valid ipv4!", __func__);
        ret = -1;
    }
    else
    {
        ret = 0;
    }

exit:
    close(fd);

    return ret;
}
