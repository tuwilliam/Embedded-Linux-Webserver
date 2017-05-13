CFLAGS = -lsqlite3 -L/scrcode/sqlite/sqlite-arm/lib -I/scrcode/sqlite/sqlite-arm/include
TARGET = user
INCLUDE = ../../common

$(TARGET): %:%.c
	$(CROSS_COMPILE)gcc -o $(TARGET) $< $(INCLUDE)/*.c $(CFLAGS)
clean:
	rm -f $(TARGET)
