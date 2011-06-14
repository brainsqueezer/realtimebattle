
#include <iostream>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "botBattle.h"

using namespace std;

botBattle bot;

void sigusr1(int sig)
{
	bot.TreatMessage();
}

int main()
{	
	cout << "RobotOption " << "3" << " " << 0 << endl;
	
	signal(SIGUSR1, sigusr1);
	
	for(;;)
	{
		sigusr1(0);
	}
	
	return 0;
}
