#include <linux/module.h>
#include <linux/types.h>
#include "test.h"

MODULE_LICENSE("GPL v2");

struct nf_targets targets;
void hello(void);
void set(__u8);

static int mymodule_init(void) {
  __u8 data = 'x';
  printk(KERN_ALERT "little!%c\n", targets.nf_target_num);
  printk(KERN_ALERT "little!%c\n", (&targets)->nf_target_num);

//  targets.nf_target_num = 'e';
  hello();
  set(data);
  hello();
  (&targets)->nf_target_num = 'p';
  hello();
  printk(KERN_ALERT "little!%c\n", targets.nf_target_num);
  test("test!\n");
  return 0;
}

static void mymodule_exit(void) {
  /* Do nothing */
}

module_init(mymodule_init);
module_exit(mymodule_exit);
