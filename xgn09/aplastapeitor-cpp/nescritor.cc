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
	Muestra un mensaje de texto de depuracion
*/
void nEscritor::Debug(const char *mensaje)
{	
	cout<<_("Debug") << " " << mensaje <<endl;
}

/**
	Muestra un mensaje de texto de depuracion con un numero
*/
void nEscritor::Debug(const char *mensaje, double numero)
{	
	cout<<_("Debug") << " " << mensaje << numero <<endl;
}

/**
	Muestra una linea
*/
void nEscritor::DebugLine(double angulo1, double radio1, double angulo2, double radio2)
{	
	cout<<_("DebugLine") << " " << angulo1 << radio1 << angulo2 << radio2 <<endl;
}

/**
	Muestra una circulo
*/
void nEscritor::DebugCircle(double anguloCentro, double radioCentro, double radioCirculo)
{	
	cout<<_("DebugCircle") << " " << anguloCentro << radioCentro << radioCirculo <<endl;
}

/**
	Muestra un mensaje de texto
*/
void nEscritor::Print(const char *mensaje)
{	
	cout<<_("Print") << " " << mensaje <<endl;
}

/**
	Muestra un mensaje de texto con un numero
*/
void nEscritor::Print(const char *mensaje, double numero)
{	
	cout<<_("Print") << " " << mensaje << numero <<endl;
}

