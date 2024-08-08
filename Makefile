# This is a very simple makefile

OBJS=crypto2.o crc32.o stdfunc.o
INC=env
INC_PARAMS=$(INC:%=-I%)
CC = gcc
CFLAGS = -O1 -Wall -o

DESTENC = sape-enc
SOURCEENC = encode.c stdfunc.c crypto2.c crc.c
DESTDEC = sape-dec
SOURCEDEC = decode.c stdfunc.c crypto2.c crc.c

all: $(DESTENC) $(DESTDEC)

$(DESTENC): $(SOURCEENC)
	$(CC) $(INC_PARAMS) $(CFLAGS) $(DESTENC) $(SOURCEENC)

$(DESTDEC): $(SOURCEDEC)
	$(CC) $(INC_PARAMS) $(CFLAGS) $(DESTDEC) $(SOURCEDEC)

clean:
	rm -f *.o
	rm -f $(DESTENC)
	rm -f $(DESTDEC)
	