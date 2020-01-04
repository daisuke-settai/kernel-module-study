#include <linux/module.h>
#include <linux/kernel.h>

struct cup {
  int water;
  int tea;
};

static int __init hello_init(void)
{
  struct cup *c1;
  c1 = 0;
  printk("hello: %d\n", c1->water);
  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "exiting hello module\n");
}

module_init(hello_init);
module_exit(hello_exit);
