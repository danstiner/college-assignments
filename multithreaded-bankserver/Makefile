CC=gcc 
CFLAGS=-O0 -g -Wall -fmessage-length=0
DEPOS= accounts.o common.o Bank.o errors.o linkedlist.o main.o requests.o tokenizer.o request_check.o request_transaction.o

all: appserver appserver-coarse

appserver: $(DEPOS)
	$(CC) -o appserver appserver.c $(DEPOS) $(CFLAGS) -lpthread

appserver-coarse: $(DEPOS)
	$(CC) -o appserver-coarse appserver-coarse.c $(DEPOS) $(CFLAGS) -lpthread

clean:
	rm -f appserver appserver-coarse $(DEPOS)
