cmd_/home/ed/workspace/kernel-module-study/hello/kernel-program.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/ed/workspace/kernel-module-study/hello/kernel-program.ko /home/ed/workspace/kernel-module-study/hello/kernel-program.o /home/ed/workspace/kernel-module-study/hello/kernel-program.mod.o
