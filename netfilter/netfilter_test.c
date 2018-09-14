#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <net/ip.h>

static struct nf_hook_ops nf_ops_recv;
static struct nf_hook_ops nf_ops_recv2;

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

	printk("[ed] ipv4_hook_recv ip1: %x\n", be32_to_cpu(iph->saddr));
	printk("[ed] ipv4_hook_edit ip1: %x\n", cpu_to_be32(0xc0a87b02));
	printk("[ed] ipv4_hook_after ip1: %x\n", be32_to_cpu(cpu_to_be32(0xc0a87b02)));
  iph->saddr = cpu_to_be32(0xc0a87b02);  
	return NF_ACCEPT;
}

unsigned int ipv4_hook_recv2(
	void *priv,
  struct sk_buff *skb,            // パケットデータなどが格納される領域、基本的にこのデータを参照する
  const struct nf_hook_state *state     // 未調査
) {
  struct iphdr *iph = ip_hdr(skb);
  
  if(!iph) return NF_ACCEPT;
  if(iph->version != 4) return NF_ACCEPT;
  if((be32_to_cpu(iph->saddr) & 0xffffffff) == 0xc0a87b01) //192.168.123.1
    return NF_ACCEPT;

	printk("[ed] ipv4_hook_recv ip2: %x\n", be32_to_cpu(iph->saddr));
	return NF_ACCEPT;
}

int filter_test(void)
{
  // IPv4 パケット受信時に動作する関数の登録
  nf_ops_recv.hook     = ipv4_hook_recv1;                         /* フック関数の登録 */
  nf_ops_recv.priv     = NULL;                          /* .hookのprivに渡す値 */
  nf_ops_recv.pf       = NFPROTO_IPV4;                   /* フック対象のインターネットプロトコルファミリー */
  nf_ops_recv.hooknum  = NF_INET_PRE_ROUTING;              /* IPパケット受信時に呼び出す設定 */
  nf_ops_recv.priority = NF_IP_PRI_FIRST;                        /* 最優先で実行される */

  nf_ops_recv2.hook     = ipv4_hook_recv2;                         /* フック関数の登録 */
  nf_ops_recv2.priv     = NULL;                          /* .hookのprivに渡す値 */
  nf_ops_recv2.pf       = NFPROTO_IPV4;                   /* フック対象のインターネットプロトコルファミリー */
  nf_ops_recv2.hooknum  = NF_INET_PRE_ROUTING;              /* IPパケット受信時に呼び出す設定 */
  nf_ops_recv2.priority = NF_IP_PRI_FIRST;                        /* 最優先で実行される */
  
  int reg_r = 0;

  /* フック関数の登録 */
  reg_r = nf_register_hook(&nf_ops_recv);
  if (reg_r < 0) {
    printk(KERN_ALERT "Failed to register nf_register_hook.(ipv4_recv)");
    return reg_r;
  }
  printk("register nf\n");

  reg_r = nf_register_hook(&nf_ops_recv2);
  if (reg_r < 0) {
    printk(KERN_ALERT "Failed to register nf_register_hook.(ipv4_recv2)");
    return reg_r;
  }
  return reg_r;
}

void cleanup(void) {
  nf_unregister_hook(&nf_ops_recv);
  nf_unregister_hook(&nf_ops_recv2);
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

