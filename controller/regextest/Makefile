SYSROOTPATH ?=/home/jay/work/31452/rootfs
CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include \
		-march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=${SYSROOTPATH}
TARGET = regextest
INCLUDE = ../../common

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) $< $(CFLAGS)
clean:
	rm -f $(TARGET)
