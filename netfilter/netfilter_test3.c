#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <net/ip.h>

static struct nf_hook_ops nf_ops_recv;
extern unsigned int ipv4_hook_recv1(void *, struct sk_buff *, const struct nf_hook_state *);

int filter_test(void)
{
  // IPv4 パケット受信時に動作する関数の登録
  nf_ops_recv.hook     = ipv4_hook_recv1;                         /* フック関数の登録 */
  nf_ops_recv.priv     = NULL;                          /* .hookのprivに渡す値 */
  nf_ops_recv.pf       = NFPROTO_IPV4;                   /* フック対象のインターネットプロトコルファミリー */
  nf_ops_recv.hooknum  = NF_INET_PRE_ROUTING;              /* IPパケット受信時に呼び出す設定 */
  nf_ops_recv.priority = NF_IP_PRI_FIRST;                        /* 最優先で実行される */
  int reg_r = 0;

  /* フック関数の登録 */
  reg_r = nf_register_hook(&nf_ops_recv);
  if (reg_r < 0) {
    printk(KERN_ALERT "Failed to register nf_register_hook.(ipv4_recv)");
    return reg_r;
  }
  printk("register nf\n");

  return reg_r;
}

void cleanup(void) {
  nf_unregister_hook(&nf_ops_recv);
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

