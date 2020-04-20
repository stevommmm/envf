CC = gcc
CFLAGS = -g -Wall -Wuninitialized -O1 -std=c11
BINARY = envf

all: $(BINARY)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARY): $(wildcard src/*.c)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) */*.o *.o $(BINARY)
