#include "botBattle.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

#define SPEEDPROPORTION 0.5
// Desviación del barrido en radianes. Antes 0.5
#define SWEEPDESVIATION 1.0
// Desviación en el giro del robot para compensar la inercia
#define ROTATEDESVIATION 0.2

botBattle::botBattle() : Robot()
{
	_sweeping = false;
	_spin = -1;
	_lastShotTime = clock();
}

botBattle::~botBattle()
{
}

void botBattle::TreatInitialize(int first)
{
	SendColour("FF0000", "00FF00");
	SendName("botBattle!");
}

void botBattle::TreatYourName(const char *name)
{
}

void botBattle::TreatYourColour(const char *colour)
{
}

void botBattle::TreatGameStarts()
{
	SendAccelerate(_maxAcel);
	//SendSweep(ROTATE_CANON, _maxCanonRotate, -PI/2, PI/2);
}

void botBattle::TreatRadarRobot(double distance, double radarAngle, double energy, int teammate)
{
	SendAccelerate(_maxAcel/2);
	RotateToObject(radarAngle);
	SendShotByDistance(distance);
	//RadarAndCanonToFront();
	RadarAndCanonToFrontWithDesviation(distance, radarAngle);
}

void botBattle::TreatRadarShot(double distance, double radarAngle)
{
}

void botBattle::TreatRadarWall(double distance, double radarAngle)
{
	if (distance > 10 * _speed * SPEEDPROPORTION)
	{
		SendBreak(0.0);
		SendAccelerate(_maxAcel);
		SendRotate(ROTATE_ROBOT, _maxRotate/2 * _spin);
		SweepRadarAndCanon();
	}
	else if (distance > 5 * _speed * SPEEDPROPORTION)
	{
		SendBreak(0.0);
		SendAccelerate(_maxAcel/2);
		SendRotate(ROTATE_ROBOT, _maxRotate/2 * _spin);
		SweepRadarAndCanon();
	}
	else if (distance > 3 * _speed * SPEEDPROPORTION)
	{
		SendBreak(0.0);
		SendAccelerate(_maxAcel/5);
		SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, PI/4 * _spin);
		SweepRadarAndCanon();
	}
	else //if (distance > 2 * _speed * SPEEDPROPORTION)
	{
		SendBreak(0.5);
		SendAccelerate(_maxAcel/10);
		SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, PI/2 * _spin);
		SweepRadarAndCanon();
	}
	/*else
	{
		SendAccelerate(_maxAcel);
		SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, PI/1.5 * _spin);
	}*/
}

void botBattle::TreatRadarCookie(double distance, double radarAngle)
{
	RotateToObject(radarAngle);
	SendAccelerate(_maxAcel/2); 
	RadarAndCanonToFront();
}

void botBattle::TreatRadarMine(double distance, double radarAngle)
{
	if (distance < 5)
		SendMinShot();
}

void botBattle::TreatCollisionRobot(double angle)
{
	SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, angle); // TODO: girar por lado más corto
}

void botBattle::TreatCollisionShot(double angle)
{
	//SendPrint("Arghh!");
	/*SendAccelerate(_maxAcel);
	SendRotateAmount(ROTATE_ROBOT, _maxRotate, angle);*/
	// TODO: Cambiar el giro al contrario
}

void botBattle::TreatCollisionWall(double angle)
{
	//SendAccelerate(_maxAcel/2);
	SendAccelerate(_minAcel);
	SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, (angle + PI / 1.5) * _spin);
}

void botBattle::TreatCollisionCookie(double angle)
{
}

void botBattle::TreatCollisionMine(double angle)
{
}

void botBattle::TreatRotationReached(int what)
{
	if (what == ROTATE_ROBOT) // Robot
	{
	}
	else if (what == ROTATE_CANON) // Canon
	{
		
	}
	else if (what == ROTATE_RADAR) // Radar
	{
	}
}

// Funciones auxiliares propias
////////////////////////////////////

void botBattle::SendShotByDistance(double distance)
{

	if (distance > 6)
		SendDelayedShot(_shotMaxEnergy / 4);
	else if (distance > 3)
		SendDelayedShot(_shotMaxEnergy / 2);
	else
		SendDelayedShot(_shotMaxEnergy);
}

void botBattle::SweepRadarAndCanon()
{
	if (!_sweeping)
	{
		// Mirar más hacia el lado del interior del giro
		if (_spin == 1) SendSweep(ROTATE_CANON_AND_RADAR, _maxCanonRotate * -_spin, -PI/4, PI/4+SWEEPDESVIATION);
		else SendSweep(ROTATE_CANON_AND_RADAR, _maxCanonRotate * -_spin, -PI/4-SWEEPDESVIATION, PI/4);

		_sweeping = true;
	}
}

// En realidad lo para
void botBattle::RadarAndCanonToFront()
{
	if (_sweeping)
	{
		SendRotateAmount(ROTATE_CANON_AND_RADAR, _maxCanonRotate, 0);
		_sweeping = false;
	}
}

void botBattle::RadarAndCanonToFrontWithDesviation(double distance, double angle)
{
	if (distance > 6)
	{
		SendRotateAmount(ROTATE_RADAR, _maxRadarRotate, 0);
		SendRotateTo(ROTATE_CANON, _maxCanonRotate, angle + 0.2 * _spin);
		_sweeping = false;
	}
	else
		RadarAndCanonToFront();
}

void botBattle::RotateToObject(double angle)
{
	SendRotateAmount(ROTATE_ROBOT, _maxRotate * _spin, angle + ROTATEDESVIATION * _spin);
}

void botBattle::SendDelayedShot(double energy)
{
	clock_t now = clock();
	if ((now - _lastShotTime) > 0.001) //CLOCKS_PER_SEC/1000000)
	{
		SendShot(energy);
		_lastShotTime = clock();
	}
	if (_lastShotTicks > 3) SendShot(energy);
	SendShot(energy);
}

