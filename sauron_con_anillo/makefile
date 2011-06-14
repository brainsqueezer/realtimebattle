CC=gcc
CFLAGS= -g -ffast-math -fwhole-program -combine

PROGS= sauron

all: $(PROGS)

% : %.c
	$(CC) $< -o $@.robot $(CFLAGS)

clean:
	rm -f $(PROGS) *.robot