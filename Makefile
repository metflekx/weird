CC=gcc
CFLAGS=-Wall -g
EXEC=weird
OBJS=weird.o

all: $(EXEC)
# Make Objects
$(OBJS): weird.c weird.h scan_token.h kmap.h parser.h gen.h
# Make Executables
$(EXEC): $(OBJS)
clean:
	rm -fr $(OBJS) $(EXEC)
