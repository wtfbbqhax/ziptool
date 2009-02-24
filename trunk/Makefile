# Lame makefile just here for happiness
#
all: ziptool

ziptool: ziptool.o zcommon.o zinfo.o
	gcc -o zit ziptool.o zcommon.o archive.o

ziptool.o: ziptool.c
	gcc -Wall -O3 -c -o ziptool.o ziptool.c

common.o: zcommon.c
	gcc -Wall -O3 -c -o zcommon.o zcommon.c

zinfo.o: archive.c
	gcc -Wall -O3 -c -o archive.o archive.c
clean:
	rm *.o zit
