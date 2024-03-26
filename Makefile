CC=gcc
CFLAGS=-Wall
EXEC=test
OBJS=weird.o

.PHONY: all

all: $(EXEC)

# Make Objects
$(OBJS): weird.c weird.h scan_token.h kmap.h
	$(CC) $(CFLAGS) -c weird.c -o $@

# Make Executables
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o test

clean:
	rm -fr $(OBJS) $(BINS)
