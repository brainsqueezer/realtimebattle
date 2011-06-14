lenthar.robot: lenthar.cc maquinaestados.cc nmemoria.cc nescritor.cc nlector.cc nia.cc nbot.cc
	g++ maquinaestados.cc -c
	g++ nmemoria.cc -c
	g++ nescritor.cc -c
	g++ nlector.cc -c 
	g++ nia.cc -c 
	g++ nbot.cc -c
	g++ lenthar.cc -o lenthar.robot nbot.o nlector.o nescritor.o nia.o nmemoria.o maquinaestados.o

clean: 
	rm lenthar.robot
	rm *.o

install:
	sudo rm -f /usr/lib/realtimebattle/Robots/lenthar.robot
	sudo cp lenthar.robot /usr/lib/realtimebattle/Robots/lenthar.robot

all:
	make
	make install
	make clean
