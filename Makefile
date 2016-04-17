CC = gcc
CFLAGS = -Wall

OBJECTS = rmdup.o lsdir.o listdir.o listfiles.o

project : rmdup lsdir listdir listfiles

rmdup: rmdup.o
			cc bin/rmdup.o -o bin/rmdup $(CFLAGS)
lsdir: lsdir.o
			cc bin/lsdir.o -o bin/lsdir $(CFLAGS)
listdir: listdir.o
			cc bin/listdir.o -o bin/listdir $(CFLAGS)
listfiles: listfiles.o
			cc bin/listfiles.o -o bin/listfiles $(CFLAGS)


rmdup.o: rmdup.c rmdup.h
			cc -c rmdup.c -o bin/rmdup.o $(CFLAGS)
lsdir.o: lsdir.c
			cc -c lsdir.c -o bin/lsdir.o $(CFLAGS)
listdir.o: listdir.c
			cc -c listdir.c -o bin/listdir.o $(CFLAGS)
listfiles.o: listfiles.c
			cc -c listfiles.c -o bin/listfiles.o $(CFLsAGS)
