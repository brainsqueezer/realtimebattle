/*
 * Copyright (c) 2010 Ramon Antonio Parada <rap@ramonantonio.net>
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/*
 * Intrucciones compilacion
 *
 * g++ -O2 brainsqueezer.cpp -o brainsqueezer.robot
 * -g--ffast-math -fwhole-program -combine
 */
//mas aceleracion, moverse poco girar rapido
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "realtimebattle/Messagetypes.h"

using namespace std;
double tiempo, velocidad;
double angulo_canhon = 0.0;
double distancia = 0.0;
double angulo_radar = 0.0;
double angulo_colision = 0.0;
double energia_propia = 100.0;
double energia_rival = 100.0;
int object;
double rotate_speed = 2.0;
//int rivales_vivos;

sigset_t usr1set;
volatile sig_atomic_t salir = false;
message_to_robot_type msg_t;
char msg_name[80];
char texto[80];

message_to_robot_type name2msg_to_robot_type(char* msg_name){
	for (int i=0; message_to_robot[i].msg[0] != '\0'; i++) {
		if (strcmp(message_to_robot[i].msg,msg_name) == 0)
			return (message_to_robot_type)i;
	}
	return UNKNOWN_MESSAGE_TO_ROBOT;
}


/*


static const struct Message message_to_robot[25] = 
{
  {"Initialize", 1, {INT,    NONE,   NONE,   NONE}},   // arg: if 1 this is the first sequence for the robot, send name and colour!!
  {"YourName",   1, {STRING, NONE,   NONE,   NONE}},   // arg: previous name, send new name only if you don't like it
  {"YourColour", 1, {HEX,    NONE,   NONE,   NONE}},   // arg: previous colour
  //  {"BinData",    2, {INT,    BINDATA,NONE,   NONE}},
  //  {"AsciiData",  1, {STRING, NONE,   NONE,   NONE}},
  //  {"LoadDataFinished",0, {NONE,NONE, NONE,   NONE}},
  {"GameOption", 2, {INT,    DOUBLE, NONE,   NONE}},   // arg 1: OPTION_NR,  arg 2:  value 
  {"GameStarts", 0, {NONE,   NONE,   NONE,   NONE}},
  {"Radar",      3, {DOUBLE, INT,    DOUBLE, NONE}},   // first arg: distance, second arg: object_type, third arg: radar_angle
  {"Info",       3, {DOUBLE, DOUBLE, DOUBLE, NONE}},   // first arg: time, second arg: speed, third arg: cannon_angle
  {"Coordinates",3, {DOUBLE, DOUBLE, DOUBLE, NONE}},   // first arg: x, second arg: y, third arg: angle
  {"RobotInfo",  2, {DOUBLE, INT,    NONE,   NONE}},   // first arg: Other robots energylevel, second arg: enemy - 0, teammate - 1
  {"RotationReached",1,{INT, NONE,  NONE,   NONE}},    // first arg: what has finished rotation (see Rotate below)
  {"Energy",     1, {DOUBLE, NONE,   NONE,   NONE}},   // arg: energylevel
  {"RobotsLeft", 1, {INT,    NONE,   NONE,   NONE}},   // arg: robots left
  {"Collision",  2, {INT,    DOUBLE, NONE,   NONE}},   // first arg: object_type, second arg: collision angle
  {"Warning",    2, {INT,    STRING, NONE,   NONE}},   // first arg: warning_type, second arg: string
  {"Dead",       0, {NONE,   NONE,   NONE,   NONE}},   // Robot died  
  {"GameFinishes",0,{NONE,   NONE,   NONE,   NONE}},
  //  {"SaveData",   0, {NONE,   NONE,   NONE,   NONE}},

  {"ExitRobot",  0, {NONE,   NONE,   NONE,   NONE}},

  {"",           0, {}}
};

static const struct Message message_from_robot[25] = 
{
  {"RobotOption",  2, {INT, INT}},        // arg 1: OPTION_NR,  arg 2:  value 
  {"Name",         1, {STRING}},             // arg: name
  {"Colour",       2, {HEX, HEX}},           // first arg: home colour, second arg: away colour
  //  {"LoadData",     1, {INT}},                // arg: type of data, 0 - binary, 1 - ascii
  {"Rotate",       2, {INT, DOUBLE}},     // first arg: what to rotate: 1 Robot, 2 cannon, 4 radar and apropriate sums for combinations                                                             // second arg: angular velocity (rad/s)
  {"RotateTo",     3, {INT, DOUBLE, DOUBLE}},     // first and second arg: as in 'Rotate', third arg: angle to move to
  {"RotateAmount", 3, {INT, DOUBLE, DOUBLE}},     // first and second arg: as in 'Rotate', third arg: angle to rotate
  {"Sweep",        4, {INT, DOUBLE, DOUBLE, DOUBLE}}, // first and second arg: as in 'Rotate', but only cannon and radar
                                                      // third arg: left_angle, fourth arg: right_angle  (relative robot)
  {"Accelerate", 1, {DOUBLE}},             // arg: accelerate (m/s^2)  
  {"Brake",        1, {DOUBLE}},             // arg: brake precentage, 0 = no brake, 1 = full brake 
  {"Break",        1, {DOUBLE}},             // Wrong spelling, included for compabillity reasons.
  {"Shoot",        1, {DOUBLE}},             // arg: amount of energy
  {"Print",        1, {STRING}},             // arg: message to print   
  {"Debug",        1, {STRING}},             // arg: debug message
  {"DebugLine",    4, {DOUBLE, DOUBLE, DOUBLE, DOUBLE}}, // arg1,2: startpoint angle, radius (relative robot)
                                                         // arg3,4: endpoint angle, radius (relative robot)
  {"DebugCircle",  3, {DOUBLE, DOUBLE, DOUBLE}},         // arg1,2: centerpoint angle, radius (relative robot)
                                                         // arg3: circle radius
  //  {"BinData",      2, {INT, BINDATA}},    // first arg: number of bytes,  second arg: data
  //  {"AsciiData",    1, {STRING}},          // arg: data
  //  {"SaveDataFinished",0, {}},
  {"",             0, {}}
};
*/

void check_messages(int sig) {

	cin.clear();
	while (!cin.eof()){
		cin >> msg_name;
		msg_t = name2msg_to_robot_type(msg_name);
		
		switch(msg_t) {
			


	case RADAR:
		cin >> distancia >> object >> angulo_radar;
		
		switch (object) { 
					
			case NOOBJECT: //raar.noobject
			cout << "Rotate 1 " << rotate_speed << " " << endl;
			rotate_speed += 0.8;
			//cout << "Brake 2.0 " << endl;
			//cout << "Print Radar.Noobject angulo:" << angulo_radar << endl;
			
			break;
			case WALL: //radar.wall
			//cout << "Print Radar.Wall angulo:" << angulo_radar << endl;
			
			cout << "Rotate 1 20 " << endl;
			//cout << "Accelerate -2.0 " << endl;
			//xornadaslibres
			cout << "Brake 2.0 " << endl;
			break;

			case ROBOT: //radar.robot
			 
			if (distancia > 4) {
			cout << "Shoot 5.0 " << endl;
			cout << "Shoot 5.0 " << endl;
			cout << "Shoot 5.0 " << endl;
			}
			if ((distancia < 4) && (distancia > 2)) {
				cout << "Shoot 10.0 " << endl;
				cout << "Shoot 20.0 " << endl;
				cout << "Shoot 10.0 " << endl;
				cout << "Shoot 20.0 " << endl;
			}
			if (distancia < 2) {
				cout << "Shoot 30.0 " << endl;
				cout << "Shoot 30.0 " << endl;
				cout << "Shoot 30.0 " << endl;
				cout << "Shoot 30.0 " << endl;
				cout << "Shoot 30.0 " << endl;
			}
			
			rotate_speed = 0.0;
			
			
			cout << "RotateAmount 1 " << angulo_radar << endl;
			//cout << "Accelerate 1.0 " << endl;
			cout << "Brake 1.0 " << endl;
			//cout << "Print Radar.Robot angulo:" << angulo_radar << endl;
			break;
	 
			case SHOOT: //radar.shoot
			//cout << "Print Radar.Shoot angulo:" << angulo_radar << endl;
			cout << "Shoot 0.5 " << endl;
				cout << "Shoot 30.0 " << endl;
				cout << "Shoot 30.0 " << endl;
			cout << "Brake 1.0 " << endl;
			rotate_speed = 0.0;
			
			break;


			case COOKIE: //radar.cookie
			//cout << "Print Radar.Cookie angulo:" << angulo_radar << endl;
			cout << "RotateAmount " << (angulo_radar-0.1) << endl;
			cout << "Accelerate 2.0 " << endl;
			rotate_speed = 0.0;
			
			//cout << "Print Radar.Cookie angulo:" << angulo_radar << endl;
			//cout << "Brake 0.0 " << endl;
			  //TODO rotar?
			//}
			break; 

			case MINE: //radar.mine
			cout << "Rotate 1 " << rotate_speed << " " << endl;
			rotate_speed += 0.5;
			cout << "Accelerate -2.0 " << endl;
			break;

		}// switch radar
		break;

	case COLLISION:
		cin >> object >> angulo_colision;
		cout << "Print Status.Warning: collision " << object << " ." << endl;
		
		switch (object) {
		
			case ROBOT: //collision.robot
			cout << "Rotate 1 " << (angulo_colision) << endl;
			cout << "Brake 1.0 " << endl; 
			break;
		 
			case SHOT: //collision.shot
			cout << "Accelerate 2.0 " << endl;
			break;

			case WALL: //collision.wall
			cout << "Brake 2.0 " << endl;
			cout << "Rotate 1 180 " << endl;
			break;

			case MINE: //collision.mine
			cout << "Brake 2.0 " << endl;
			//cout << "Print Collision.Minaaaa " << endl;
			break;

			case COOKIE: //collision.mine
			//cout << "Print Collision.Cookie " << endl;
			break;
		 
		} //collision
		break;
	//case GAME_OPTION:
	//cout << "Print Status.GameOption " << endl;
	//break;
		    
	//case COORDINATES:
	//cout << "Print Status.GameFinishes " << endl;
	//break;
  
	//case ROBOT_INFO:
	//cin >> energia_rival;
	//cout << "Print Status.RobotInfo " << energia_rival << endl;
	//break;
  
	//case ROTATION_REACHED:
	//cout << "Print Status.RotationReached " << endl;
	//break;
	 
	case INITIALIZE:
		int inicio;
		cin >> inicio;
		cout << "Name bigpress.net es nan" << endl;
		cout << "Colour FFAAAA FF3333" << endl;
		cout << "RobotOption " << USE_NON_BLOCKING << " 1 " << endl;
		break;


	case GAME_STARTS:
		cout << "Accelerate 2.0 " << endl;
		break;

	case GAME_FINISHES:
		cout << "Print :) " << endl;
		break;
		
	case INFO:
		cin >> tiempo >> velocidad >> angulo_canhon; 
		break;

	//case ROBOTS_LEFT:
	//	cin >> rivales_vivos;
	//	break;
	
	case ENERGY:
		cin >> energia_propia;
		//cout << "Print Status.Warning: energia_propia" << energia_propia << " ." << endl;
		break;
		
	case WARNING:
		cin.getline(texto,80,'\n');
		cout << "Print Status.Warning: " << texto << " ." << endl;
		break;

	//case DEAD:
	//	cout << "Print Status.Dead: " << texto << " ." << endl;
	//	break;
		
	case EXIT_ROBOT:
	//	cout << "Print Status.ExitRobot: " << texto << " ." << endl;
		salir = true;
		break;

	default:
		break;
	
		}
	}
	signal(sig, check_messages);
}


void check_messages2(int sig) {
  
	sigprocmask(SIG_BLOCK, &usr1set, NULL);
	check_messages(sig);
	sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
}

int main(int argc, char * argv[]) {
	signal(SIGUSR1, check_messages2);
	sigemptyset(&usr1set);
	sigaddset(&usr1set, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &usr1set, NULL);


	cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

	for (;;sleep(1)) {
		if (salir) {
			return EXIT_SUCCESS;
		}
	 }
	return EXIT_SUCCESS;
}
