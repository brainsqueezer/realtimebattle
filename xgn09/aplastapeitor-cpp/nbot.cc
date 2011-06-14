#define N_IMPLEMENTS nBot

#include "nbot.h"
#include "Messagetypes.h"#include "IntlDefs.h"


//------------------------------------------------------------------------------
/**
*/
nBot::nBot()
{
	// Inicializamos las variables
	continuar 	= true;
	lector		= new nLector();
	logica		= new nIA();
	memoria		= new nMemoria();
}

//------------------------------------------------------------------------------
/**
*/
nBot::~nBot()
{
	delete lector;
	delete logica;
	delete memoria;
}

/**
*/
void nBot::Run()
{
	// Comenzamos el juego escribiendo las condiciones de funcionamiento del bot
	nEscritor *escritor = new nEscritor(); 
	escritor->RobotOption( USE_NON_BLOCKING, 0 );
	escritor->RobotOption( SEND_ROTATION_REACHED, 1 );
	delete escritor;
	
	while (continuar)
	{
		// Ejecuto una lectura de la entrada estándar
		lector->Read( memoria );
		
		// Ejecuto la lógica en función de los datos almacenados en la memoria
		continuar=logica->Run( memoria );
	}
}

