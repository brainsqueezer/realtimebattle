#ifndef ROBOT_H
#define ROBOT_H

#define ROBOT 0
#define SHOT 1
#define WALL 2
#define COOKIE 3
#define MINE 4
#define LAST_OBJECT_TYPE 5

#define ROTATE_ROBOT 1
#define ROTATE_CANON 2
#define ROTATE_RADAR 4
#define ROTATE_ROBOT_AND_CANON 3
#define ROTATE_ROBOT_AND_RADAR 5
#define ROTATE_CANON_AND_RADAR 6
#define ROTATE_ALL 7

class Robot
{
public:
	Robot();
	virtual ~Robot();
	
	void TreatMessage();

protected:
	virtual void TreatInitialize(int first) = 0;
	virtual void TreatYourName(const char *name) = 0;
	virtual void TreatYourColour(const char *colour) = 0;
	void TreatGameOption(int option, double value);
	virtual void TreatGameStarts() = 0;
	void TreatRadar(double distance, int objType, double radarAngle);
	void TreatCollision(int what, double angle);
	virtual void TreatRotationReached(int what) = 0;

	virtual void TreatRadarRobot(double distance, double radarAngle, double energy, int teammate) = 0;
	virtual void TreatRadarShot(double distance, double radarAngle) = 0;
	virtual void TreatRadarWall(double distance, double radarAngle) = 0;
	virtual void TreatRadarCookie(double distance, double radarAngle) = 0;
	virtual void TreatRadarMine(double distance, double radarAngle) = 0;

	virtual void TreatCollisionRobot(double angle) = 0;
	virtual void TreatCollisionShot(double angle) = 0;
	virtual void TreatCollisionWall(double angle) = 0;
	virtual void TreatCollisionCookie(double angle) = 0;
	virtual void TreatCollisionMine(double angle) = 0;

protected:
	void SendRobotOption(int option, int value);
	void SendName(char *name);
	void SendColour(char *homeColor, char *awayColour);
	void SendRotate(int what, double angVel);
	void SendRotateTo(int what, double angVel, double endAng);
	void SendRotateAmount(int what, double angVel, double endAng);
	void SendSweep(int what, double angVel, double rightAng, double leftAng);
	void SendAccelerate(double value);
	void SendBreak(double portion);
	void SendShot(double energy);
	void SendPrint(char *str);
	void SendDebug(char *str);
	void SendDebugLine(double ang1, double radius1, double ang2, double radius2);
	void SendDebugCircle(double ang, double radius, double circRadius);

	// Funciones auxiliares
	void SendMaxShot();
	void SendMinShot();

	double _energy;
	double _angle, _canonAngle;
	double _x, _y;
	double _speed;

	// Configurations
	double _maxRotate, _maxCanonRotate, _maxRadarRotate;
	double _maxAcel, _minAcel;
	double _startEnergy, _maxEnergy, _energyLevels;
	double _shotSpeed, _shotMinEnergy, _shotMaxEnergy, _shotEnergyIncreaseSpeed;
	double _timeout, _debugLevel;
	double _sendCoordinates;

	// Last robot at radar
	double _lr_radarAngle;
	double _lr_distance;

	//
	int _lastShotTicks;
};

#endif
