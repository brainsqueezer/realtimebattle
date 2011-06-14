#ifndef N_MAQUINAESTADOS_H
#define N_MAQUINAESTADOS_H

#undef N_DEFINES
#define N_DEFINES nMaquinaEstados

enum TAlfabetoEntrada
{
	BUSCAR,
    PERSEGUIR,
	ATAQUE_FINAL,
	ATAQUE_LATERAL,
	ATAQUE_KAMIKAZE,
	ATAQUE_PREVENTIVO,
	ATAQUE_COLISION,
	COGER_GALLETA,
	HUIR
};

enum TEstado
{
	BUSQUEDA,
    PERSECUCION,
	FINAL,
	LATERAL,
	KAMIKAZE,
	PREVENTIVO,
	COLISION,
	HAMBRIENTO,
	CAGUETA
};

//------------------------------------------------------------------------------
class nMaquinaEstados
{
public:
	/// constructor
	nMaquinaEstados();
	/// destructor
	virtual ~nMaquinaEstados();    
	
	bool NextStatus(int in);
	int GetStatus();
	void SetStatus(int estado);

private:
	int  estado_actual;
};

#endif
