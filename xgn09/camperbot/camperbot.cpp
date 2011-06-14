/*
    Camperbot - a real time battle bot
    Copyright (C) 2009 Severo Javier Castro Garrido

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "realtimebattle/Messagetypes.h"

using namespace std;

//Declaración de variables

double tiempo, velocidad, angulo_C = 0.0; // Variables de Tiempo, Velocidad y Angulo del Cañon --> Para INFO
double distancia, angulo_R = 0.0; // Variables de Distancia y Angulo del Radar --> Para RADAR
double angulo_Col = 0.0; // Angulo de Colision --> Para COLLISION
double energia = 100.0; // Energia del Robot --> Para ENERGY
int objeto; // Robot, disparo, muro, galleta, mina --> Para RADAR
int objeto2; // Robot, disparo, muro, galleta, mina --> Para COLLISION
double ajuste = 1.0; //Ajuste para el angulo de tiro

volatile sig_atomic_t salir = false; //Para salir cuando se acabe el juego


message_to_robot_type name2msg_to_robot_type(char* msg_name){
  for(int i=0; message_to_robot[i].msg[0] != '\0'; i++)
    {
      if(strcmp(message_to_robot[i].msg,msg_name) == 0)
	
        return (message_to_robot_type)i;

    }
  return UNKNOWN_MESSAGE_TO_ROBOT;
}


void check_messages(int sig){

  salir=false;
	
  char msg_name[81];
  char texto[81];
  message_to_robot_type msg_t;

  cin.clear();
  while(!cin.eof()){
     cin >> msg_name;
     msg_t = name2msg_to_robot_type(msg_name);
     
     switch(msg_t){
         
        case INITIALIZE:
	   int inicio;
	   cin >> inicio;
	   cout << "Name Camperbot" << endl;
	   cout << "Colour FF4500 363636" << endl;
	   break;
	
	case GAME_STARTS:
	   cout << "Rotate 6 1.57 " << endl;	
	   break;

        case GAME_FINISHES:
	   cout << "Print Levantamos el campamento!! " << endl;
	   break;
	   
	case INFO:
	   cin >> tiempo >> velocidad >> angulo_C; 
	   break;

	case ROBOTS_LEFT:
	   int bots_vivos;
	   cin >> bots_vivos;
	   break;
	
	case ENERGY:
	   cin >> energia;
	   break;

	case RADAR:
           cin >> distancia >> objeto >> angulo_R;
	   
	   switch(objeto){ 
     		   
	      case NOOBJECT:
			cout << "Rotate 6 1.57 " << endl;
			cout << "Accelerate 0.0 " << endl;
			cout << "Brake 0.3 " << endl;
			break;

	      case ROBOT:
			cout << "Rotate 6 0.0 " << endl;
			if (distancia > 30)
				ajuste = 1.7;
			else{
				if (distancia > 15)
					ajuste = 1.4;
				else
					ajuste = 1.2;
			}
			cout << "RotateAmount 6 1.5 " << (angulo_R * ajuste) << endl;
			cout << "Shoot 30.0 " << endl;
			cout << "Accelerate 0.0 " << endl;
			cout << "Brake 0.3 " << endl;
			break;
	 
	      case SHOOT:
			break;

   	      case WALL:
			cout << "Brake 1.0 " << endl;
			cout << "RotateAmount 1 0.8 3.14 " << endl;
			cout << "Rotate 6 1.57 " << endl;
			break;

	      case COOKIE:
			if (distancia > 10.0){
				cout << "RotateAmount 6 1.57 " << angulo_R << endl;
				cout << "Shoot 0.5 " << endl;
			}
			break; 

	      case MINE:
			if (distancia < 5.0){
				cout << "RotateAmount 6 1.57 " << angulo_R << endl;
				cout << "Shoot 0.5 " << endl;
			}
			break;

	   }
	   break;

	case COLLISION:
	   cin >> objeto2 >> angulo_Col;
	   
	   switch(objeto2){
	   
              case ROBOT:
			cout << "RotateAmount 6 1.57 " << angulo_R * 1.1 << endl;
			cout << "Brake 1.0 " << endl; 
			break;
		 
	      case SHOT:
			cout << "Accelerate 2.0 " << endl;
			cout << "RotateTo 1 0.8 -2.0 " << endl;
			break;

	      case WALL:
			cout << "Brake 1.0 " << endl;
			cout << "Rotate 1 0.80 " << endl;
			break;

	      case MINE:
			cout << "Print Cago'n el camping Gas " << endl;
			cout << "Rotate 1 0.80 " << endl;
			break;

	      case COOKIE:		
			cout << "Print Bocadillo de tortilla " << endl;
			cout << "Rotate 1 0.80 " << endl;
			break;
		 
	   }
	   break;

	case WARNING:
	   cin.getline(texto,80,'\n');
	   cout << "Print Error: " << texto << " ." << endl;
	   break;

	case DEAD:
	   break;
	   
	case EXIT_ROBOT:
	   salir=true;
	   break;

	default:
	   break;
	
        }	   
     }
  signal(sig, check_messages);
}

int main(int argc, char * argv[])
{
  sigset_t usr1set;
  signal(SIGUSR1, check_messages);
  sigemptyset(&usr1set);
  sigaddset(&usr1set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
  

  cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

  for(;;sleep(1))
    {
      if( salir ) 
        {
                return(EXIT_SUCCESS);
        }
    }
  return(EXIT_SUCCESS);
}
