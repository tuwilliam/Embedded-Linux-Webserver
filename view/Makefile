CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include
TARGET = view
INCLUDE += ../common

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) $< $(INCLUDE)/*.c ../model/session.c $(CFLAGS)
clean:
	rm -f $(TARGET)
