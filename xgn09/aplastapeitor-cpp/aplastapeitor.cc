/**
    @brief Inicialización y llamada al objeto BOT

*/

#include "nbot.h"
#include <stdlib.h>

int main()
{
	nBot *bot = new nBot();
	
	// Comienzo a utilizar el bot
	bot->Run();

	delete bot;
	return (EXIT_SUCCESS);
}
