CC = clang
CFLAGS =  -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o bf.o bst.o bv.o ht.o node.o parser.o speck.o
	$(CC) -o banhammer banhammer.o bf.o bst.o bv.o ht.o node.o parser.o speck.o $(CFLAGS) -lgmp -lm

banhammer.o: banhammer.c
	$(CC) -c banhammer.c $(CFLAGS)

bf.o: bf.c
	$(CC) -c bf.c $(CFLAGS)

bst.o: bst.c
	$(CC) -c bst.c $(CFLAGS)

bv.o: bv.c
	$(CC) -c bv.c $(CFLAGS)

ht.o: ht.c
	$(CC) -c ht.c $(CFLAGS)

node.o: node.c
	$(CC) -c node.c $(CFLAGS)

parser.o: parser.c
	$(CC) -c parser.c $(CFLAGS)

speck.o: speck.c
	$(CC) -c speck.c $(CFLAGS)

clean:
	rm -f *.o *.exe
	rm -f banhammer

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build make
