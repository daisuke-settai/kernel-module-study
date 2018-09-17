#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <net/ip.h>

#define N 70000 

static struct nf_hook_ops nf_ops_recv[N];
struct private_data {
  __u32 num;
} priv_data[N];

unsigned int ipv4_hook(
	void *priv,
  struct sk_buff *skb,            // パケットデータなどが格納される領域、基本的にこのデータを参照する
  const struct nf_hook_state *state     // 未調査
) {
  struct iphdr *iph = ip_hdr(skb);
  struct private_data *recv_data;
  
  if(!iph) return NF_ACCEPT;
  if(iph->version != 4) return NF_ACCEPT;
  if((be32_to_cpu(iph->saddr) & 0xffffffff) != 0xc0a87b0b) // through only 192.168.123.11 
    return NF_ACCEPT;
  recv_data = (struct private_data *)priv;
	printk("[ed] nf: %d\n", recv_data->num);
	return NF_ACCEPT;
}

int filter_test(void)
{
  int reg_r = 0;
  int i;
  /* フック関数の登録 */  
  for(i = 0; i < N; i++){
    priv_data[i].num = i;
    nf_ops_recv[i].hook     = ipv4_hook;                         /* フック関数の登録 */
    nf_ops_recv[i].priv     = (void *)&priv_data[i];                          /* .hookのprivに渡す値 */
    nf_ops_recv[i].pf       = NFPROTO_IPV4;                   /* フック対象のインターネットプロトコルファミリー */
    nf_ops_recv[i].hooknum  = NF_INET_PRE_ROUTING;              /* IPパケット受信時に呼び出す設定 */
    nf_ops_recv[i].priority = NF_IP_PRI_FIRST;                        /* 最優先で実行される */
    reg_r = nf_register_hook(&nf_ops_recv[i]);
    if (reg_r < 0) {
      printk(KERN_ALERT "Failed to register nf_register_hook.(ipv4_recv)");
      return reg_r;
    }
    printk("register nf%d\n", i);
  }
  return reg_r;
}

void cleanup(void) {
  int i;
  for(i = 0; i < N; i++){
    nf_unregister_hook(&nf_ops_recv[i]);
  }
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

