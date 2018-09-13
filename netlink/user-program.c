#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_SOCK 30 
#define MAX_PAYLOAD 1024

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;

int netlink_socket;
struct iovec iov;
struct msghdr msg;

int main(){
  netlink_socket = socket(AF_NETLINK, SOCK_RAW, USER_SOCK);
  if(netlink_socket < 0)
    return -1;

  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.nl_family = AF_NETLINK;
  src_addr.nl_pid = getpid();
  
  bind(netlink_socket, (struct sockaddr*)&src_addr, sizeof(src_addr));
  
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0; // to linux kernel process
  dest_addr.nl_groups = 0; //unicast

  printf("stage 1\n");
  nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
  memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
  printf("memset\n");
  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
  nlh->nlmsg_pid = getpid();
  nlh->nlmsg_flags = 0;

  printf("strcpy\n");

  strcpy(NLMSG_DATA(nlh), "Hello");

  printf("stage 1\n");
  iov.iov_base = (void *)nlh;
  iov.iov_len = nlh->nlmsg_len;
  msg.msg_name = (void *)&dest_addr;
  msg.msg_namelen = sizeof(dest_addr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  printf("Sending message to kernel\n");
  sendmsg(netlink_socket, &msg, 0);
  printf("Waiting for message from kernel\n");

  recvmsg(netlink_socket, &msg, 0);
  printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
  close(netlink_socket);

  return 0;
}
