CC = gcc
CFLAGS = -g -Wall -Wuninitialized -O1 -std=c11 -static
BINARY = envf

all: $(BINARY)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARY): $(wildcard src/*.c)
	$(CC) $(CFLAGS) -o $@ $^

install: $(BINARY)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 555 $(BINARY) $(DESTDIR)$(PREFIX)/bin/

.PHONY: clean
clean:
	$(RM) */*.o *.o $(BINARY)
