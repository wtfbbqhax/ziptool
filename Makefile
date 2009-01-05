# Lame makefile just here for happiness
#
all: ziptool

ziptool: ziptool.o common.o
	gcc -o zit ziptool.o common.o

ziptool.o: ziptool.c
	gcc -Wall -O3 -c -o ziptool.o ziptool.c

common.o: common.c
	gcc -Wall -O3 -c -o common.o common.c

clean:
	rm common.o ziptool.o zit
