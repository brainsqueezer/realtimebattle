#ifndef N_IA_H
#define N_IA_H
//------------------------------------------------------------------------------
/**
    @class nIA

    @brief Contiene la inteligencia artificial del BOT

*/

#include "nmemoria.h"
#include "nescritor.h"
#include "maquinaestados.h"


enum tipo_rotacion
{
  ROTAR_ROBOT = 1,
  ROTAR_ARMA = 2,
  ROTAR_RADAR = 4
};

class nIA
{
public:
	/// constructor
	nIA();
	/// destructor
	virtual ~nIA();

	/// Inicializa el juego
	bool Run(nMemoria *memoria);
	
	/// Actualiza la máquina de estados
	void UpdateStatusMachine(nMemoria *memoria);
	
	/// Ejecuta un comportamiento de velocidad
	void ExecuteSpeedBehavior(nMemoria *memoria);

	/// Ejecuta un comportamiento del Radar y del Cañón
	void ExecuteRadarBehavior(nMemoria *memoria);

	/// Ejecuta un comportamiento del Robot
	void ExecuteRobotBehavior(nMemoria *memoria);

	/// Ejecuta un comportamiento de disparos
	void ExecuteShotBehavior(nMemoria *memoria);

private:
	nEscritor *escritor;
	nMaquinaEstados *estado;

	int estado_anterior;
	bool acelerando;
};
#endif
