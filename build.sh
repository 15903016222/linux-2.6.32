export PATH=/stuff/tmp/sysroots/i686-linux/usr/armv7a/bin:$PATH

make CROSS_COMPILE=arm-angstrom-linux-gnueabi-  ARCH=arm uImage
