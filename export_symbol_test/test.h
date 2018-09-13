#include <linux/module.h>
struct nf_targets {
    __u8 nf_target_num;
};

void test(char *str) {
  printk(KERN_INFO "%s\n", str);
  return;
}

