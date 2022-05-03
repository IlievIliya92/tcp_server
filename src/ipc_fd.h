#ifndef _IPC_FD_H_
#define _IPC_FD_H_

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
ssize_t ipc_fd_read(int fd, void *ptr, size_t nbytes, int *recvfd);
ssize_t ipc_fd_write(int fd, void *ptr, size_t nbytes, int sendfd);

#endif /* _IPC_FD_H_ */

