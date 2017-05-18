CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include
TARGET = user
INCLUDE = ../../common

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) *.c  $(INCLUDE)/*.c $(CFLAGS)
clean:
	rm -f $(TARGET)
