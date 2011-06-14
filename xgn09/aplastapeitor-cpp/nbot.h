#ifndef N_BOT_H
#define N_BOT_H
//------------------------------------------------------------------------------
/**
    @class nBot

    @brief Es el bucle principal del BOT
*/

#include "nlector.h"
#include "nescritor.h"
#include "nia.h"
#include "nmemoria.h"

class nBot
{
public:
	/// constructor
	nBot();
	/// destructor
	virtual ~nBot();

	/// Bucle principal del Bot
	void Run();

private:
	bool continuar;
	nLector *lector;
	nIA *logica;
	nMemoria* memoria;
};
#endif
