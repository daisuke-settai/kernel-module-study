cmd_/home/ed/workspace/kernel-module-study/kthread/thread.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/ed/workspace/kernel-module-study/kthread/thread.ko /home/ed/workspace/kernel-module-study/kthread/thread.o /home/ed/workspace/kernel-module-study/kthread/thread.mod.o