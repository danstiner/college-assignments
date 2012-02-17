CC=gcc
CFLAGS=
DEPS = errors tokenizer commands builtins background linkedlist resh
DEPOS = errors.o tokenizer.o commands.o builtins.o background.o linkedlist.o resh.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

resh: $(DEPOS)
	$(CC) -o resh $(DEPOS)

clean:
	rm -f resh $(DEPOS)
