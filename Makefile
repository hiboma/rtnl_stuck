obj-m := rtnl_stuck.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
insmod:
	insmod rtnl_stuck.ko
rmmod:
	rmmod rtnl_stuck.ko

disk:
	dd if=/dev/zero of=/tmp/disk bs=1M count=1000
	mkfs.xfs -f /tmp/disk

mount:
	mount -o loop -t xfs /tmp/disk /mnt

umount:
	umount /mnt

romount:
	mount -o loop -r -t xfs /tmp/disk /mnt

dropcache:
	sysctl vm.drop_caches=3
tune:
	sysctl fs.xfs.xfssyncd_centisecs=100
