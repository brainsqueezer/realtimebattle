aplastapeitor.robot: aplastapeitor.cc maquinaestados.cc nmemoria.cc nescritor.cc nlector.cc nia.cc nbot.cc
	g++ maquinaestados.cc -c
	g++ nmemoria.cc -c
	g++ nescritor.cc -c
	g++ nlector.cc -c 
	g++ nia.cc -c 
	g++ nbot.cc -c
	g++ aplastapeitor.cc -o aplastapeitor.robot nbot.o nlector.o nescritor.o nia.o nmemoria.o maquinaestados.o

clean: 
	rm aplastapeitor.robot
	rm *.o

install:
	sudo rm -f /usr/lib/realtimebattle/Robots/aplastapeitor51.robot
	sudo cp aplastapeitor.robot /usr/lib/realtimebattle/Robots/aplastapeitor51.robot
	
all:
	make
	make install
	make clean
