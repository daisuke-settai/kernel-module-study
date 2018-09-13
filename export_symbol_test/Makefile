obj-m = elder.o little.o 

all:
	make -C /lib/modules/4.4.90nfv/build M=$(PWD) modules

clean:
	make -C /lib/modules/4.4.90nfv/build M=$(PWD) clean
