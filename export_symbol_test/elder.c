#include <linux/module.h>
#include <linux/types.h>
#include "test.h"

MODULE_LICENSE("GPL v2");

struct nf_targets targets = {
  .nf_target_num = 'c',
};
EXPORT_SYMBOL(targets);

void hello(void) {
  printk("hello!%c\n", targets.nf_target_num);
}

EXPORT_SYMBOL(hello);

void set(__u8 data) {
  targets.nf_target_num = data;
  printk("set %c\n", data);
}

EXPORT_SYMBOL(set);

static int mymodule_init(void) {
//  targets.nf_target_num = 'd';
  printk(KERN_ALERT "elder!%c\n", targets.nf_target_num);
  test("test!\n");
  return 0;
}

static void mymodule_exit(void) {
  /* Do nothing */
}

module_init(mymodule_init);
module_exit(mymodule_exit);
