CC=gcc 
CFLAGS=-O0 -g -Wall -fmessage-length=0
DEPOS= errors.o tokenizer.o Bank.o linkedlist.o requests.o main.o

all: appserver appserver-coarse

appserver: $(DEPOS)
	$(CC) -o appserver appserver.c $(DEPOS) $(CFLAGS) -lpthread

appserver-coarse: $(DEPOS)
	$(CC) -o appserver-coarse appserver-coarse.c $(DEPOS) $(CFLAGS) -lpthread

clean:
	rm -f appserver appserver-coarse $(DEPOS)
