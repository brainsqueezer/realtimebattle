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
					escritor->Initialize("Lenthar","66FFFF","FFFF66");
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
			escritor->Initialize("Lenthar","66FFFF","FFFF66");
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
			escritor->Print("Ataque Final");	
		}		
		return;
	}

	// Ataque de Colision
	else if ( 
	    
	    memoria->GetGameTime() > 10.0 && 
		memoria->GetEnemyCollideTime() < 1.0 &&
		estado->NextStatus( ATAQUE_COLISION ) )
	{
		estado->SetStatus( COLISION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("¡Ataque de colision!");	
		}	
		return;
	}	
	
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
			escritor->Print("Ataque");	
		}	
		return;
	}	

	// Ataque preventivo contra minas
	else if ( 
	    memoria->GetLastObjective() == MINE && 
		memoria->GetLastObjective() != SHOT &&
		memoria->GetNearestMineSightTime() <= 2.0 &&
		memoria->GetNearestMineDistance() <= 6.0 &&
		!memoria->MineTaken() &&
		estado->NextStatus( ATAQUE_PREVENTIVO )	)
	{
		estado->SetStatus( PREVENTIVO );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Preventivo");	
		}	
		return;
	}
	
	// Buscar comida
	else if ( 	
		//memoria->GetNearestObjective() == COOKIE && 
		memoria->GetNearestCookieDistance() <= 10.0 &&
		memoria->GetNearestCookieSightTime() < 4.0 &&
		!memoria->CookieTaken() &&
		estado->NextStatus( COGER_GALLETA )	)
	{
		estado->SetStatus( HAMBRIENTO );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Hambriento");	
		}		
		return;
	}

	// Modo Defensivo
	if ( 	
		memoria->GetEnemyEnergy() > 20.0 &&
		memoria->GetRobotEnergy() <= 10.0 &&
		estado->NextStatus( HUIR )	)
	{
		estado->SetStatus( CAGUETA );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Retirada");	
		}		
		return;
	}
	
    // Ataque Lateral
	/*else if ( 
	     memoria->GetNearestShotSightTime() > 5.0 &&
        (int( memoria->GetNearestEnemySightTime() ) + 1 ) % 10 < 2 &&	 
		estado->NextStatus( ATAQUE_LATERAL ) )
	{
		estado->SetStatus( LATERAL );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Ataque lateral");	
		}
		return;
	}*/
	
    // Persecucion
	else if ( 
	    //memoria->GetLastObjective() == WALL &&
		//memoria->GetNearestCookieSightTime() > 2.0 &&
		//memoria->GetNearestEnemySightTime() > 2.0 &&
		int( memoria->GetNearestEnemySightTime() ) < 10.0 &&
		//memoria->GetNearestMineSightTime() > 2.0 &&
		estado->NextStatus( PERSEGUIR ) )
	{
		estado->SetStatus( PERSECUCION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Persecucion");	
		}
		return;
	}		

	// Búsqueda de objetivo
	else
	{
		estado->SetStatus( BUSQUEDA );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Print("Busqueda");	
		}
		return;		
	}
	
}

void nIA::ExecuteSpeedBehavior(nMemoria *memoria)
{;
	switch ( estado->GetStatus() )
	{
		case BUSQUEDA:	
			if ( memoria->GetNearestWallDistance() < 3.0 )
					memoria->SetRobotMaxSpeed( 0.1 );
					
			if (memoria->GetRobotSpeed() < 0.5 )
					memoria->SetRobotMaxSpeed( 6.0 + 6.0 * memoria->GetNearestEnemySightTime() );
					
			break;
			
		case PERSECUCION:
	    case LATERAL:
			memoria->SetRobotMaxSpeed( 5.0 );
			break;
			
		case KAMIKAZE:						
			memoria->SetRobotMaxSpeed( 0.1 );
			break;
			
		case FINAL:						
			memoria->SetRobotMaxSpeed( 0.0 );
			break;

		case PREVENTIVO:					
			memoria->SetRobotMaxSpeed( 0.1 );
			break;
			
		case COLISION:					
			memoria->SetRobotMaxSpeed( 1.0 );
			break;

		case HAMBRIENTO:		
			memoria->SetRobotMaxSpeed( 5.0 );
			break;

		case CAGUETA:					
			memoria->SetRobotMaxSpeed( 5.0 );
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
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance())) )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
			}
			break;
				
		case COLISION:
		case KAMIKAZE:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 2.0)) )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
			}
			break;

	    case LATERAL:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 3.0)) )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
			}
			break;			
			
		case FINAL:
			escritor->Shoot( memoria->GetShotMinEnergy() );
			break;

		case PREVENTIVO:
			if ( memoria->GetNearestMineDistance() <= 5.0 && memoria->GetLastObjective() == MINE )
			{
				escritor->Shoot( memoria->GetShotMinEnergy() );
			}
			break;
			
		case CAGUETA:
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
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), memoria->GetRobotCannonMaxRotate() );
			break;	

		case LATERAL:
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), sgn(memoria->GetNearestEnemyAngle()) * ( M_PI / 2.0 ) );
			break;

		case COLISION:
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), memoria->GetEnemyCollideAngle());
			break;
			
		case BUSQUEDA:
	    case PERSECUCION:			
		case KAMIKAZE:	
		case PREVENTIVO:
		case HAMBRIENTO:
		case CAGUETA:
		default:
			//escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), -memoria->GetCannonAngle() );
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, -sgn(memoria->GetPrevEnemyAngle()) * memoria->GetRobotCannonMaxRotate(), 0.0 );
			/*
			angulo_busqueda = memoria->GetStateTime( ) >10.0 ? ( M_PI / 2.0 ) : ( M_PI / 10.0 )  ;
			escritor->RotateAmount( ROTAR_ROBOT, memoria->GetRobotRadarMaxRotate(), angulo_busqueda );
			*/
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
			if ( memoria->GetNearestEnemyAngle() != 0.0 && 
				 int( memoria->GetNearestEnemySightTime() ) % 20 < 10) 
			{			
				angulo_giro = sgn(memoria->GetNearestEnemyAngle())*memoria->GetRobotMaxRotate();
			} 
			else if ( memoria->GetNearestEnemyAngle() != 0.0 && 
					  int( memoria->GetNearestEnemySightTime() ) % 20 >= 10) 
			{			
				angulo_giro = -sgn(memoria->GetNearestEnemyAngle())*memoria->GetRobotMaxRotate();
			}
			else
			{
				angulo_giro =  memoria->GetRobotMaxRotate() ;
			}

			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
		
		case LATERAL:
		case PERSECUCION:			
			angulo_giro = -sgn(memoria->GetNearestEnemyAngle())*memoria->GetRobotMaxRotate();
			
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate() / (memoria->GetTimesEnemySeen() + 1), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;

		case KAMIKAZE:
			// Rotamos hacia el robot			
			//escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetPrevEnemyAngle());	
            if (memoria->GetNearestEnemyAngle() > ( M_PI / 20.0 ))
		    {
			   escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetPrevEnemyAngle());
			}
			else
		    {
			   escritor->RotateAmount( ROTAR_ROBOT,  0.0, 0.0);
		    }
			//memoria->SetRotationSpeed( memoria->GetPrevEnemyAngle() );
			// memoria->SetRotationSpeed( 0 );
			break;
			
		case FINAL:
			// Rotamos hacia el robot			
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetRobotCannonMaxRotate());	

			memoria->SetRotationSpeed( memoria->GetRobotMaxRotate() );
			// memoria->SetRotationSpeed( 0 );
			break;

		case PREVENTIVO:
			// Rotamos hacia la mina
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetNearestMineAngle() );	

			memoria->SetRotationSpeed( memoria->GetRobotMaxRotate() * sgn( memoria->GetNearestMineAngle() ) );
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
		
		case CAGUETA:
		default:
			if ( memoria->GetNearestEnemyAngle() != 0.0 ) 
			{			
				angulo_giro = memoria->GetRobotMaxRotate() * sgn( memoria->GetNearestEnemyAngle() );
			}
			else
			{
				angulo_giro = memoria->GetRobotMaxRotate() ;
			}

			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
	}
}
