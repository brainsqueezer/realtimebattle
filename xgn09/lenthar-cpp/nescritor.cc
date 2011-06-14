#define N_IMPLEMENTS nEscritor

#include "nescritor.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

#include "Messagetypes.h"
#include "IntlDefs.h"

//------------------------------------------------------------------------------
/**
*/
nEscritor::nEscritor()
{
}

//------------------------------------------------------------------------------
/**
*/
nEscritor::~nEscritor()
{
}

/**
	Escribe el nombre y el color del Bot
*/
void nEscritor::Initialize(const char *nombre,const char *color1,const char *color2)
{
	cout<<_("Name")	  << " " << nombre << endl;
	cout<<_("Colour") << " " << color1 << " " << color2 <<endl;
}

/**
	Acelera
*/
void nEscritor::Accelerate(double aceleracion)
{
	cout<<_("Accelerate") << " " << aceleracion <<endl;
}

/**
	Frena
*/
void nEscritor::Brake(double frenada)
{
	cout<<_("Brake") << " " << frenada <<endl;
}

/**
	Opcion de robot
*/
void nEscritor::RobotOption(int opcion, int valor)
{
	cout<<_("RobotOption") << " " << opcion << " " << valor <<endl;
}

/**
	Rota
*/
void nEscritor::Rotate(int elemento, double velocidad)
{
	cout<<_("Rotate") << " " << elemento << " " << velocidad <<endl;
}

/**
	Rota Hacia un angulo
*/
void nEscritor::RotateTo(int elemento, double velocidad, double angulo_destino)
{
	cout<<_("RotateTo") << " " << elemento << " " << velocidad << " " << angulo_destino <<endl;
}

/**
	Rota un ángulo determinado
*/
void nEscritor::RotateAmount(int elemento, double velocidad, double angulo)
{
	cout<<_("RotateAmount") << " " << elemento << " " << velocidad << " " << angulo <<endl;
}

/**
	"Barre" un ángulo
*/
void nEscritor::Sweep(int elemento, double velocidad, double angulo_izquierdo, double angulo_derecho)
{
	cout<<_("Sweep") << " " << elemento << " " << velocidad << " " << angulo_izquierdo << " " << angulo_derecho <<endl;
}

/**
	Dispara
*/
void nEscritor::Shoot(double energia)
{
	cout<<_("Shoot") << " " << energia <<endl;
}

/**
	Muestra un mensaje de texto
*/
void nEscritor::Print(const char *mensaje)
{	
	cout<<_("Debug") << " " << mensaje <<endl;
}

/**
	Muestra un mensaje de texto con un numero
*/
void nEscritor::Print(const char *mensaje, double numero)
{	
	cout<<_("Debug") << " " << mensaje << numero <<endl;
}
