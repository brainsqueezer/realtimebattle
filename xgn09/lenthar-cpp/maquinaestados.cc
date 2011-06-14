#define N_IMPLEMENTS nMaquinaEstados
#include "maquinaestados.h"

//------------------------------------------------------------------------------
/**
*/
nMaquinaEstados::nMaquinaEstados()
{
	estado_actual=BUSQUEDA;
}

//------------------------------------------------------------------------------
/**
*/
nMaquinaEstados::~nMaquinaEstados()
{
}

// --------------------------------------------------------------------
bool nMaquinaEstados::NextStatus( int in )
{
	switch ( this->estado_actual )
	{
		case BUSQUEDA:
			switch ( in )
			{
				case BUSCAR:
					return true;

				case PERSEGUIR:
					return true;					

				case ATAQUE_FINAL:
					return true;	
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
			
		case FINAL:
			switch ( in )
			{
				case BUSCAR:
					return true;

				case PERSEGUIR:
					return true;					

				case ATAQUE_FINAL:
					return true;	
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;

				case ATAQUE_COLISION:
					return true;
					
				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;

		case LATERAL:
			switch ( in )
			{
				case BUSCAR:
					return true;

				case PERSEGUIR:
					return true;					

				case ATAQUE_FINAL:
					return true;	
					
				case ATAQUE_LATERAL:
					return true;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;			
			
		case PERSECUCION:
			switch ( in )
			{
				case BUSCAR:
					return true;

				case PERSEGUIR:
					return true;					

				case ATAQUE_FINAL:
					return true;
					
				case ATAQUE_LATERAL:
					return true;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		case KAMIKAZE:
			switch ( in )
			{
				case BUSCAR:
					return true;
					
				case PERSEGUIR:
					return true;

				case ATAQUE_FINAL:
					return true;
					
				case ATAQUE_LATERAL:
					return true;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		case PREVENTIVO:
			switch ( in )
			{
				case BUSCAR:
					return true;
					
				case PERSEGUIR:
					return false;

				case ATAQUE_FINAL:
					return true;
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return false;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		case COLISION:
			switch ( in )
			{
				case BUSCAR:
					return true;
					
				case PERSEGUIR:
					return false;

				case ATAQUE_FINAL:
					return true;
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return false;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		case HAMBRIENTO:
			switch ( in )
			{
				case BUSCAR:
					return true;
					
				case PERSEGUIR:
					return false;

				case ATAQUE_FINAL:
					return true;
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return true;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		case CAGUETA:
			switch ( in )
			{
				case BUSCAR:
					return true;
					
				case PERSEGUIR:
					return false;

				case ATAQUE_FINAL:
					return false;
					
				case ATAQUE_LATERAL:
					return false;
					
				case ATAQUE_KAMIKAZE:
					return false;

				case ATAQUE_PREVENTIVO:
					return true;
					
				case ATAQUE_COLISION:
					return true;

				case COGER_GALLETA:
					return true;

				case HUIR:
					return true;
			
				default:
					return false;
			}
			break;
		}
	return false;
}

int nMaquinaEstados::GetStatus()
{
	return( this->estado_actual );
}

void nMaquinaEstados::SetStatus(int estado)
{
	this->estado_actual=estado;
}
