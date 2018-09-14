#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>

static struct nf_hook_ops nf_ops_recv;

//受信時はout = NULL, 送信時はin = NULL
unsigned int ipv4_hook_recv(
	const struct nf_hook_ops *ops,  // たぶん登録したnf_hook_opsの情報が入っている
  struct sk_buff *skb,            // パケットデータなどが格納される領域、基本的にこのデータを参照する
  const struct net_device *in,    // パケットを受信したデバイス名（ネットワークデバイスに関する情報が入っている)
  const struct net_device *out,   // パケットを送信しようとするデバイス名
  struct nf_hook_state *state     // 未調査
) {
	printk("[ed] ipv4_hook_recv\n");
	return 0;
}

int filter_test(void)
{
  // IPv4 パケット受信時に動作する関数の登録
  nf_ops_recv.hook     = ipv4_hook_recv;                         /* フック関数の登録 */
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
}

void cleanup(void) {
  nf_unregister_hook(&nf_ops_recv);
 }

static void __init nf_init(void)
{
  printk("netfilter init\n");
  filter_test();
}

static void __exit nf_exit(void)
{
  printk("netfilter exit\n");
  cleanup();
}

module_init(nf_init);
module_exit(nf_exit);

