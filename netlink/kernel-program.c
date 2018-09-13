#include <uapi/linux/pkt_cls.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/sock.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define USER_SOCK 30 

struct sock *nl_sk = NULL;

static void hello_nl_recv_msg(struct sk_buff *skb)
{
  struct nlmsghdr *nlh;
  int pid;
  struct sk_buff *oskb;
  int msg_size;
  char *msg = "hello from kernel module.";
  int res;

  printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
  
  nlh = (struct nlmsghdr *)skb->data;
  printk(KERN_INFO "Netlink received msg payload: %s\n", (char *)NLMSG_DATA(nlh));
  pid = nlh->nlmsg_pid;

  msg_size = strlen(msg);
  if(!(oskb = nlmsg_new(msg_size, 0))) {
    printk(KERN_ERR "Failed to allocate new skb\n");
    return;
  }
  nlh = nlmsg_put(oskb, 0, 0, NLMSG_DONE, msg_size, 0);
  NETLINK_CB(oskb).dst_group = 0;
  strncpy(nlmsg_data(nlh), msg, msg_size);

  if((res = nlmsg_unicast(nl_sk, oskb, pid)) < 0) {
    printk(KERN_INFO "Error: sending back to user\n");
  }
  printk("About to send msg bak:%s\n", msg); 
}

static int __init hello_init(void)
{
  printk("Entering: %s\n", __FUNCTION__);
  struct netlink_kernel_cfg cfg = {
    .input = hello_nl_recv_msg,
  };
  nl_sk = netlink_kernel_create(&init_net, USER_SOCK, &cfg);
  if(!nl_sk){
    printk(KERN_ALERT "Error creating socket.\n");
    return -1;
  }
  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "exiting hello module\n");
  netlink_kernel_release(nl_sk);
}

module_init(hello_init);
module_exit(hello_exit);
