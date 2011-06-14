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
#include <stdio.h>
#include <fstream>
//#include <string.h>
#include <realtimebattle/Messagetypes.h>


using namespace std;

#define abs(x) ((x>0) ? (x) : -(x))

volatile double acceleration = 0.0;
volatile double robot_rotate = 0.0;
volatile double cannon_rotate = 0.0, radar_rotate = 0.0;
double tid, speed, radar_angle, cannon_angle;
double maxspeed = 10.0; 
int robots_left = 20;
bool align_cro= true, align_cra = true, enemy = false;


volatile sig_atomic_t exit_robot = false;
int rivales = 20;
int energia = 100;
int energia_old = 100;
int distancia_rival = 10;

message_to_robot_type
name2msg_to_robot_type(char* msg_name)
{
  for(int i=0; message_to_robot[i].msg[0] != '\0'; i++)
    {
      if( strcmp(message_to_robot[i].msg, msg_name) == 0 )
        return (message_to_robot_type)i;
    }
  return UNKNOWN_MESSAGE_TO_ROBOT;
}


int PotenciaDisparo(float distancia, int energia)
{
	int potencia = 10;

	if (distancia<7 )  {potencia = 15 ;}
	if (distancia<5 )  {potencia = 20 ;}
	if (distancia<3 )  {potencia = 30 ;}
	if (distancia<2 )  {potencia = 30 ;}

	//if (energia<11) {potencia = potencia/2;}
	//if (energia< 5) {potencia = 0;}

	return potencia;
}




void
check_messages(int sig)
{
  exit_robot = false;

  char msg_name[81];
  char text[81];
  message_to_robot_type msg_t;

  //timeval current_time;
  //gettimeofday(&current_time, NULL);
  //srand(current_time.tv_usec);

  cin.clear();
  while( !cin.eof() )
    {
      cin >> msg_name;
      msg_t = name2msg_to_robot_type(msg_name);
      switch(msg_t)
        {

// CASO CONFIG HECHO OK.
        case INITIALIZE:
          int init;
          cin >> init ;
          if (init == 1)
            {
              cout << "Name Cascoporro" << endl;
              cout << "Colour 1111ff 1155ff" << endl;
            }
          break;

// CASO INICIO HECHO OK.
        case GAME_STARTS:
          //          cout << "Rotate 1 " << robot_rotate << endl;
	  cout << "Print Te voy a dar a cascoporro" << endl;
          acceleration = 0.6;
          cout << "Accelerate " << acceleration << endl;
          break;

// CASO FIN HECHO OK.
        case GAME_FINISHES:
          break;

	case ENERGY:
		cin >> energia;
		if ( energia_old - energia > 5 && distancia_rival<15)
		{
			acceleration = 5.0;
			//cout << "Print Me han breao!" << endl;
                	cout << "Accelerate " << acceleration << endl;

		}
		energia_old = energia;

		if ( energia < 15 ) 
		{
			acceleration = 5.0;
			cout << "Print pupita que me las piro!" << endl;
                	cout << "Accelerate " << acceleration << endl;
		}

	break;


// CASO INFO HECHO OK.
        case INFO:
          cin >> tid >> speed  >> cannon_angle;
          
          break;


// CASO RESTANTES HECHO OK.          
        case ROBOTS_LEFT:
          cin >> robots_left;
	  if (rivales>robots_left) {cout << "Print Solo puede quedar uno!" << endl;}
	  rivales = robots_left;
          break;



// CASO HECHO OK. Lo hacemos mas rapido, pero evitando mas las paredes con mas giro al llegar a pared. El resto igual.
        case RADAR:
          {
            double dist, energy;
            int object;

            cin >> dist >> object >> radar_angle;
            switch(object)
              {
              case ROBOT:
		distancia_rival = dist;
                enemy = true;
                
		energy = PotenciaDisparo(dist,energia);
                robot_rotate = M_PI/3.0;
                if (dist<3.0)
                  {
                    acceleration = 5.0;
                  }
                else
                  {
                    if (speed<maxspeed)
                      acceleration = 4.0; // de 2 a ...
                    else
                      acceleration = 3.0;  // de 0 a ...
                  }
                cout << "RotateAmount 1 " << robot_rotate << " " << radar_angle << endl;
		cout << "Shoot " << energy << endl;
                cout << "RotateTo 6 " << -robot_rotate << " " << 0.0 << endl;
                cout << "Shoot " << energy << endl;
                cout << "Accelerate " << acceleration << endl;


                break;
              case WALL:
                {
                  if (enemy)
                    {
                     
                      enemy = false;
                    }
                  if( dist < 2.0 )
                    {
                       if (speed > 0.1)
                        {
                          acceleration = 2.0;
                          robot_rotate = 9.5;
                          cout << "Brake 1" << endl;
                        }
                      else
                        {
                          robot_rotate = 6.0;
                          acceleration = 2.0;
                        }
                    }
                  else
                    {
                      if (dist<5.0)
                        {
                          if (speed > 2.0)
                            {
                              acceleration = 1.0 ;
                              robot_rotate = 5.2 ;  // de 2.0 a 2.2
                             // cout << "Brake 0.7" << endl;
                            }
                          else
                            {
                              acceleration = 3.1;
                              robot_rotate = 1.0;
                            }
                        }
                      else
                        {
                          robot_rotate = 3.3;
                          if (speed<maxspeed)
                            acceleration = 3.0;
                          else
                            acceleration = 2.0;
                        }
                        
                    }
                    cout << "Rotate 1 " << robot_rotate << endl;
                    cout << "Accelerate " << acceleration << endl;
                    
                }
                break;
              case SHOT:
		// si me disparan lento y lejos, con el giro y velocidad lo evito.
                //cout << "Print Leches me disparan!" << endl;
                if (speed < 1.0)
                  {
                    acceleration = 3.0;
                    robot_rotate = 1.0;
                  }
                else
                  {robot_rotate = 3.0;}

		// si lo tengo cerca, disparo if the flies. aunque esto salta demasiado y no se por que.
		// QUITAR ESTE SHOOT SI VEO QUE GASTO DEMASIADO.
		if (dist<2) {/*cout << "Shoot 0.5" << endl;*/}

                cout << "Accelerate " << acceleration << endl;
                cout << "Rotate 1 " << robot_rotate << endl;
                break;

// CASO GALLETA HECHO OK
              case COOKIE:
                robot_rotate = 0.0;
		if (dist>9)  // si esta a tomar por saco, la matamos, aqui no come ni dios.
			{
				cout << "Shoot 0.5" << endl;
				//cout << "Print Aqui no come ni Dios!" << endl;
			}
		else
			{
    			      if (speed<3.0)
    			              acceleration = 3.0;
      			      else
       			              acceleration = 2.0;
          		      cout << "Rotate 1 " << robot_rotate << endl;
           		      cout << "Accelerate " << acceleration << endl;
			}

                break;


// CASO MINA HECHO OK.
              case MINE:
                if (dist < 7) // si la tenemos cerca, la matamos, sino, que se la coma el rival.
                  {
                    acceleration = 2.0;
                    robot_rotate = 2.0;
                    if (speed > 1.0 && dist < 5)		//si estamos muy cerca, chamos el freno madaleno.
                      {cout << "Brake 1" << endl;cout << "Shoot 0.5" << endl;}
			
                    //cout << "Shoot 0.5" << endl;
                    cout << "Accelerate " << acceleration << endl;
                    cout << "Rotate 1 " << robot_rotate << endl;
                  }
                //if (dist < 5){cout << "Print madre, que me la como..." << endl;}
                break;
              }
          break;
          }


//CASO CRASH OK. Excepto con galleta y mina (aqui llegariamos tarde), salimos echando leches de cualquier colision.
        case COLLISION:
          {
            int tmp;
            
	    double coll_angle;
            cin >> tmp >> coll_angle;
            switch(tmp)
              {
              case ROBOT: 
                
                  acceleration = 2.0;
                  robot_rotate = 5.0;
                 
                cout << "Accelerate " << acceleration << endl; 
                cout << "Rotate 1 " << robot_rotate << endl;
                break;
              case SHOT:
		//cout << "Print Me han breao fino fino" << endl; 
                if (abs(coll_angle)<M_PI/4.0)
                  {
                    if(speed<3.0)
                      acceleration = 3.0;
                    else
                      acceleration = 0.7;
                    robot_rotate = 5.0;
                  }
                cout << "Rotate 1 " << robot_rotate << endl;
                cout << "Accelerate " << acceleration << endl; 
                break;
              case MINE: 
                cout << "Print Lacagamos Luis!" << endl; 
                break;
              case COOKIE: 
                cout << "Print ñam!" << endl; 
                break;
              case WALL:
                acceleration = 15.5;
		robot_rotate = 27;
                cout << "Accelerate " << acceleration << endl; 
		cout << "Rotate 1 " << robot_rotate << endl;		// metemos una rotacion para salir chando milks.
                break;
              }
          }
          break;

// CASO GUARNING HECHO OK.
        case WARNING:
          cin.getline(text,80,'\n');
          cout << "Guarnin: " << text << endl;
          break;

// CASO MUERTO HECHO OK.
	case DEAD:
                cout << "Print Muerto soy!  " << endl;
	  break;

// CASO SALIDA HECHO OK.
        case EXIT_ROBOT:
          cout << "Print Me las piro vampiro" << endl;
          exit_robot = true;
          break;
        default:
          break;
        }
    }
  signal (sig, check_messages);
}

int 
main(int argc, char * argv[])
{
  sigset_t usr1set;

  //check_messages(SIGUSR1);
  signal(SIGUSR1, check_messages);

  // libpthread seems to block USR1 sometimes for some reason
  sigemptyset(&usr1set);
  sigaddset(&usr1set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
  

  cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

  for(;;sleep(1))
    {
      if( exit_robot ) 
        {
          //          cerr << "Fire: Exiting normally!" << endl;
          return(EXIT_SUCCESS);
        }
    }
  return(EXIT_SUCCESS);
}
