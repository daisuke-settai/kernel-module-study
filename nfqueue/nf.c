#include <linux/module.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <net/ip.h>


#define QUEUE_ID  60000
static struct nf_hook_ops nf_ops;
static struct nf_hook_ops nf_ops1;

unsigned int nf_queue_in (void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
  struct iphdr *iph = ip_hdr(skb);
  if(!iph) return NF_ACCEPT;
  if(iph->version != 4) return NF_ACCEPT;
  if((be32_to_cpu(iph->saddr) & 0xffffffff) == 0xc0a87b01)  // 192.168.123.1
    return NF_ACCEPT;

  printk("queue in:%u\n", skb->queue_mapping);
  return NF_QUEUE + (QUEUE_ID << NF_VERDICT_QBITS);
}

unsigned int hello (void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
  int i;
  struct iphdr *iph = ip_hdr(skb);
  if(!iph) return NF_ACCEPT;
  if(iph->version != 4) return NF_ACCEPT;
  if((be32_to_cpu(iph->saddr) & 0xffffffff) == 0xc0a87b01)  // 192.168.123.1
    return NF_ACCEPT;

  //i = *(int *)priv;

  printk("return from user:%u\n", skb->queue_mapping);
  return NF_ACCEPT; 
}



static int __init nf1_init(void)
{ 
  printk("[ed] NF1 is inserted.\n");
  printk("queue in: 2\n");

  nf_ops1.hooknum = NF_INET_PRE_ROUTING;
  nf_ops1.priority = NF_IP_PRI_FIRST + 1;
  nf_ops1.pf = NFPROTO_IPV4;
  nf_ops1.hook = hello;

  nf_ops.hooknum = NF_INET_PRE_ROUTING;
  nf_ops.priority = NF_IP_PRI_FIRST;
  nf_ops.pf = NFPROTO_IPV4;
  nf_ops.hook = nf_queue_in;

  printk(KERN_INFO "register\n");
  nf_register_hook(&nf_ops);
  nf_register_hook(&nf_ops1);

  return 0;
}

static void __exit nf1_exit(void)
{
  printk("[ed] NF1 is removed.\n");
  nf_unregister_hook(&nf_ops);
  nf_unregister_hook(&nf_ops1);
}

module_init(nf1_init);
module_exit(nf1_exit);
