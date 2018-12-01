#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/net.h>
#include <linux/netfilter/x_tables.h>

#define IPT_MIN_ALIGN (__alignof__(struct ipt_entry))
#define IPT_ALIGN(s) (((s) + ((IPT_MIN_ALIGN)-1)) & ~((IPT_MIN_ALIGN)-1))

static struct xt_table nf_table = {
  .name = "recv";
  .valid_hooks = (1 << NF_INET_PRE_ROUTING);
  .me = THIS_MODULE;
  .af = NFPROTO_IPV4;
};

int filter_test(void)
{
  unsigned int hook = NF_INET_PRE_ROUTING;
  struct xt_table *table;
  struct xt_table_info *private;
  void *table_base;
  struct ipt_entry *e, *last_e;
  struct ipt_entry_match *match_proto;
  struct ipt_entry_target *target;

  struct net *net;
  unsigned int size_ipt_entry, size_ipt_entry_match, size_ipt_entry_target, total_length;

  size_ipt_entry = IPT_ALIGN(sizeof(struct ipt_entry));
  size_ipt_entry_match = IPT_ALIGN(sizeof(struct ipt_entry_match));
  size_ipt_entry_target = IPT_ALIGN(sizeof(struct ipt_entry_target));
    
  net = current->nsproxy->net_ns;
  table = net->ipv4.nat_table;
  private = table->private;
  table_base = private->entries;

  

  return 0;
}

void cleanup(void) {
}

static int __init nf_init(void)
{
  int ret;
  printk("netfilter init\n");
  ret = filter_test();
  return ret;
}

static void __exit nf_exit(void)
{
  printk("netfilter exit\n");
  cleanup();
}

module_init(nf_init);
module_exit(nf_exit);

