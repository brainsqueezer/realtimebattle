/*
 *  GNU GPL3
 *  Autor: Samuel Jarque Abizanda
 */

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
#define POSITIVO 1
#define NEGATIVO -1

volatile double acceleration = 0.0;
volatile double robot_rotate = 0.0;
volatile double cannon_rotate = 0.0, radar_rotate = 0.0;
double sweep_left, sweep_right, angular_vel, brake = 0.0;
double max_angular_vel = 0.0;
double tid, speed, radar_angle, enemy_angle , tid0, slumprotate = 1.0 ;
double shot_min_energy, maxspeed = 10.0, enemy_time, cannon_sentido; 
double robotmaxrotate;
int robots_left = 20, enemy_radar_count = 0, cannon_angle;
bool sweep = false, align_cro= true, align_cra = true, enemy = false, touch_wall = false, fixed_cannon = false;
int enemy_left_right = 1;
volatile sig_atomic_t exit_robot = false;

//Obtenemos el nombre del tipo de mensaje recibido
message_to_robot_type name2msg_to_robot_type(char* msg_name){

  for(int i=0; message_to_robot[i].msg[0] != '\0'; i++) {
    if( strcmp(message_to_robot[i].msg, msg_name) == 0 ) return (message_to_robot_type)i;
  }
  return UNKNOWN_MESSAGE_TO_ROBOT;
}

// Comprobacion de los mensajes
void check_messages(int sig) {

  exit_robot = false;

  char msg_name[81];
  char text[81];
  message_to_robot_type msg_t;

  cin.clear();
  while( !cin.eof() ) {
    cin >> msg_name;
    msg_t = name2msg_to_robot_type(msg_name);
    switch(msg_t){

      case INITIALIZE: // Primer mensaje que el robot recibe
        int init;
        cin >> init ;
        if (init == 1) {
            cout << "Name Diego es RETARD" << endl;
            cout << "Colour 17d772 3ec00c  " << endl;
        }
        break;

      case GAME_STARTS:  // Mensaje enviado cuando comienza el juego
        // Sweep: 1 robot, 2 cañon, 4 radar
        acceleration = 0.4;
        robot_rotate = 0.4;
        sweep_right = -M_PI/4.0;
        sweep_left = M_PI/4.0;
//        cannon_rotate = 2.0;
        cout << "Accelerate " << acceleration << endl;
//        cout << "Rotate 6 " << cannon_rotate << endl;
        cout << "Rotate 1 " << robot_rotate << endl;
        cout << "Sweep 6 " << max_angular_vel << " " << sweep_right << " " << sweep_left << endl;            
        break;

      case GAME_FINISHES: // El juego actual ha terminado
        break;

      case GAME_OPTION:
        int optnr;
        double value;
        cin >> optnr >> value ;
        switch(optnr){
          case ROBOT_MAX_ROTATE:
            max_angular_vel = value;
            break;
          case SHOT_MIN_ENERGY:
            shot_min_energy = value;
            break;
          default:
            break;
        }
        break;
      
      case INFO: // Mensaje que sigue al mensaje RADAR y que proporciona mas informacion
        cin >> tid >> speed  >> cannon_angle;
        //cout << "Print " << tid << endl;
        // Calculo el sentido de giro del cañon
        if (radar_angle > cannon_angle)
          cannon_sentido = POSITIVO;
        else if (radar_angle < cannon_angle)
          cannon_sentido = NEGATIVO;

        break;
        
      case ROBOTS_LEFT: // Numero de robots restantes
        cin >> robots_left;
        break;

      case RADAR:
        double dist;
        int object;
        cin >> dist >> object >> radar_angle;

        switch(object){
          case ROBOT:
            enemy = true;
            enemy_angle = radar_angle;
            enemy_time = tid;
            if (dist < 0.8)
              cout << "Shoot 50.0 " << endl;
            else if (dist < 2.0)
              cout << "Shoot 30.0 " << endl;
            else if (dist < 5.0 )
              cout << "Shoot 20.0 " << endl;
            else if (dist < 8)
              cout << "Shoot 10.0 " << endl;
            else
              cout << "Shoot 5.0 " << endl;

            sweep_right = - M_PI/10.0;
            sweep_left = M_PI/10.0;
            cout << "Sweep 6 " << max_angular_vel << " " << sweep_right << " " << sweep_left << endl;            
            break;

          case WALL:
        sweep_right = -M_PI/4.0;
        sweep_left = M_PI/4.0;
        cout << "Accelerate " << acceleration << endl;
        cout << "Rotate 1 " << robot_rotate << endl;
        cout << "Sweep 6 " << max_angular_vel << " " << sweep_right << " " << sweep_left << endl;  

            break;
          case SHOT:
            break;
          case COOKIE:
       
            break;
          case MINE:
            break;
        }
        break;
      case COLLISION: // El robot choca o recibe el impacto de algo
        int tmp;
        double coll_angle;
        cin >> tmp >> coll_angle;
        switch(tmp){
          case ROBOT: 
            break;

          case SHOT:

            break;

          case MINE: 
            break;

          case COOKIE: 
            break;

          case WALL:
            touch_wall = true;
            break;

          }
          break;

        case WARNING: //Mensajes de advertencia sobre posibles problemas que ocurran
          cin.getline(text,80,'\n');
          cout << "Print Oh no!! WARNING!!!  " << text << endl;
          break;

        case EXIT_ROBOT: // Hay que salir del programa
          cout << "Print Definitivamente, Diego es RETARD!!!" << endl;
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
