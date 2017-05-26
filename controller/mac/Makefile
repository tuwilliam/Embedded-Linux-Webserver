SYSROOTPATH ?=/home/jay/work/31452/rootfs
CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include \
		-march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=${SYSROOTPATH}
TARGET = mac
INCLUDE = ../../common
DEPENDENT = ../../uart/uart.c

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) $<  $(INCLUDE)/*.c $(DEPENDENT) $(CFLAGS)
clean:
	rm -f $(TARGET)
