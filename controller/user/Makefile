#CFLAGS=-I/home/jay/my-imx6/02_source/imx_3.0.35_4.1.0/linux-2.6-imx-imx_3.0.35_4.1.0
all: user
user: user.c
	$(CROSS_COMPILE)gcc -o user user.c ../../common/cJSON.c ../../common/common.c -lsqlite3 -L/scrcode/sqlite/sqlite-arm/lib -I/scrcode/sqlite/sqlite-arm/include
clean:
	rm -f user
