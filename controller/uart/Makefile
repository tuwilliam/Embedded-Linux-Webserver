SYSROOTPATH ?=/home/jay/work/31452/rootfs
CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include \
		-march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=${SYSROOTPATH}
TARGET = uarttest
INCLUDE = ../../common
MODEL = ../../model

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) ./*.c  $(INCLUDE)/*.c  $(CFLAGS)
clean:
	rm -f $(TARGET)
