#define N_IMPLEMENTS nMemoria

#include "nmemoria.h"
#include "nescritor.h"
#include "Messagetypes.h"


#define mod(a,b) ( a - floor( a / b ) * b )
#define abs(x) ((x>0) ? (x) : -(x))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define ROBOT_START_ENERGY_DEF			100
#define ROBOT_MAX_ENERGY_DEF			120
#define ROBOT_MAX_ROTATE_SPEED_DEF		0.785398
#define ROBOT_CANNON_MAX_ROTATE_SPEED_DEF	1.5708
#define ROBOT_RADAR_MAX_ROTATE_SPEED_DEF	2.0944
#define SHOT_MIN_ENERGY_DEF			0.5
#define SHOT_MAX_ENERGY_DEF			30
#define ROBOT_MAX_ACCELERATION_DEF		2
#define ROBOT_MIN_ACCELERATION_DEF		-0.5

//------------------------------------------------------------------------------
/**
*/
nMemoria::nMemoria()
{
	escritor = new nEscritor();
	partida_activa			= true;
	partida_comenzada		= false;
}

//------------------------------------------------------------------------------
/**
*/
nMemoria::~nMemoria()
{
	delete escritor;
}

/**
	Inicializa las variables
*/
void nMemoria::Init()
{
	// Inicializamos las variables
	veces_visto_enemigo				= 0;
	angulo_enemigo						= 0.0;
	angulo_enemigo_anterior		= 0.0;
	velocidad_max_robot				= 0.0;
	velocidad_robot						= 0.0;
	distancia_bot							= 10000.0;
	distancia_enemigo					= 10000.0;
	distancia_enemigo_anterior	= 10000.0;
	distancia_galleta						= 10000.0;
	distancia_mina						= 10000.0;
	distancia_pared						= 10000.0;
	distancia_disparo					= 10000.0;

	tiempo_actual							= 0.0;
	tiempo_colision						= 0.0;
	tiempo_avistamiento_enemigo 					= 99.0;
	tiempo_avistamiento_enemigo_anterior 	= 0.0;
	tiempo_avistamiento_bot		= 0.0;
	tiempo_avistamiento_galleta	= 0.0;
	tiempo_avistamiento_pared	= 0.0;
	tiempo_avistamiento_mina	= 0.0;
	tiempo_avistamiento_disparo	= 0.0;
	disparos_recibidos_seguidos = 0;

	galleta_cogida				= false;
	disparo_cogido			= false;
	mina_cogida				= false;
	rotacion_alcanzada		= true;

	ultimo_objeto_visto		= NOOBJECT;
	
	elemento_colision		= NOOBJECT;
	angulo_colision			= 0.0;
	angulo_colision_enemigo			= 0.0;
	tiempo_colision_enemigo			= 0.0;
	energia_bot				= 100.0;
	energia_enemigo			= 100.0;
}

/**
	GAME_STARTS
	Comienza el juego
	
	GAME_FINISHES
	Termina el juego
	
	EXIT_ROBOT
	Sal de programa inmediatamente!!!
	
	DEAD
	Robot died
*/
void nMemoria::Store_Message(int mensaje)
{
	switch (mensaje)
	{
		case GAME_STARTS:
			partida_comenzada = true;
			break;
		
		case GAME_FINISHES:
		case DEAD:
			partida_comenzada = false;
			break;
		case EXIT_ROBOT:		
			partida_activa = false;
			break;
	}
	mensaje_actual = mensaje;
}

/**
	YOUR_NAME
	arg 1: previous name, send new name only if you don't like it

	YOUR_COLOUR
	arg 1: previous colour
*/
void nMemoria::Store_Message(int mensaje, char par_1[256])
{
	switch (mensaje)
	{
		case YOUR_NAME:
			break;
		
		case YOUR_COLOUR:
			break;	
	}
	mensaje_actual = mensaje;
}

/**
	INITIALIZE
	arg 1: if 1 this is the first sequence for the robot, send name and colour!!
	
	ROTATION_REACHED
	arg 1: what has finished rotation
	
	ROBOTS_LEFT
	arg 1: robots left
*/
void nMemoria::Store_Message(int mensaje, int par_1)
{
	switch (mensaje)
	{
		case INITIALIZE:
			numero_secuencia = par_1;
			break;
		
		case ROTATION_REACHED:
			rotacion_alcanzada = true;
			elemento_rotacion = par_1;
			break;
		
		case ROBOTS_LEFT:
			break;
			
	}
	mensaje_actual = mensaje;
}

/**
	GAME_OPTION
	arg 1: OPTION_NR,  arg 2:  value
	
	COLLISION
	arg 1: object_type, arg 2: collision angle
*/
void nMemoria::Store_Message(int mensaje, int par_1, double par_2)
{
	switch (mensaje)
	{
		case GAME_OPTION:
			switch( par_1 )
			{
				case ROBOT_MAX_ROTATE:
					robot_max_rotate = max( par_2, ROBOT_MAX_ROTATE_SPEED_DEF );
					break;
					
				case ROBOT_CANNON_MAX_ROTATE:
					robot_cannon_max_rotate = max( par_2, ROBOT_CANNON_MAX_ROTATE_SPEED_DEF );
					break;
					
				case ROBOT_RADAR_MAX_ROTATE:
					robot_radar_max_rotate = max( par_2, ROBOT_RADAR_MAX_ROTATE_SPEED_DEF );
					break;
				
				case ROBOT_MAX_ACCELERATION:
					robot_max_acceleration = max( par_2, ROBOT_MAX_ACCELERATION_DEF );
					break;
					
				case ROBOT_MIN_ACCELERATION:
					robot_min_acceleration = max( par_2, ROBOT_MIN_ACCELERATION_DEF );
					break;
				
				case ROBOT_START_ENERGY:
					robot_start_energy = max( par_2, ROBOT_START_ENERGY_DEF );
					break;
				
				case ROBOT_MAX_ENERGY:
					robot_max_energy = max( par_2, ROBOT_MAX_ENERGY_DEF );
					break;
				
				case ROBOT_ENERGY_LEVELS:
					robot_energy_levels = par_2;
					break;
				
				case SHOT_SPEED:
					shot_speed = par_2;
					break;
				
				case SHOT_MIN_ENERGY:
					shot_min_energy = par_2;
					break;
				
				case SHOT_MAX_ENERGY:
					shot_max_energy = par_2;
					break;
				
				case SHOT_ENERGY_INCREASE_SPEED:
					shot_energy_increase_speed = par_2;
					break;
				
				case TIMEOUT:
					timeout = par_2;
					break;
				
				case DEBUG_LEVEL:
					debug_level = par_2;
					break;
				
				case SEND_ROBOT_COORDINATES:
					send_robot_coordinates = par_2;
					break;
				
			}		
		break;
		
		case COLLISION:
			switch (par_1)
			{
				par_2 = acos(cos(par_2)) - (M_PI/180.0);
				case ROBOT:
					elemento_colision = par_1;
					angulo_colision = par_2;
					tiempo_colision = tiempo_actual;
					tiempo_colision_enemigo = tiempo_actual;
					angulo_enemigo_anterior = angulo_enemigo;
					angulo_colision_enemigo = angulo_colision;
					angulo_enemigo = par_2; // < 0.0001 ? velocidad_rotacion_actual : par_2;							
					break;

				case SHOT:
					if (elemento_colision == SHOT)
						disparos_recibidos_seguidos++;
					else
						disparos_recibidos_seguidos = 0;
					
					elemento_colision = par_1;
					angulo_colision = par_2;
					tiempo_colision = tiempo_actual;
					disparo_cogido = true;
					//angulo_enemigo_anterior = angulo_enemigo;
					//angulo_enemigo = par_2; // < 0.0001 ? velocidad_rotacion_actual : par_2;
					angulo_colision_enemigo = angulo_colision;
					break;

				case WALL:
					distancia_pared = 0.0;
					angulo_pared = par_2;
					break;

				case COOKIE:
					distancia_galleta = 10000.0;
					angulo_galleta = par_2;
					galleta_cogida = true;
					break;

				case MINE:
					distancia_mina = 10000.0;
					angulo_mina = par_2;
					mina_cogida = true;
					break;

				default:
					break;
			}	
			break;	
	}
	mensaje_actual = mensaje;
}

/**
	RADAR
	arg 1: distance, arg 2: object_type, arg 3: radar_angle
*/
void nMemoria::Store_Message(int mensaje, double par_1, int par_2, double par_3)
{
	elemento_colision = NOOBJECT;
	par_3 = acos(cos(par_3)) - (M_PI/180.0);
	switch (par_2)
	{
		case NOOBJECT:
			//escritor->Print("Radar NOOBJECT: ", par_3);	
			veces_visto_enemigo = 0;
			break;
			
		case ROBOT:
			//escritor->Print("Radar ROBOT: ", par_3);	
			distancia_enemigo = par_1;
			veces_visto_enemigo ++;
			if (veces_visto_enemigo > 9 )
		    {
			   angulo_enemigo_anterior = angulo_enemigo;
			   angulo_enemigo = par_3; // < 0.0001 ? velocidad_rotacion_actual : par_3;
			}
			break;

		case SHOT:
			//escritor->Print("Radar SHOT: ", par_3);
			if ( par_1 < distancia_disparo || GetNearestShotSightTime() > 1.0 )
			{
				distancia_disparo =  par_1;
				angulo_disparo = par_3; // < 0.0001 ? velocidad_rotacion_actual : par_3;
				if (GetNearestEnemySightTime() < 1.0)
				{
					angulo_enemigo_anterior = angulo_enemigo;
					angulo_enemigo = par_3; // < 0.0001 ? velocidad_rotacion_actual : par_3;
					veces_visto_enemigo = 0;
				}
				disparo_cogido = false;
			}
			break;

		case WALL:
			//escritor->Print("Radar WALL: ", par_1);
			if ( abs( GetCannonAngle() ) < M_PI / 180.0 )
				distancia_pared = par_1;
				veces_visto_enemigo = 0;
			break;

		case COOKIE:
			//escritor->Print("Radar COOKIE: ", par_3);
			if ( par_1 < distancia_galleta || GetNearestCookieSightTime() > 0.5 )
			{		
				distancia_galleta = par_1;
				angulo_galleta = velocidad_rotacion_actual;
				galleta_cogida = false;
				veces_visto_enemigo = 0;
			}
			break;

		case MINE:
			//escritor->Print("Radar MINE: ", par_3);
			if ( par_1 < distancia_mina || GetNearestMineSightTime() > 0.5 )
			{		
				distancia_mina = par_1;
				angulo_mina = velocidad_rotacion_actual;
				mina_cogida = false;
				veces_visto_enemigo = 0;
			}
			break;

		default:
			break;
	}
	penultimo_objeto_visto = ultimo_objeto_visto;
	ultimo_objeto_visto = par_2;
	mensaje_actual = mensaje;
}

/**
	INFO
	arg 1: time, arg 2: speed, arg 3: cannon_angle
	
	COORDINATES
	arg 1: x, arg 2: y, arg 3: angle
*/
void nMemoria::Store_Message(int mensaje, double par_1, double par_2, double par_3)
{
	par_3 = acos(cos(par_3)) - (M_PI/180.0);
	switch (mensaje)
	{
		case INFO:
			tiempo_actual = par_1;
			velocidad_robot = par_2;
			angulo_arma = par_3;
			
			switch (ultimo_objeto_visto)
			{
				case ROBOT:
					tiempo_avistamiento_bot = tiempo_actual;
					break;
		
				case SHOT:
					tiempo_avistamiento_disparo = tiempo_actual;
					break;

				case WALL:
					tiempo_avistamiento_pared = tiempo_actual;
					break;

				case COOKIE:
					tiempo_avistamiento_galleta = tiempo_actual;
					break;

				case MINE:
					tiempo_avistamiento_mina = tiempo_actual;
					break;

				default:
					break;
			}
			break;
		
		case COORDINATES:
			break;
				
	}
	mensaje_actual = mensaje;
}

/**
	ROBOT_INFO
	arg 1: Other robots energylevel, arg 2: enemy[0], teammate[1]
*/
void nMemoria::Store_Message(int mensaje, double par_1, int par_2)
{
	if ( par_2 == 0)
	{
		tiempo_avistamiento_enemigo_anterior = tiempo_avistamiento_enemigo;
		tiempo_avistamiento_enemigo = tiempo_avistamiento_bot;
		energia_enemigo = par_1;
	}
	mensaje_actual = mensaje;
}

/**
	ENERGY
	arg 1: energylevel
*/
void nMemoria::Store_Message(int mensaje, double par_1)
{
	energia_bot = par_1;
	mensaje_actual = mensaje;
}

/**
	WARNING
	arg 1: warning_type, arg 2: stringtiempo_estad
	
	@TODO Mostrar los mensajes de alerta
*/
void nMemoria::Store_Message(int mensaje, int par_1, char par_2[256])
{
	switch(par_1)
	{
		case UNKNOWN_MESSAGE:
			escritor->Debug("WARNING - Mensaje Desconocido:");
			escritor->Debug(par_2);
			break;

		case PROCESS_TIME_LOW:
			escritor->Debug("WARNING - Tiempo de proceso lento:");
			escritor->Debug(par_2);
			break;

		case MESSAGE_SENT_IN_ILLEGAL_STATE:
			escritor->Debug("WARNING - Mensaje enviado en estado ilegal:");
			escritor->Debug(par_2);	
			break;

		case OBSOLETE_KEYWORD:
			escritor->Debug("WARNING - Comando obsoleto:");
			escritor->Debug(par_2);			
			break;

		case NAME_NOT_GIVEN:
			escritor->Debug("WARNING - Nombre no ofrecido:");
			escritor->Debug(par_2);
			break;

		case COLOUR_NOT_GIVEN:
			escritor->Debug("WARNING - Color no ofrecido:");
			escritor->Debug(par_2);
			break;

		default:
			break;
	}
	mensaje_actual = mensaje;
}

/**
	Devuelve el tipo de objetivo más cercano
*/
int nMemoria::GetNearestObjective()
{
	double distancia_minima = 10000.0;
	int objetivo_seleccionado = NOOBJECT;


	if ( distancia_pared < distancia_minima && GetNearestWallSightTime() < 1.0 )
	{
		distancia_minima=distancia_pared;
		objetivo_seleccionado = WALL;
	}
	if ( distancia_disparo < distancia_minima && GetNearestShotSightTime() < 1.0 )
	{
		distancia_minima=distancia_disparo;
		objetivo_seleccionado = SHOT;
	}
	if ( distancia_mina < distancia_minima && GetNearestMineSightTime() < 1.0 )
	{
		distancia_minima=distancia_mina;
		objetivo_seleccionado = MINE;
	}		
	if ( distancia_enemigo < distancia_minima && GetNearestEnemySightTime() < 1.0 || ultimo_objeto_visto ==  ROBOT )
	{
		distancia_minima=distancia_enemigo;
		objetivo_seleccionado = ROBOT;
		return objetivo_seleccionado;
	}
	if ( distancia_galleta < distancia_minima && GetNearestCookieSightTime() < 1.0  )
	{
		distancia_minima=distancia_galleta;
		objetivo_seleccionado = COOKIE;
	}	

	return objetivo_seleccionado;
}

double nMemoria::GetPrevEnemyAngle()
{
	//return  -0.2 * angulo_enemigo *  (-0.7 + GetNearestEnemySightTime());
	return -angulo_enemigo;
}

double nMemoria::GetPrevEnemyDistance()
{
	return 2.0 * cos( ( angulo_enemigo - tiempo_avistamiento_enemigo * velocidad_rotacion_actual ) ) * distancia_enemigo  - ( angulo_enemigo_anterior - tiempo_avistamiento_enemigo_anterior * velocidad_rotacion_actual ) * distancia_enemigo_anterior;
}
