CC=g++
CFLAGS= -g -ffast-math -fwhole-program -combine

PROGS=bigpresspuntonet_es_nan 

all: $(PROGS)

% : %.cpp
	$(CC) $< -o $@.robot $(CFLAGS)

clean:
	rm -f $(PROGS) *.robot

copy:
	sudo cp $(PROGS).robot /usr/lib/realtimebattle/Robots/
