DIR=/home/build/openwrt/OpenWrt-Toolchain/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin
STAGING_DIR := $(DIR) 
export STAGING_DIR

CC=$(DIR)/mips-openwrt-linux-gcc
CFLAGS=
LDFLAGS=

all: readtemp

readtemp: readtemp.o
	$(CC) readtemp.o -o readtemp

readtemp.o: readtemp.c
	$(CC) $(CFLAGS) -c readtemp.c

clean:
	rm *.o readtemp
