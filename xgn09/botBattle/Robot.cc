
#include "Robot.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;

Robot::Robot()
{
	_lastShotTicks = 0;
}

Robot::~Robot()
{
}

void Robot::TreatMessage()
{
	string command;
	cin >> command;
	
	if (command == "Initialize")
	{
		int first;
		cin >> first;
		TreatInitialize(first);
	}
	else if (command == "YourName")
	{
		string name;
		cin >> name;
		TreatYourName(name.data());
	}
	else if (command == "YourColour")
	{
		
	}
	else if (command == "GameOption")
	{
		int option;
		double value;
		cin >> option >> value;
		TreatGameOption(option, value);
	}
	else if (command == "GameStarts")
	{
		TreatGameStarts();
	}
	else if (command == "Radar")
	{
		double distance;
		int objType;
		double radarAngle;
		cin >> distance >> objType >> radarAngle;
		TreatRadar(distance, objType, radarAngle);
	}
	else if (command == "Info")
	{
		double time; // ?
		cin >> time >> _speed >> _canonAngle;
	}
	else if (command == "Coordinates")
	{
		cin >> _x >> _y >> _angle;
	}
	else if (command == "RobotInfo")
	{
		double energy;
		int teammate;
		cin >> energy >> teammate;
		TreatRadarRobot(_lr_distance, _lr_radarAngle, energy, teammate); // TODO
	}
	else if (command == "RotationReached")
	{
		int what;
		cin >> what;
		TreatRotationReached(what);
	}
	else if (command == "Energy")
	{
		cin >> _energy;
	}
	else if (command == "RobotsLeft")
	{
		int robots;
		cin >> robots;
		// TODO
	}
	else if (command == "Collision")
	{
		int what;
		double angle;
		cin >> what >> angle;
		TreatCollision(what, angle);
	}
}

void Robot::TreatGameOption(int option, double value)
{
	switch(option)
	{
	case 0: _maxRotate = value; break;
	case 1: _maxCanonRotate = value; break;
	case 2: _maxRadarRotate = value; break;
	case 3: _maxAcel = value; break; 
	case 4: _minAcel = value; break;
	case 5: _startEnergy = value; break; 
	case 6: _maxEnergy = value; break;
	case 7: _energyLevels = value; break;
	case 8: _shotSpeed = value; break; 
	case 9: _shotMinEnergy = value; break; 
	case 10: _shotMaxEnergy = value; break; 
	case 11: _shotEnergyIncreaseSpeed = value; break;
	case 12: _timeout = value; break; 
	case 13: _debugLevel = value; break;
	case 14: _sendCoordinates = value; break;
	}
}

void Robot::TreatRadar(double distance, int objType, double radarAngle)
{
	_lastShotTicks++;

	if (objType == ROBOT) // Robot
	{
		// Esperamos por el robotinfo (Lo bloqueamos un poquito)
		_lr_distance = distance;
		_lr_radarAngle = radarAngle;
		
		/*double energy;
		int teammate;
		cin >> energy >> teammate;
		TreatRadarRobot(distance, radarAngle, energy, teammate);*/
	}

	if (objType == SHOT) // Shot
		TreatRadarShot(distance, radarAngle);

	if (objType == WALL) // Wall
		TreatRadarWall(distance, radarAngle);

	if (objType == COOKIE) // Cookie
		TreatRadarCookie(distance, radarAngle);

	if (objType == MINE) // Mine
		TreatRadarMine(distance, radarAngle);

	if (objType == LAST_OBJECT_TYPE) // Last object type?
		SendDebug("Fuck! what's the las object type?");
}

void Robot::TreatCollision(int what, double angle)
{
	if (what == ROBOT) // Robot
		TreatCollisionRobot(angle);

	if (what == SHOT) // Shot
		TreatCollisionShot(angle);

	if (what == WALL) // Wall
		TreatCollisionWall(angle);

	if (what == COOKIE) // Cookie
		TreatCollisionCookie(angle);

	if (what == MINE) // Mine
		TreatCollisionMine(angle);

	if (what == LAST_OBJECT_TYPE) // Last object type?
		SendDebug("Fuck! what's the las object type?");
}

void Robot::SendRobotOption(int option, int value)
{
	cout << "RobotOption " << option << " " << value << endl;
}

void Robot::SendName(char *name)
{
	cout << "Name " << name << endl;
}

void Robot::SendColour(char *homeColor, char *awayColour)
{
	cout << "Colour " << homeColor << " " << awayColour << endl;
}

void Robot::SendRotate(int what, double angVel)
{
	cout << "Rotate " << what << " " << angVel << endl;
}

void Robot::SendRotateTo(int what, double angVel, double endAng)
{
	cout << "RotateTo " << what << " " << angVel << " " << endAng << endl;
}

void Robot::SendRotateAmount(int what, double angVel, double endAng)
{
	cout << "RotateAmount " << what << " " << angVel << " " << endAng << endl;
}

void Robot::SendSweep(int what, double angVel, double rightAng, double leftAng)
{
	cout << "Sweep " << what << " " << angVel << " " << rightAng << " " << leftAng << endl;
}

void Robot::SendAccelerate(double value)
{
	cout << "Accelerate " << value << endl;
}

void Robot::SendBreak(double portion)
{
	cout << "Break " << portion << endl;
}

void Robot::SendShot(double energy)
{
	_lastShotTicks = 0;
	if (_energy > 1)
		cout << "Shoot " << energy << endl;
}

void Robot::SendPrint(char *str)
{
	cout << "Print " << str << endl;
}

void Robot::SendDebug(char *str)
{
	cout << "Debug " << str << endl;
}

void Robot::SendDebugLine(double ang1, double radius1, double ang2, double radius2)
{
	cout << "DebugLine " << ang1 << " " << radius1 << " " << ang2 << " " << radius2 << endl;
}

void Robot::SendDebugCircle(double ang, double radius, double circRadius)
{
	cout << "DebugCircle " << ang << " " << radius << " " << circRadius << endl;
}

// Funciones auxiiares
//////////////////////////////

void Robot::SendMaxShot()
{
	SendShot(_shotMaxEnergy);
}

void Robot::SendMinShot()
{
	SendShot(_shotMinEnergy);
}