all: sqlitetest 
sqlitetest: sqlitetest.c sqlite3.h
	$(CROSS_COMPILE)gcc -o sqlitetest sqlitetest.c -lsqlite3 -L/scrcode/sqlite/sqlite-arm/lib -I/scrcode/sqlite/sqlite-arm/include
clean:
	rm -f sqlitetest
