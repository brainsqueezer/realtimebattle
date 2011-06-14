CC=g++
CFLAGS= -g -ffast-math -fwhole-program -combine

PROGS=camperbot 

all: $(PROGS)

% : %.cpp
	$(CC) $< -o $@.robot $(CFLAGS)

clean:
	rm -f $(PROGS) *.robot

copy:
	sudo cp $(PROGS).robot /usr/lib/realtimebattle/Robots/
