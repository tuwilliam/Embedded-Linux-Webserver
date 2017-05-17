CFLAGS = -lsqlite3 -L/home/jay/work/sqlite-arm/lib -I/home/jay/work/sqlite-arm/include
TARGET = sqlitetest

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) $< $(CFLAGS)
clean:
	rm -f $(TARGET)