#define N_IMPLEMENTS nIA

#include "nia.h"
#include "Messagetypes.h"
#include <math.h>

#define abs(x) ((x>0) ? (x) : -(x))
#define sgn(x) ((x>0) ? 1 : -1)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define mod(a,b) ( a - floor( a / b ) * b )
#define exp(a) ( a * a )

//------------------------------------------------------------------------------
/**
*/
nIA::nIA()
{
	escritor = new nEscritor();
	estado 	 = new nMaquinaEstados();
	estado_anterior = BUSQUEDA;
}

//------------------------------------------------------------------------------
/**
*/
nIA::~nIA()
{
	delete escritor;
	delete estado;
}

/**
*/
bool nIA::Run(nMemoria *memoria)
{
	switch ( memoria->GetLastMessage() )
	{
		case INITIALIZE:
			if ( !memoria->GameStarted() )
			{
				if ( memoria->GetSecNumber() == 1 )
				{
					escritor->Initialize("Pender","666666","6666FF");
				}
			}
			break;
		case GAME_STARTS:
			estado->SetStatus( BUSQUEDA );
			estado_anterior = BUSQUEDA;
			memoria->Init();
			escritor->Accelerate( 1.0 );
			acelerando = true;
			break;
		case NAME:
			escritor->Initialize("Pender","666666","6666FF");
			escritor->Print("Besame mi brillante y metalico culo!");
			break;
		default:
			break;
	}
	
	if ( memoria->GameStarted() )
	{
		UpdateStatusMachine( memoria );
		
		if ( estado_anterior != estado->GetStatus()  )
		{
			memoria->ResetStateTime( );
		}
		
		// En cada vuelta damos preferencia a la aceleración
		if (memoria->GetRobotSpeed() < memoria->GetRobotMaxSpeed() ) 
		{
			ExecuteSpeedBehavior( memoria );
		}
		else
		{
			ExecuteRobotBehavior( memoria );
			ExecuteRadarBehavior( memoria );
		}
		ExecuteShotBehavior( memoria );
		
		if ( estado_anterior != estado->GetStatus()  )
		{
			estado_anterior = estado->GetStatus() ;
		}
	}
	
	return memoria->GameActive();
}


void nIA::UpdateStatusMachine(nMemoria *memoria)
{
	// Ataque Final
	if ( 	 
		memoria->GetEnemyEnergy() <= 8.0 &&
		memoria->GetRobotEnergy() >= 50.0 &&
		estado->NextStatus( ATAQUE_FINAL )	)
	{
		estado->SetStatus( FINAL );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Ataque Final");	
		}		
		return;
	}
	
	// Ataque de Colision
	/*else if ( 
	    
	    memoria->GetGameTime() > 10.0 && 
		memoria->GetEnemyCollideTime() < 2.0 &&
		estado->NextStatus( ATAQUE_COLISION ) )
	{
		estado->SetStatus( COLISION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("¡Ataque de colision!");	
		}	
		return;
	}*/	
	
	// Ataque normal
	else if ( 	
	   (memoria->GetLastObjective() == ROBOT ||
		memoria->GetLastObjective() == SHOT ) &&
		memoria->GetNearestEnemySightTime() < 2.0 &&
		estado->NextStatus( ATAQUE_KAMIKAZE )	)
	{
		estado->SetStatus( KAMIKAZE );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Ataque");	
		}	
		return;
	}	

	// Ataque preventivo contra minas
	else if ( 
	    //memoria->GetLastObjective() == MINE && 
		//memoria->GetLastObjective() != SHOT &&
		memoria->GetNearestMineSightTime() <= 1.0 &&
		memoria->GetNearestMineDistance() <= 6.0 &&
		!memoria->MineTaken() &&
		estado->NextStatus( ATAQUE_PREVENTIVO )	)
	{
		estado->SetStatus( PREVENTIVO );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Preventivo");	
		}	
		return;
	}
	
    // Persecucion
	else if ( 
	    //memoria->GetLastObjective() == WALL &&
		//memoria->GetNearestCookieSightTime() > 2.0 &&
		//memoria->GetNearestEnemySightTime() > 2.0 &&
		int( memoria->GetNearestEnemySightTime() ) < 5.0 &&
		int( memoria->GetGameTime() ) > 5.0 &&
		//memoria->GetNearestMineSightTime() > 2.0 &&
		estado->NextStatus( PERSEGUIR ) )
	{
		estado->SetStatus( PERSECUCION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Persecucion");
		}
		return;
	}	

	// Búsqueda de objetivo
	else
	{
		estado->SetStatus( BUSQUEDA );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Busqueda");	
		}
		return;		
	}
	
}

void nIA::ExecuteSpeedBehavior(nMemoria *memoria)
{;
	switch ( estado->GetStatus() )
	{
		case BUSQUEDA:	
		case PERSECUCION:
	    case LATERAL:
		case KAMIKAZE:						
		case FINAL:						
		case PREVENTIVO:					
		case COLISION:					
		case CAGUETA:					
			memoria->SetRobotMaxSpeed( 0.2 );
			break;

		default:		
			break;
	}
	// Aceleramos hasta un máximo
	if ( memoria->GetRobotSpeed() < memoria->GetRobotMaxSpeed() )
	{
		escritor->Accelerate( 5.0 );
	}
	else
	{
		escritor->Brake( 1.0 );
	}
}

void nIA::ExecuteShotBehavior(nMemoria *memoria)
{
	double fuerza_disparo;
	switch ( estado->GetStatus() )
	{
	
		case BUSQUEDA:
	    case PERSECUCION:
		case COLISION:
		case KAMIKAZE:
		case LATERAL:
		case CAGUETA:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 7.0 )) && 
				 memoria->GetShotTime() > 0.18 )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
				memoria->ResetShotTime();
			}
			break;

		case FINAL:
			escritor->Shoot( memoria->GetShotMinEnergy() );
			break;

		case PREVENTIVO:
			if ( memoria->GetLastObjective() == MINE )
			{
				escritor->Shoot( memoria->GetShotMinEnergy() );
			}
			break;
			
		default:
			break;
	}
}

void nIA::ExecuteRadarBehavior(nMemoria *memoria)
{
	double angulo_busqueda;

	switch ( estado->GetStatus() )
	{
		case FINAL:
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), memoria->GetRobotRadarMaxRotate() );
			break;	

		case LATERAL:
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetNearestEnemyAngle(), -sgn(memoria->GetNearestEnemyAngle()) * (M_PI/4) );
			break;

		case COLISION:
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), memoria->GetEnemyCollideAngle());
			break;

		case PERSECUCION:
			angulo_busqueda = -sgn(memoria->GetPrevEnemyAngle()) * memoria->GetRobotRadarMaxRotate();
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotRadarMaxRotate(), angulo_busqueda );
			break;
			
		case KAMIKAZE:			
			angulo_busqueda = sgn(memoria->GetPrevEnemyAngle()) * memoria->GetRobotRadarMaxRotate() / 2.0 ;
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotRadarMaxRotate(), angulo_busqueda );
			//angulo_busqueda = memoria->GetPrevEnemyAngle() ;
			//escritor->Sweep( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotRadarMaxRotate(), angulo_busqueda + (M_PI/8), angulo_busqueda - (M_PI/8));
			break;
			
		case BUSQUEDA:		
		case PREVENTIVO:
		case HAMBRIENTO:
		case CAGUETA:
		default:
			angulo_busqueda = memoria->GetRobotCannonMaxRotate();
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), angulo_busqueda );
			break;
	}
	estado_anterior = estado->GetStatus();
}

void nIA::ExecuteRobotBehavior(nMemoria *memoria)
{
	double angulo_giro;

	switch ( estado->GetStatus() )
	{
		case BUSQUEDA:	
		case CAGUETA:
		case LATERAL:
		case PERSECUCION:
		case KAMIKAZE:
		case FINAL:
		case PREVENTIVO:
		default:
			angulo_giro =  memoria->GetRobotMaxRotate() ;
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
			
		case HAMBRIENTO:
			// Rotamos hacia la galleta
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetNearestCookieAngle() );	

			memoria->SetRotationSpeed( memoria->GetRobotMaxRotate() * sgn( memoria->GetNearestCookieAngle() ) );
			break;			
		
		case COLISION:
			// Dejamos de rotar
			//angulo_giro = sgn(memoria->GetNearestShotAngle()) * memoria->GetRobotMaxRotate();	
			
			// Rotamos lejos del disparo
			//escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), -angulo_giro );

			//memoria->SetRotationSpeed( -memoria->GetRobotMaxRotate() * sgn( angulo_giro ) );
			escritor->RotateAmount( ROTAR_ROBOT,  0.0, 0.0 );
			break;
		
		
	}
}
