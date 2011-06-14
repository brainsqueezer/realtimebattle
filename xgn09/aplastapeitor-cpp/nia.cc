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
					escritor->Initialize("Aplastapeitor v5.1","EEEE11","6666CC");
					escritor->Print("¡Que descuaje, macho!");
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
			escritor->Initialize("Aplastapeitor v5.1","EEEE11","6666CC");
			escritor->Print("¡Que descuaje, macho!");
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
		memoria->GetNearestEnemySightTime() > 0.5 &&
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
	else if ( 
	    
	    (memoria->GetGameTime() > 10.0 && 
		 memoria->GetEnemyCollideTime() < 1.0) ||
	    (memoria->GetNearestEnemySightTime() < 0.2 &&
		 estado->GetStatus() == COLISION) &&
		estado->NextStatus( ATAQUE_COLISION ) )
	{
		estado->SetStatus( COLISION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("¡Ataque de colision!");	
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
			escritor->Debug("Ataque");	
		}	
		return;
	}	

	// Ataque preventivo contra minas
	else if ( 
	    //memoria->GetLastObjective() == MINE && 
		//memoria->GetLastObjective() != SHOT &&
		memoria->GetNearestMineSightTime() <= 1.0 &&
		memoria->GetNearestMineDistance() <= 7.0 &&
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
			escritor->Debug("Hambriento");	
		}		
		return;
	}

	// Modo Defensivo
	else if ( 	
	    memoria->GetCollideObject() == SHOT && 
		memoria->GetCollideTime() < 10.0 &&
		memoria->GetNumberShotTaken() > 2 &&
		estado->NextStatus( HUIR )	)
	{
		estado->SetStatus( CAGUETA );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Retirada");	
		}		
		return;
	}
	
    // Persecucion
	else if ( 
	    //memoria->GetLastObjective() == WALL &&
		//memoria->GetNearestCookieSightTime() > 2.0 &&
		//memoria->GetNearestEnemySightTime() > 2.0 &&
		int( memoria->GetNearestEnemySightTime() ) < 0.2 ||
		(memoria->GetTimesEnemySeen() > 0 || 
		 memoria->GetShotTime() < 0.5) &&
		//int( memoria->GetGameTime() ) > 0.2 &&
		//memoria->GetNearestMineSightTime() > 5.0 &&
		estado->NextStatus( PERSEGUIR ) )
	{
		estado->SetStatus( PERSECUCION );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Persecucion");
		}
		return;
	}

    // Ataque Lateral
	else if ( 
	    memoria->GetGameTime() > 10.0 && 
	    memoria->GetNearestEnemySightTime() > 12.0 &&
		memoria->GetReceivedShotTime() > 10.0 &&
		estado->NextStatus( ATAQUE_LATERAL ) )
	{
		estado->SetStatus( LATERAL );
		if ( estado->GetStatus() != estado_anterior)
		{
			escritor->Debug("Ataque lateral");	
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
			if ( memoria->GetNearestWallDistance() < 3.0 )
					memoria->SetRobotMaxSpeed( 0.1 );
					 
			if (memoria->GetRobotSpeed() < 0.5 )
					memoria->SetRobotMaxSpeed( 6.0 + 6.0 * memoria->GetNearestEnemySightTime() );
					
			break;
			
		case PERSECUCION:
	    case LATERAL:
			memoria->SetRobotMaxSpeed( 0.0 );
			break;
			
		case KAMIKAZE:						
			memoria->SetRobotMaxSpeed( 0.0 );
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
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 1.8 )) && 
				 memoria->GetShotTime() > 0.18 )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
				memoria->ResetShotTime();
			}
			break;
				
		case COLISION:
		case KAMIKAZE:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 20.0 )) && 
				 memoria->GetShotTime() > 0.15 )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() / max(5 - memoria->GetTimesEnemySeen(), 1.0) );
				memoria->ResetTimesEnemySeen();
				memoria->ResetShotTime();
			}
			break;

		case LATERAL:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 1.2 )) && 
				 memoria->GetShotTime() > 0.18 )
			{
				escritor->Shoot( memoria->GetShotMaxEnergy() );
				memoria->ResetTimesEnemySeen();
				memoria->ResetShotTime();
			}
			break;			
			
		case CAGUETA:
			if ( memoria->GetTimesEnemySeen() > int( sqrt(memoria->GetNearestEnemyDistance() / 1.8 )) && 
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
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), sgn(memoria->GetRotationSpeed()) *memoria->GetRobotRadarMaxRotate() );
			break;	

		case LATERAL:
			angulo_busqueda = sgn(memoria->GetRotationSpeed())*memoria->GetRobotMaxRotate();
			if (abs(angulo_busqueda) < M_PI/20.0)
			{
				angulo_busqueda = -memoria->GetRobotMaxRotate();
			}
			
			escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), angulo_busqueda + (sgn(angulo_busqueda))*(M_PI/4.0) );
			break;

		case COLISION:
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), memoria->GetEnemyCollideAngle());
			break;

		case PREVENTIVO:	
			angulo_busqueda = memoria->GetNearestMineAngle() / max(10 - memoria->GetNearestMineSightTime()*memoria->GetNearestMineSightTime(),1) ;
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), angulo_busqueda );
			break;
			
		case KAMIKAZE:
			angulo_busqueda = sgn(memoria->GetRotationSpeed()) * memoria->GetRobotCannonMaxRotate() *1.5 / max(10 - memoria->GetNearestEnemySightTime()*memoria->GetNearestEnemySightTime(),1) ;
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), angulo_busqueda );
			break;

		case HAMBRIENTO:			
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), 0.0 );
			break;
		
		case PERSECUCION:
			//angulo_busqueda = -sgn(memoria->GetRotationSpeed()) * memoria->GetRobotCannonMaxRotate() * 2.0 / max(10 - memoria->GetNearestEnemySightTime()*memoria->GetNearestEnemySightTime(),1) ;
			//escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), angulo_busqueda );
			//break;
			
		case BUSQUEDA:			
		case CAGUETA:
		default:
			//escritor->RotateAmount( ROTAR_RADAR + ROTAR_ARMA, memoria->GetRobotCannonMaxRotate(), -memoria->GetCannonAngle() );
			angulo_busqueda = -sgn(memoria->GetPrevEnemyAngle()) * memoria->GetRobotMaxRotate() / max(10 - memoria->GetNearestEnemySightTime()*memoria->GetNearestEnemySightTime(),1) ;
			escritor->RotateTo( ROTAR_RADAR + ROTAR_ARMA, angulo_busqueda, 0.0 );
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
		
		case LATERAL:
		case BUSQUEDA:	
		case CAGUETA:
		/*
			if ( memoria->GetNearestEnemyAngle() != 0.0 && 
				 int( memoria->GetNearestEnemySightTime() ) % 50 < 25) 
			{			
				angulo_giro = -sgn(memoria->GetNearestEnemyAngle())*memoria->GetRobotMaxRotate();
			} 
			else if ( memoria->GetNearestEnemyAngle() != 0.0 && 
					  int( memoria->GetNearestEnemySightTime() ) % 50 >= 25) 
			{			
				angulo_giro = sgn(memoria->GetNearestEnemyAngle())*memoria->GetRobotMaxRotate();
			}
			else
			{
				angulo_giro =  -memoria->GetRobotMaxRotate() ;
			}
			//escritor->DebugLine(0.0,0.0,angulo_giro,5.0);

			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
		*/
		/* Prueba de otra estrategia de búsqueda */
			//if ( memoria->GetNearestEnemySightTime() < 10.0)
			//{
				//escritor->Debug("angulo enemigo: ", sgn(memoria->GetPrevEnemyAngle()));
				angulo_giro = sgn(memoria->GetPrevEnemyAngle())*memoria->GetRobotMaxRotate();
				if (abs(angulo_giro) < M_PI/20.0)
				{
					angulo_giro = memoria->GetRotationSpeed();
				}
				if (abs(angulo_giro) < M_PI/20.0)
				{
					angulo_giro = -memoria->GetRobotMaxRotate();
				}
				//escritor->Debug("Angulo Giro: ", angulo_giro);
			//}
			/*else
			{
				if ( abs(memoria->GetRotationSpeed()) < M_PI/20.0 )
				{			
					angulo_giro = -sgn(memoria->GetRotationSpeed())*memoria->GetRobotMaxRotate();
				} 
				else
				{
					angulo_giro = -memoria->GetRobotMaxRotate() ;
				}
			}*/
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
				
		case PERSECUCION:	
			//escritor->Debug("Visto enemigo hace: ", memoria->GetNearestEnemySightTime());
			//angulo_giro = -sgn(memoria->GetRotationSpeed())*memoria->GetRobotMaxRotate();
			angulo_giro = -sgn(memoria->GetRotationSpeed()) * memoria->GetRobotMaxRotate() / (memoria->GetTimesEnemySeen()*sqrt(memoria->GetNearestEnemyDistance()) + 1);
			
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			//memoria->SetRotationSpeed( angulo_giro );
			break;

		case KAMIKAZE:
			// Rotamos hacia el robot			
			//escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetPrevEnemyAngle());	
            //if (abs(memoria->GetRotationSpeed()) > ( M_PI / 20.0 ))
		    //{
				angulo_giro = sgn(memoria->GetRotationSpeed()) * memoria->GetRobotMaxRotate() / (memoria->GetTimesEnemySeen()*sqrt(memoria->GetNearestEnemyDistance()) + 1);
				escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro);
				memoria->SetRotationSpeed( angulo_giro );
			//}
			//else
		    //{	escritor->Debug("angulo enemigo: ", angulo_enemigo));
			//	angulo_giro = memoria->GetRobotMaxRotate();
			//    escritor->RotateAmount( ROTAR_ROBOT,  0.0, 0.0);
			//    memoria->SetRotationSpeed( memoria->GetRobotMaxRotate() );
		    //}
			//memoria->SetRotationSpeed( memoria->GetPrevEnemyAngle() );
			// memoria->SetRotationSpeed( 0 );
			break;
			
		case FINAL:
			// Rotamos hacia el robot			
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetRobotCannonMaxRotate());	

			memoria->SetRotationSpeed( memoria->GetRobotMaxRotate() );
			break;

		case PREVENTIVO:
			// Rotamos hacia la mina
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetNearestMineAngle() );	

			//memoria->SetRotationSpeed( sgn(memoria->GetNearestMineAngle()) * memoria->GetRobotMaxRotate());
			break;
			
		case HAMBRIENTO:
			// Rotamos hacia la galleta
			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), memoria->GetNearestCookieAngle() );	

			//memoria->SetRotationSpeed( sgn(memoria->GetNearestCookieAngle()) * memoria->GetRobotMaxRotate());
			break;			
		
		case COLISION:
			// Dejamos de rotar
			//angulo_giro = sgn(memoria->GetNearestShotAngle()) * memoria->GetRobotMaxRotate();	
			
			// Rotamos lejos del disparo
			//escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), -angulo_giro );

			//memoria->SetRotationSpeed( -memoria->GetRobotMaxRotate() * sgn( angulo_giro ) );
			escritor->RotateAmount( ROTAR_ROBOT,  0.0, 0.0 );
			memoria->SetRotationSpeed( 0.0 );
			break;
		
		default:
			if ( memoria->GetNearestEnemyAngle() != 0.0 ) 
			{			
				angulo_giro = memoria->GetRobotMaxRotate() * sgn( memoria->GetNearestEnemyAngle() );
			}
			else
			{
				angulo_giro = -memoria->GetRobotMaxRotate() ;
			}

			escritor->RotateAmount( ROTAR_ROBOT,  memoria->GetRobotMaxRotate(), angulo_giro );
			memoria->SetRotationSpeed( angulo_giro );
			break;
	}
}
