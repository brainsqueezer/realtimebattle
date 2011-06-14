#define N_IMPLEMENTS nLector

#include "nlector.h"
#include "Messagetypes.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using std::cin;
using std::endl;

typedef message_to_robot_type msg;

msg nomAmsg(char* nom)
{
	int i;
	for (i=0; message_to_robot[i].msg[0]!='\0' ;i++)
	{
		if (strcmp(nom,message_to_robot[i].msg)==0) return (msg) i;
	}
}

//------------------------------------------------------------------------------
/**
*/
nLector::nLector()
{
}

//------------------------------------------------------------------------------
/**
*/
nLector::~nLector()
{
}

/**
*/
void nLector::Read(nMemoria *memoria)
{
	cin >> msg_aux;
	mensaje = nomAmsg(msg_aux);
	switch(mensaje)
	{
		case YOUR_NAME:
		case YOUR_COLOUR:
			cin >> msg_aux;
			memoria->Store_Message(mensaje,msg_aux);
			break;

		case INITIALIZE:
		case ROTATION_REACHED:
		case ROBOTS_LEFT:		
			cin >> par_ent_1;
			memoria->Store_Message(mensaje,par_ent_1);
			break;

		case GAME_STARTS:
		case GAME_FINISHES:
		case EXIT_ROBOT:
		case DEAD:		
			memoria->Store_Message(mensaje);
			break;

		case GAME_OPTION:
		case COLLISION:		
			cin >> par_ent_1;
			cin >> par_dou_2;
			memoria->Store_Message(mensaje, par_ent_1, par_dou_2);
			break;

		case RADAR:
			cin >> par_dou_1;
			cin >> par_ent_2;
			cin >> par_dou_3;		
			memoria->Store_Message(mensaje, par_dou_1, par_ent_2, par_dou_3);
			break;

		case INFO:
		case COORDINATES:		
			cin >> par_dou_1;
			cin >> par_dou_2;
			cin >> par_dou_3;		
			memoria->Store_Message(mensaje, par_dou_1, par_dou_2, par_dou_3);
			break;

		case ROBOT_INFO:
			cin >> par_dou_1;
			cin >> par_ent_2;		
			memoria->Store_Message(mensaje, par_dou_1, par_ent_2);
			break;

		case ENERGY:
			cin >> par_dou_1;	
			memoria->Store_Message(mensaje, par_dou_1);		
			break;

		case WARNING:
			cin >> par_ent_1;
			cin >> msg_aux;
			memoria->Store_Message(mensaje, par_ent_1, msg_aux);
			break;

		default:
			break;
	}
	cin.clear();
}

