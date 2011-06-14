#ifndef BOTBATTLE_H
#define BOTBATTLE_H

#include "Robot.h"

#include <stdio.h>
#include <time.h>

class botBattle : public Robot
{
public:
	botBattle();
	~botBattle();

protected:
	void TreatInitialize(int first);
	void TreatYourName(const char *name);
	void TreatYourColour(const char *colour);
	void TreatGameStarts();
	void TreatRotationReached(int what);

	void TreatRadarRobot(double distance, double radarAngle, double energy, int teammate) ;
	void TreatRadarShot(double distance, double radarAngle);
	void TreatRadarWall(double distance, double radarAngle);
	void TreatRadarCookie(double distance, double radarAngle);
	void TreatRadarMine(double distance, double radarAngle);

	void TreatCollisionRobot(double angle);
	void TreatCollisionShot(double angle);
	void TreatCollisionWall(double angle);
	void TreatCollisionCookie(double angle);
	void TreatCollisionMine(double angle);

	// Funciones auxiliares propias
	void SendShotByDistance(double distance);
	void SweepRadarAndCanon();
	void RadarAndCanonToFront();
	void RadarAndCanonToFrontWithDesviation(double distance, double angle);
	void RotateToObject(double angle);
	void SendDelayedShot(double energy);

private:
	bool _sweeping;
	int _spin;
	clock_t _lastShotTime;
};

#endif
