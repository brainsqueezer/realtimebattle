#ifndef N_MEMORIA_H
#define N_MEMORIA_H
//------------------------------------------------------------------------------
/**
    @class nMemoria

    @brief Contiene la memoria de estados del bot

*/

#include "nescritor.h"
#include <math.h>

class nMemoria
{
public:
	/// constructor
	nMemoria();
	/// destructor
	virtual ~nMemoria();
	/// Inicialización de variables
	void Init();

	/// Almacenamiento de instrucciones (sobrecargada)
	void Store_Message(int mensaje);
	void Store_Message(int mensaje, char par_1[256]);
	void Store_Message(int mensaje, int par_1);
	void Store_Message(int mensaje, int par_1, double par_2);
	void Store_Message(int mensaje, double par_1, int par_2, double par_3);
	void Store_Message(int mensaje, double par_1, double par_2, double par_3);
	void Store_Message(int mensaje, double par_1, int par_2);
	void Store_Message(int mensaje, double par_1);
	void Store_Message(int mensaje, int par_1, char par_2[256]);

	/// Devuelve el estado de la partida (si está activa o no)
	inline bool GameActive();

	/// Devuelve si la partida ha comenzado
	inline bool GameStarted();

	/// Devuelve el último mensaje recibido
	inline int GetLastMessage();

	/// Devuelve el ángulo del enemigo más cercano
	inline double GetNearestEnemyAngle();

	/// Devuelve el ángulo anterior del enemigo más cercano
	inline double GetNearestEnemyPrevAngle();

	/// Devuelve el ángulo de la gallega más cercana
	inline double GetNearestCookieAngle();

	/// Devuelve el ángulo de la pared más cercana
	inline double GetNearestWallAngle();

	/// Devuelve el ángulo de la mina más cercana
	inline double GetNearestMineAngle();

	/// Devuelve el ángulo del disparo más cercano
	inline double GetNearestShotAngle();

	/// Devuelve la distancia del enemigo más cercano
	inline double GetNearestEnemyDistance();

	/// Devuelve la distancia del enemigo más cercano
	inline double GetNearestEnemyPrevDistance();

	/// Devuelve la distancia de la gallega más cercana
	inline double GetNearestCookieDistance();

	/// Devuelve la distancia de la pared más cercana
	inline double GetNearestWallDistance();

	/// Devuelve la distancia de la mina más cercana
	inline double GetNearestMineDistance();

	/// Devuelve la distancia del disparo más cercano
	inline double GetNearestShotDistance();

	/// Devuelve el tiempo de avistamiento del enemigo más cercano
	inline double GetNearestEnemySightTime();

	/// Devuelve el tiempo de avistamiento anterior del enemigo más cercano
	inline double GetNearestEnemyPrevSightTime();

	/// Devuelve el tiempo de avistamiento de la gallega más cercana
	inline double GetNearestCookieSightTime();

	/// Devuelve el tiempo de juego
	inline double GetGameTime();

	/// Devuelve el tiempo de avistamiento de la pared más cercana
	inline double GetNearestWallSightTime();

	/// Devuelve el tiempo de avistamiento de la mina más cercana
	inline double GetNearestMineSightTime();

	/// Devuelve el tiempo de avistamiento del disparo más cercano
	inline double GetNearestShotSightTime();

	/// Devuelve el tipo de objetivo más cercano
	int GetNearestObjective();

	/// Devuelve la energia del robot
	inline double GetRobotEnergy();

	/// Devuelve la energia del enemigo
	inline double GetEnemyEnergy();

	/// Devuelve la velocidad del bot
	inline double GetRobotSpeed();

	/// Devuelve el ángulo del arma del robot
	inline double GetCannonAngle();

	/// Devuelve la aceleracion máxima del robot
	inline double GetRobotMaxAcceleration();

	/// Devuelve la aceleracion mínima del robot
	inline double GetRobotMinAcceleration();

	/// Devuelve la velocidad de rotación máxima del cañon del robot
	inline double GetRobotCannonMaxRotate();

	/// Devuelve la velocidad de rotación máxima del radar del robot
	inline double GetRobotRadarMaxRotate();

	/// Devuelve la energía máxima de disparo
	inline double GetShotMaxEnergy();

	/// Devuelve la energía mínima de disparo
	inline double GetShotMinEnergy();

	/// Devuelve la velocidad de rotación máxima del robot
	inline double GetRobotMaxRotate();

	/// Devuelve la velocidad máxima del robot
	inline double GetRobotMaxSpeed();

	/// Indica si una mina se ha cogido recientemente
	inline bool MineTaken();

	/// Indica si una galleta se ha cogido recientemente
	inline bool CookieTaken();

	/// Indica si un disparo se ha cogido recientemente
	inline bool ShotTaken();
	
	/// Indica el numero de disparos recibidos seguidos
	inline int GetNumberShotTaken();	

	/// Indica si la rotación se ha alcanzado
	inline bool GetRotationReached();

	/// Indica que elemento ha alcanzado el angulo
	inline int GetRotationReachedObject();

	/// Modifica el marcador de rotación alcanzada
	inline void SetRotationReached( bool par_1 );

	/// Devuelve el último objetivo visto
	inline int GetLastObjective();

	/// Devuelve el penúltimo objetivo visto
	inline int GetPreviousObjective();

	/// Modifica la velocidad máxima del robot
	inline void SetRobotMaxSpeed( double par_1 );

	/// Devuelve el elemento de colisión
	inline int GetCollideObject();

	/// Devuelve el ángulo de colisión
	inline double GetCollideAngle();

	/// Devuelve el ángulo de colisión con un enemigo
	inline double GetEnemyCollideAngle();

	/// Devuelve el tiempo de la última colisión
	inline double GetCollideTime();

	/// Devuelve el tiempo de la última colisión con un enemigo
	inline double GetEnemyCollideTime();

	/// Devuelve el número de secuencia
	inline int GetSecNumber();

	/// Devuelve el ángulo previsto donde se encontrará el enemigo
	double GetPrevEnemyAngle();

	/// Devuelve la distancia prevista donde se encontrará el enemigo
	double GetPrevEnemyDistance();

	/// Modifica la velocidad de rotación del robot
	inline void SetRotationSpeed( double par_1 );

	/// Devuelve la velocidad de rotación del robot
	inline double GetRotationSpeed();

	/// Modifica la velocidad de rotación del robot
	inline void SetRotationAngle( double par_1 );

	/// Devuelve la velocidad de rotación del robot
	inline double GetRotationAngle();
	
	/// Pone a cero el tiempo de estado
	inline void ResetStateTime();

	/// Devuelve el tiempo de estado
	inline double GetStateTime();	

	/// Pone a cero el tiempo de disparo
	inline void ResetShotTime();

	/// Devuelve el tiempo de disparo
	inline double GetShotTime();

	/// Devuelve el tiempo de la ultima vez que nos dispararon
	inline double GetReceivedShotTime();	
	
	/// Devuelve el número de veces que se ha visto el enemigo seguidas
	inline int GetTimesEnemySeen();
	
	/// Pone a 0 el numero de veces visto seguidas al enemigo
	inline void ResetTimesEnemySeen();	

private:
	bool partida_activa, partida_comenzada;
	int mensaje_actual;

	/// Información del Entorno
	int ultimo_objeto_visto ,penultimo_objeto_visto, elemento_colision, numero_secuencia, elemento_rotacion;
	int veces_visto_enemigo,disparos_recibidos_seguidos;
	
	double energia_bot, energia_enemigo, energia_amigo;
	double angulo_enemigo, angulo_galleta, angulo_pared, angulo_mina, angulo_disparo, angulo_amigo, angulo_bot, angulo_colision, angulo_colision_enemigo;
	double angulo_enemigo_anterior, velocidad_rotacion_actual, angulo_rotacion_actual;
	double distancia_enemigo, distancia_galleta, distancia_pared, distancia_mina, distancia_disparo, distancia_amigo, distancia_bot;
	double distancia_enemigo_anterior;
	double tiempo_avistamiento_enemigo, tiempo_avistamiento_enemigo_anterior, tiempo_avistamiento_bot, tiempo_avistamiento_galleta, tiempo_avistamiento_pared, tiempo_avistamiento_mina, tiempo_avistamiento_disparo;
	double tiempo_actual, tiempo_colision, tiempo_colision_enemigo, velocidad_robot, angulo_arma, velocidad_max_robot;
	double tiempo_estado, tiempo_disparo, tiempo_disparo_recibido;

	bool galleta_cogida, disparo_cogido, mina_cogida, rotacion_alcanzada;
	
	/// Información de la Partida
	double robot_max_rotate, robot_cannon_max_rotate, robot_radar_max_rotate;
	double robot_max_acceleration, robot_min_acceleration;
	double robot_start_energy, robot_max_energy, robot_energy_levels;
	double shot_speed, shot_min_energy, shot_max_energy, shot_energy_increase_speed;
	double timeout, debug_level, send_robot_coordinates;

	/// Para escribir las alertas
	nEscritor* escritor;
};

inline bool nMemoria::GameActive()
{
	return partida_activa;
}

inline bool nMemoria::GameStarted()
{
	return partida_comenzada;
}

inline int nMemoria::GetLastMessage()
{
	return mensaje_actual;
}

inline double nMemoria::GetNearestEnemyAngle()
{
	return angulo_enemigo;
}

inline double nMemoria::GetNearestEnemyPrevAngle()
{
	return angulo_enemigo_anterior;
}

inline double nMemoria::GetNearestCookieAngle()
{
	return -0.1 * angulo_galleta *  (-0.2 + GetNearestCookieSightTime());
}

inline double nMemoria::GetNearestWallAngle()
{
	return asin( sin( angulo_pared ) );
}

inline double nMemoria::GetNearestMineAngle()
{
	return -0.1 * angulo_mina *  (-0.2 + GetNearestMineSightTime());
}

inline double nMemoria::GetNearestShotAngle()
{
	return asin( sin( angulo_disparo ) );
}

inline double nMemoria::GetNearestEnemyDistance()
{
	return distancia_enemigo;
}

inline double nMemoria::GetNearestEnemyPrevDistance()
{
	return distancia_enemigo_anterior;
}

inline double nMemoria::GetNearestCookieDistance()
{
	return distancia_galleta;
}

inline double nMemoria::GetNearestWallDistance()
{
	return distancia_pared;
}

inline double nMemoria::GetNearestMineDistance()
{
	return distancia_mina;
}

inline double nMemoria::GetNearestShotDistance()
{
	return distancia_disparo;
}

inline double nMemoria::GetNearestEnemySightTime()
{
	return tiempo_actual - tiempo_avistamiento_enemigo;
}

inline double nMemoria::GetNearestEnemyPrevSightTime()
{
	return tiempo_actual - tiempo_avistamiento_enemigo_anterior;
}

inline double nMemoria::GetNearestCookieSightTime()
{
	return tiempo_actual - tiempo_avistamiento_galleta;
}

inline double nMemoria::GetGameTime()
{
	return tiempo_actual;
}

inline double nMemoria::GetNearestWallSightTime()
{
	return tiempo_actual - tiempo_avistamiento_pared;
}

inline double nMemoria::GetNearestMineSightTime()
{
	return tiempo_actual - tiempo_avistamiento_mina;
}

inline double nMemoria::GetNearestShotSightTime()
{
	return tiempo_actual - tiempo_avistamiento_disparo;
}

inline double nMemoria::GetRobotEnergy()
{
	return energia_bot;
}

inline double nMemoria::GetEnemyEnergy()
{
	return energia_enemigo;
}

inline double nMemoria::GetRobotMaxAcceleration()
{
	return robot_max_acceleration;
}

inline double nMemoria::GetRobotMinAcceleration()
{
	return robot_min_acceleration;
}

inline double nMemoria::GetRobotCannonMaxRotate()
{
	return robot_cannon_max_rotate;
}

inline double nMemoria::GetRobotRadarMaxRotate()
{
	return robot_radar_max_rotate;
}

inline double nMemoria::GetRobotMaxRotate()
{
	return robot_max_rotate;
}

inline double nMemoria::GetShotMaxEnergy()
{
	return shot_max_energy;
}

inline double nMemoria::GetRobotSpeed()
{
	return velocidad_robot;
}

inline double nMemoria::GetCannonAngle()
{
	return asin( sin( angulo_arma ) );
}

inline double nMemoria::GetShotMinEnergy()
{
	return shot_min_energy;
}

inline double nMemoria::GetRobotMaxSpeed()
{
	return velocidad_max_robot;
}

inline bool nMemoria::MineTaken()
{
	return mina_cogida;
}

inline bool nMemoria::CookieTaken()
{
	return galleta_cogida;
}

inline bool nMemoria::ShotTaken()
{
	return disparo_cogido;
}

inline int nMemoria::GetNumberShotTaken()
{
	return disparos_recibidos_seguidos;
}

inline bool nMemoria::GetRotationReached()
{
	return rotacion_alcanzada;
}

inline int nMemoria::GetRotationReachedObject()
{
	return elemento_rotacion;
}

inline void nMemoria::SetRotationReached( bool par_1 )
{
	rotacion_alcanzada = par_1;
}

inline void nMemoria::SetRobotMaxSpeed( double par_1 )
{
	if ( par_1 < 2.0 )
		velocidad_max_robot = 2.0;		
	else if (par_1 > 8.0 )
		velocidad_max_robot = 8.0;
	else
		velocidad_max_robot = par_1;
}

inline int nMemoria::GetLastObjective()
{
	return ultimo_objeto_visto;
}

inline int nMemoria::GetPreviousObjective()
{
	return penultimo_objeto_visto;
}

inline int nMemoria::GetCollideObject()
{
	return elemento_colision;
}

inline double nMemoria::GetCollideAngle()
{
	return angulo_colision;
}

inline double nMemoria::GetCollideTime()
{
	return tiempo_actual - tiempo_colision;
}

inline double nMemoria::GetEnemyCollideAngle()
{
	return angulo_colision_enemigo;
}

inline double nMemoria::GetEnemyCollideTime()
{
	return tiempo_actual - tiempo_colision_enemigo;
}

inline int nMemoria::GetSecNumber()
{
	return numero_secuencia;
}

inline void nMemoria::SetRotationSpeed( double par_1 )
{
	velocidad_rotacion_actual = par_1;
}

inline double nMemoria::GetRotationSpeed()
{
	return velocidad_rotacion_actual;
}

inline void nMemoria::SetRotationAngle( double par_1 )
{
	angulo_rotacion_actual = par_1;
}

inline double nMemoria::GetRotationAngle()
{
	return angulo_rotacion_actual;
}

inline void nMemoria::ResetStateTime( )
{
	tiempo_estado = tiempo_actual;
}

inline double nMemoria::GetStateTime()
{
	return tiempo_actual - tiempo_estado;
}

inline void nMemoria::ResetShotTime( )
{
	tiempo_disparo = tiempo_actual;
}

inline double nMemoria::GetShotTime()
{
	return tiempo_actual - tiempo_disparo;
}

inline double nMemoria::GetReceivedShotTime()
{
	return tiempo_actual - tiempo_disparo_recibido;
}

inline int nMemoria::GetTimesEnemySeen()
{
	return veces_visto_enemigo;
}

inline void nMemoria::ResetTimesEnemySeen()
{
	veces_visto_enemigo = 0;
}

#endif
