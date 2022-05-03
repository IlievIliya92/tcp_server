#ifndef _NET_H_
#define _NET_H_

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>

/********************************** DEFINES  **********************************/
#define IP_NAME_STR_SIZE 50

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/************************** INTERFACE DATA DEFINITIONS ************************/
typedef struct _ip_parser_t {
  int ipv;
  uint32_t dwords[4];
  char ip_str[IP_NAME_STR_SIZE];
} ip_parser_t;

/*************************** INTERFACE FUNCTIONS ******************************/
int net_get_iface_index(int socket, const char *iface_name);
int net_parse_ipv4(const char *ip_str, ip_parser_t *ip_out);
int net_get_ipv4_from_iface(const char *iface, ip_parser_t *ipv4);

#endif /* _NET_H_ */

