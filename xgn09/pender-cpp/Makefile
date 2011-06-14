pender.robot: pender.cc maquinaestados.cc nmemoria.cc nescritor.cc nlector.cc nia.cc nbot.cc
	g++ maquinaestados.cc -c
	g++ nmemoria.cc -c
	g++ nescritor.cc -c
	g++ nlector.cc -c 
	g++ nia.cc -c 
	g++ nbot.cc -c
	g++ pender.cc -o pender.robot nbot.o nlector.o nescritor.o nia.o nmemoria.o maquinaestados.o

clean: 
	rm pender.robot
	rm *.o

install:
	sudo rm -f /usr/lib/realtimebattle/Robots/pender.robot
	sudo cp pender.robot /usr/lib/realtimebattle/Robots/pender.robot
	
all:
	make
	make install
	make clean
