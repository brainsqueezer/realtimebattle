/* Robot JA14
	revision 2009 */
 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#ifdef TIME_WITH_SYS_TIME 
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <math.h>
#include "Messagetypes.h"
#include <string.h>

using namespace std;

class nodo {
	public:
    	char mensaje[256];
	nodo* psig;
	nodo();
};
	
//nodo::nodo();
	
class lista {
	public:
	nodo *l;
	lista();
	void insertar(char msj[256]);
	void mostrar();
};

lista::lista()
{
	l=NULL;
}

void lista::insertar(char msj[256])
{
	nodo *aux;
	nodo *nuevo=(nodo*) malloc (sizeof(nodo));
	// insertar primero
	if (l==NULL)
		l=nuevo;
	else {
		aux=l;
		while (aux->psig!=NULL) {
			aux=aux->psig;
			}
		aux->psig=nuevo;
		}
	strcpy(nuevo->mensaje,msj);
	nuevo->psig=NULL;
}
  
void lista::mostrar()
{
	char msjaux[256];
	nodo *aux;

	aux=l;
	while (aux!=NULL) {
		strcpy(msjaux,aux->mensaje);
        l=aux->psig;
        free(aux);
		aux=l;
		cout << msjaux << endl;
		}
}

lista lista_msj;

volatile sig_atomic_t exit_robot = false;
double tid,speed,cannon_angle,robots_left,radar_angle;

int hgiro_radar,hgiro_robot;
int detectado=0,fijado=0;
double time_last_seen, distancia;
int init;
double dist, energy, radar_grados;
int object;
int tmp;
double energy_diff = 0, coll_angle;
double coll_grad;
double cannon_grados,cannon_grados_neg;

message_to_robot_type name2msg_to_robot_type(char* msg_name);
void check_messages(int sig);
double radianes2grados (double radianes);

int main(int argc, char * argv[])
{
	sigset_t usr1set;

  	//check_messages(SIGUSR1);
  	signal(SIGUSR1, check_messages);

  	// libpthread seems to block USR1 sometimes for some reason
  	sigemptyset(&usr1set);
  	sigaddset(&usr1set, SIGUSR1);
  	sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
  

  	cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

  	for(;;sleep(1)) {
      		if( exit_robot ) {
			// cerr << "Fire: Exiting normally!" << endl;
          		return(EXIT_SUCCESS);
        		}
    		}
  	return(EXIT_SUCCESS);
}

message_to_robot_type name2msg_to_robot_type(char* msg_name)
{
	for(int i=0; message_to_robot[i].msg[0] != '\0'; i++) {
      		if( strcmp(message_to_robot[i].msg, msg_name) == 0 )
        		return (message_to_robot_type)i;
   			}
  	return UNKNOWN_MESSAGE_TO_ROBOT;
}

void check_messages(int sig)
{
  	exit_robot = false;

  	char msg_name[81];
  	char text[81];
  	message_to_robot_type msg_t;

  	cin.clear();
  	while( !cin.eof() ) {
      		cin >> msg_name;
      		msg_t = name2msg_to_robot_type(msg_name);
      		switch(msg_t) {
        		case INITIALIZE:	
				cin >> init ;
          			if (init == 1) {
              				cout << "Name FragaBot" << endl;
              				cout << "Colour dede11 de5500" << endl;
            			}
          			break;
        		case GAME_STARTS:
				hgiro_robot=1;
				lista_msj.insertar("Rotate 6 1.6"); 
				lista_msj.mostrar();
          			break;
        		case GAME_FINISHES:
          			break;
			case INFO:
         			cin >> tid >> speed  >> cannon_angle;
				cannon_grados=radianes2grados(cannon_angle);
          			//cout << "Print Info, tid: " << tid << " velocidad " << speed << " angulo cañon " << cannon_grados << endl;
          			break;
          		case ROBOTS_LEFT:
          			cin >> robots_left;
				//cout << "Print Quedan " << robots_left << " robots." << endl;
				lista_msj.mostrar();
          			break;
				case RADAR: {
            				cin >> dist >> object >> radar_angle;
					radar_grados=radianes2grados(radar_angle);
					switch(object) {
		            			case ROBOT: {
			                		time_last_seen=tid;
							distancia=dist;
							detectado=1;
							fijado=1;
							lista_msj.insertar("Rotate 6 0");
							if (dist<1.2) {
								lista_msj.insertar("Shoot 30.0");
							} else {
								if (dist<4.0) {
									lista_msj.insertar("Shoot 20.0");
								} else {
									if (dist<8.0) {
										lista_msj.insertar("Shoot 10.0");
										//lista_msj.insertar("Shoot 10.0");
										}
								}	
							}	
							if (radar_grados<20.0 || radar_grados>340.0) {
								hgiro_robot=0;
								lista_msj.insertar("Rotate 1 0");
								if (speed<1.0) {
									lista_msj.insertar("Brake 0");
									lista_msj.insertar("Accelerate 0.3");
									}
								else {
									lista_msj.insertar("Brake 0");
									lista_msj.insertar("Accelerate 0.1");
									}
								}
							else {
								if (radar_grados>20 && radar_grados<180) {
									hgiro_robot=1;
									lista_msj.insertar("Brake 1");
									lista_msj.insertar("Rotate 1 0.6");
									}
								if (radar_grados>180 && radar_grados<340) {
									hgiro_robot=-1;
									lista_msj.insertar("Brake 1");
									lista_msj.insertar("Rotate 1 -0.6");
									}
								if (speed<0.5) {
									lista_msj.insertar("Brake 0");
									lista_msj.insertar("Accelerate 0.7");
									}
								else {
									lista_msj.insertar("Brake 0");
									lista_msj.insertar("Accelerate 0.1");
									}
								}
							lista_msj.mostrar();
							}
	                			break;
				            	case WALL: {
							if ((tid-time_last_seen)<0.2&&fijado) {
								if (hgiro_robot==1) {
									lista_msj.insertar("Rotate 6 1.6");
									}
								else {
									lista_msj.insertar("Rotate 6 -1.6");
									}
								fijado=0;
								}
							else {
								if ((tid-time_last_seen)>1.0&&detectado) {
									if (hgiro_robot==1) {
										hgiro_robot=-1;
										lista_msj.insertar("Rotate 6 -1.6");
										}
									else {
										hgiro_robot=1;
										lista_msj.insertar("Rotate 6 1.6");
										}
									fijado=0;
									detectado=0;
									}
								}
															
							if (tid-time_last_seen>3.0) {
								hgiro_robot=0;
								lista_msj.insertar("Rotate 1 0");
								}
							if (dist<8.5&&speed>1.5) {
								lista_msj.insertar("Accelerate 0");
								lista_msj.insertar("Brake 1");
								}
							if (speed<1.5) {
								lista_msj.insertar("Brake 0");
								lista_msj.insertar("Accelerate 0.5");
								}
							lista_msj.mostrar();
							}
                 			break;
	              			case SHOT: {
							//cout << "Print Informacion radar: disparo a " << dist << " orientacion " << radar_grados << endl;
							//fijado=0;
							//~ if (hgiro_robot==1) {
								//~ lista_msj.insertar("Rotate 6 0.3");
								//~ }
							//~ else {
								//~ lista_msj.insertar("Rotate 6 -0.3");
								//~ }
							lista_msj.mostrar();
							}
							break;
	              			case COOKIE: {		// Detecto una galleta.
							if (dist>5) {
								lista_msj.insertar("Shoot 5");
								}
							/*
		       					if (hgiro_robot==1) {
								lista_msj.insertar("Rotate 6 1.6");
								}
							else {
								lista_msj.insertar("Rotate 6 -1.6");
								}
							*/
							lista_msj.mostrar();
							}
                				break;
					case MINE: {			// Detecto una mina.
						lista_msj.insertar("Shoot 5");
						if (dist<4) {
							if (hgiro_robot==1) {
								lista_msj.insertar("Rotate 6 1.6");
							} else {
								lista_msj.insertar("Rotate 6 -1.6");
							} 
						}
						lista_msj.mostrar();
					}							
                			break;
              				}
          				break;
          			}
				case COLLISION: {
					cin >> tmp >> coll_angle;
					coll_grad=radianes2grados(coll_angle);
					switch(tmp) {
              			case ROBOT: 
					if (cannon_grados<180.0) {
						cannon_grados_neg=cannon_grados+180;
						if (coll_grad>cannon_grados && coll_grad<cannon_grados_neg)
							lista_msj.insertar("Rotate 6 1.6");
						else
							lista_msj.insertar("Rotate 6 -1.6");
						}
					else {
						cannon_grados_neg=cannon_grados-180;
						if (coll_grad<cannon_grados && coll_grad>cannon_grados_neg)
							lista_msj.insertar("Rotate 6 -1.6");
						else
							lista_msj.insertar("Rotate 6 1.6");
						}							
					fijado=0;
					detectado=0;
					lista_msj.mostrar();
					break;
              			case SHOT:
                 			if (cannon_grados<180.0) {
						cannon_grados_neg=cannon_grados+180;
						if (coll_grad>cannon_grados && coll_grad<cannon_grados_neg)
							lista_msj.insertar("Rotate 6 1.6");
						else
							lista_msj.insertar("Rotate 6 -1.6");
					} else {
						cannon_grados_neg=cannon_grados-180;
						if (coll_grad<cannon_grados && coll_grad>cannon_grados_neg)
							lista_msj.insertar("Rotate 6 -1.6");
						else
							lista_msj.insertar("Rotate 6 1.6");
					}							
					fijado=0;
					detectado=0;
					lista_msj.mostrar();
                			break;
              			case MINE: 
                			//cout << "Print Colision con mina, angulo " << coll_grad << endl; 
					lista_msj.mostrar();
                			break;
              			case COOKIE: 
                			//cout << "Print Colision con galleta, angulo " << coll_grad << endl; 
					lista_msj.mostrar();
                			break;
              			case WALL:
                			//cout << "Print Colision con muro, angulo " << coll_grad << endl; 
					lista_msj.mostrar();
                			break;
              				}
						}
            			break;
        		case WARNING: {
          			cin.getline(text,80,'\n');
          			//cout << "Print Tarjeta amarilla por ->  " << text << endl;
				lista_msj.mostrar();
				}
          			break;
        		case EXIT_ROBOT:
          			cout << "Print Shutting down and leaving" << endl;
          			exit_robot = true;
          			break;
        		default:
          			break;
        		}
    		}
  	signal (sig, check_messages);
}

double radianes2grados (double radianes)
{
	double grados;
	grados=radianes*180.0/3.141592654;
	grados=fmod(grados,360.0);
	if (grados<0)
		grados=grados+360;
	return (grados);
}
