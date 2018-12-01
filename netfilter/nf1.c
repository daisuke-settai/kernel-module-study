#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <net/ip.h>


unsigned int ipv4_hook_recv1(
        void *priv,
        struct sk_buff *skb,            // パケットデータなどが格納される領域、基本的にこのデータを参照する
          const struct nf_hook_state *state     // 未調査
    ) {
    struct iphdr *iph = ip_hdr(skb);
      
      if(!iph) return NF_ACCEPT;
        if(iph->version != 4) return NF_ACCEPT;
          if((be32_to_cpu(iph->saddr) & 0xffffffff) == 0xc0a87b01) //192.168.123.1
                return NF_ACCEPT;

            printk("[ed] 1\n");
              return NF_ACCEPT;
}

EXPORT_SYMBOL(ipv4_hook_recv1);

static int __init nf1_init(void)
{
  printk("[ed] NF1 is inserted.\n");
  return 0;
}

static void __exit nf1_exit(void)
{
  printk("[ed] NF1 is removed.\n");
}

module_init(nf1_init);
module_exit(nf1_exit);
