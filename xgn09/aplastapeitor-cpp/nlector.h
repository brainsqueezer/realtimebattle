#ifndef N_LECTOR_H
#define N_LECTOR_H
//------------------------------------------------------------------------------
/**
    @class nLector

    @brief Clase que se encarga de leer los mensajes entrantes
    y almacenarlos en la memoria
*/

#include "nmemoria.h"

class nLector
{
public:
	/// constructor
	nLector();
	/// destructor
	virtual ~nLector();

	/// Bucle principal del Bot
	void Read(nMemoria *memoria);
private:
	char 	msg_aux[256];
	int 	mensaje;
	int 	par_ent_1, par_ent_2, par_ent_3;
	double 	par_dou_1, par_dou_2, par_dou_3;
};
#endif
