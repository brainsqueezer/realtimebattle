// GPLv2 IaRRoVaWo

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
#include <cstring>

using namespace std;

#define abs(x) ((x>0) ? (x) : -(x))

volatile double acceleration = 0.0;
volatile double robot_rotate = 0.0;
volatile double cannon_rotate = 0.0, radar_rotate = 0.0;
double tid, speed, radar_angle, cannon_angle, tid0, slumprotate = 1.0 ;
double maxspeed = 10.0; 
int robots_left = 20;
bool sweep = false, align_cro= true, align_cra = true, enemy = false;

volatile sig_atomic_t exit_robot = false;

message_to_robot_type name2msg_to_robot_type(char* msg_name) {
  	for(int i=0; message_to_robot[i].msg[0] != '\0'; i++) {
      	if( strcmp(message_to_robot[i].msg, msg_name) == 0 )
        	return (message_to_robot_type)i;
    }
  	return UNKNOWN_MESSAGE_TO_ROBOT;
}

void check_messages(int sig) {
	exit_robot = false;

  	char msg_name[81];
  	char text[81];
  	message_to_robot_type msg_t;

  	cin.clear();
  	while(!cin.eof()) {
  		cin >> msg_name;
  		msg_t = name2msg_to_robot_type(msg_name);
      
      	switch(msg_t) {
      		
        	case INITIALIZE:
          		int init;
          		cin >> init ;
          		if(init == 1) {
              		cout << "Name Enjuto Mojamuto" << endl;
              		cout << "Colour dede11 de5500" << endl;
            	}
          	break;
          	
        	case GAME_STARTS:
        		//cout << "Sweep 6 " << M_PI/3.0 << " " << -M_PI/3.0 << " " << M_PI/3.0 << endl;
        		cout << "Rotate 1 0.5" << endl;
          		cout << "Accelerate 0.5" << endl;
          	break;
          	
        	case INFO:
          		cin >> tid >> speed  >> cannon_angle;
          	break;

        	case RADAR:
            	double dist, energy;
            	int object;

            	cin >> dist >> object >> radar_angle;
            	
            	switch(object) {
              		case ROBOT:
                		enemy = true;
                		
                		robot_rotate = M_PI/3.0;
                		
                		cout << "Rotate 1 " << M_PI/2.0 << endl;
						if(dist<0.5) {
							cout << "Shoot " << 100.0/energy << endl;
							cout << "Shoot 5.0" << endl;
						 } else if(dist<1.0)
							cout << "Shoot 30.0" << endl;
						else if(dist<5.0)
							cout << "Shoot 10.0" << endl;
						else
							cout << "Shoot 5.0" << endl;									
						cout << "Shoot 5.0" << endl;
						cout << "Shoot 5.0" << endl;
						cout << "Shoot 5.0" << endl;
						cout << "Shoot 5.0" << endl;
						cout << "Shoot 5.0" << endl;
						cout << "Rotate 1 " << -M_PI/2.0 << endl;
					break;
					
              		case WALL:
              			if(enemy)
              				cout << "Sweep 6 0 " << -M_PI/5.0 << " " << M_PI/5.0 << endl;
              			if(dist<5.0) {
                      		cout << "Rotate 1 30.0" << endl;
                      		cout << "Accelerate 1.0" << endl;
                      		//cout << "Rotate 1 0.0" << endl;
                      	} else
                      		cout << "Brake 1.0" << endl;               
                	break;
                	
              		case SHOT:
              			if(enemy)
              				cout << "Sweep 6 0 " << -M_PI/5.0 << " " << M_PI/5.0 << endl;
                		cout << "Accelerate 2.0" << endl;
                	break;
                	
              		case COOKIE:
              			if(enemy)
              				cout << "Sweep 6 0 " << -M_PI/5.0 << " " << M_PI/5.0 << endl;
                		if(dist<1.0)
                  			cout << "Accelerate 2.0" << endl;
                		else
                  			cout << "Shoot 0.5" << endl;               
                	break;

              		case MINE:
              			if(enemy)
              				cout << "Sweep 6 0 " << -M_PI/5.0 << " " << M_PI/5.0 << endl;
              			if(dist>10.0)
							cout << "Shoot 0.5" << endl;                	             		
                  	break;
              	}              	
          	break;

        	case COLLISION:
				int tmp;
				double coll_angle;
				cin >> tmp >> coll_angle;
				
				switch(tmp) {
				  	case ROBOT:
				  		cout << "Shoot 5" << endl;
                      	cout << "Accelerate 0.0" << endl;
                      	cout << "Shoot 5" << endl;
                       	cout << "Shoot 5" << endl;
                      	cout << "Accelerate 2.0" << endl;
                      	cout << "Shoot 5" << endl;
                      	cout << "Rotate 6 " << M_PI/2.0 << " " << -M_PI/2.0 << " " << M_PI/2.0 << endl;
					break;
						
				 	case SHOT:
						if (abs(coll_angle)<M_PI/4.0) {
							if(speed<3.0)
						  		acceleration = 2.0;
							else
						  		acceleration = 0.0;
								robot_rotate = 5.0*slumprotate;
					  	}
					  	
						//cout << "Rotate 1 " << robot_rotate << endl;
						cout << "Accelerate " << acceleration << endl;						 
					break;
					
				  	case WALL:
				  		cout << "Rotate 1 " << M_PI/2.0 << " " << -M_PI/2.0 << " " << M_PI/2.0 << endl;
						cout << "Accelerate 0.6" << endl;	
					break;
				}
			break;
			
			case EXIT_ROBOT:
			  	exit_robot = true;
			break;
			
			default:
			break;
			
		}
	  	signal (sig, check_messages);
	}
}

int main(int argc, char * argv[]) {
	sigset_t usr1set;

  	signal(SIGUSR1, check_messages);

  	// libpthread seems to block USR1 sometimes for some reason
  	sigemptyset(&usr1set);
  	sigaddset(&usr1set, SIGUSR1);
  	sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
  
  	cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

  	for(;;sleep(1)) {
  		if( exit_robot )
  			return(EXIT_SUCCESS);							
    }
    
    return(EXIT_SUCCESS);
}
