#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syscall.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "./Messagetypes.h"
#define PI 3.1415
#define GALLETAS 3
#define MATAR 2
#define BUSCANDOANILLO 1
#define ENVARADO 1
#define MIRANDOACHINA 2
#define DESCOLOCADO 3

volatile sig_atomic_t inicializado=0;
volatile sig_atomic_t gamestarted=0;
volatile sig_atomic_t tipomensaje=0;
volatile sig_atomic_t aquemededico=BUSCANDOANILLO;
volatile sig_atomic_t comoestoy=DESCOLOCADO;
volatile sig_atomic_t contador=1,tiempocolision=0,tiempodisparo=0,estaalado=0;;
volatile int tipoobjeto=666,i=1,tiempogalleta=0,tiempoenemigo=0;
volatile double distancia,angulo,energia,energiadisparo=30,angulodisparoanterior=0;
sigset_t allsignals;

void entradadatos(int sig);

inline void RobotOption(int opcion, int valor);
inline void Colour(int color, int coloralternativo);
inline void Rotate(int querotar, double velocidad);
inline void RotateTo(int querotar, double velocidad, double angulofinal);
inline void RotateAmount(int querotar, double velocidad, double angulorelativo);
inline void Sweep(int querotar, double velocidad, double anguloinicial, double angulofinal);
inline void Accelerate(double aceleracion);
inline void Brake(double frenado);
inline void Shoot(double potencia);

inline void DisparoRobot(double distancia);
inline void ParoDisparo(double angulo, double distancia);
inline void EsquivoMina(double angulo);
inline void APorGalleta(double angulo);
inline void EsquivoPared(double distancia);

inline void liberabloqueos();
inline int analizamensaje(char *buffer);



int main (int argc, char *argv[]){
    sigemptyset(&allsignals);
    sigaddset(&allsignals, SIGUSR1);
    struct sigaction action, oldaction;
    action.sa_handler=entradadatos;
    action.sa_mask=allsignals;
    action.sa_flags=SA_RESTART;
    sigaction (SIGUSR1,&action, NULL);
    siginterrupt(SIGUSR1,0);
    RobotOption(USE_NON_BLOCKING,true);
    RobotOption(SIGNAL,SIGUSR1);
    liberabloqueos();
    while(1){
        sleep(20);
    }
return(0);
}

void entradadatos(int sig){
    int d=1;
    char buffer[128];
    liberabloqueos();
    read(0,buffer,128);
	tipomensaje=analizamensaje(buffer);
	//fprintf(stderr,"%s\n",buffer);fflush(stderr);fsync(fileno(stderr));
        switch(tipomensaje){
	case RADAR:
	    sscanf(buffer,"Radar %lf %i %lf",&distancia,&tipoobjeto,&angulo);
	    switch(tipoobjeto){
	    case ROBOT:
                  DisparoRobot(distancia);
		break;
	    case SHOT:
                 ParoDisparo(angulo,distancia);
		break;
	    case WALL:
                 EsquivoPared(distancia);
		break;
	    case COOKIE:
                 APorGalleta(angulo);
		break;
	    case MINE:
		EsquivoMina(angulo);
	    }
	    break;
	case COLLISION:
	    sscanf(buffer,"Collision %i %lf",&tipoobjeto,&angulo);
	    switch(tipoobjeto){
	    case WALL:
	        if(i-tiempocolision>20){
		    Accelerate(-0.5);
		    Brake(1);
		    RotateAmount(1,666.0,angulo+180);
	        }
	        tiempocolision=i;
		Accelerate(2.0);
		break;
	    case ROBOT:
		RotateAmount(1,666,angulo+15);
                break;
	    case SHOT:
                RotateAmount(1,666,angulo+15);
                break;
	    case COOKIE:
                aquemededico=BUSCANDOANILLO;
                break;
	    case MINE:
                break;
	    }
            break;
	case INITIALIZE:
	    if(!inicializado){
		printf("Print Ash nazg durbatuluuk, ash nazg gimbatul, ash nazg trakatuluuk agh burzum-ishi krimpatul!\n");fflush(stdout);fsync(fileno(stdout));
		printf("Name sauron\n");fflush(stdout);fsync(fileno(stdout));
		printf("Colour 00F0F0 010F0F\n");fflush(stdout);fsync(fileno(stdout));
		inicializado=1;
	    }
	    break;
	case ENERGY:
	    sscanf(buffer,"Energy %lf",&energia);
            energiadisparo=30;
	    if(energia<5){
                energiadisparo=0;
	    }
            break;
	case ROBOT_INFO:
	    break;
	case ROTATION_REACHED:
            Accelerate(2);
	    Sweep(6,45.2,-0.255,0.25); // Si sabia cuanto girar es que busco algo concreto.
	    break;
	case WARNING:
	    break;
	case 666:
            sleep(1);
            break;
    }
    if(tipomensaje==GAME_STARTS){
	while(1){
            i++;
	    switch (aquemededico){
	    case BUSCANDOANILLO:
		Accelerate(2.0);
		Rotate(1,2.0);
		if(!(i%300)){
                    Brake(0.7);
		    RotateAmount(1,666.0,90.0);
		}else if(!(i%150)){
                    Brake(0.7);
		    RotateAmount(1,666.0,-90.0);
		}
		break;
	    case GALLETAS:
		Sweep(6,45.2,-0.75,0.75);
		if (!(time(NULL)-tiempogalleta)) {
		    aquemededico=BUSCANDOANILLO;
		}
		break;
	    case MATAR:
                Sweep(6,45.2,-0.01,0.01);
		break;
	    }
	    liberabloqueos();
	    sleep(1);
	}
    }
}

inline void DisparoRobot(double distancia){
    int disparosdistancia=0;
    Shoot(energiadisparo);
    Shoot(energiadisparo);
    Shoot(energiadisparo);
    if (distancia<6){
	Accelerate(2);
	disparosdistancia=5/distancia;
	while((disparosdistancia--)%10){
	    Shoot(energiadisparo);
	}
    }
    liberabloqueos();
}

inline void ParoDisparo(double angulo,double distancia){
    if ((i-tiempodisparo)>5 &  distancia<20 & angulo-angulodisparoanterior<0.1){
	Shoot(0.5);
    }
    tiempodisparo=i;
    angulodisparoanterior=angulo;
}

inline void EsquivoMina(double angulo){
    Shoot(0.02*energiadisparo);
    if (distancia<20 & ((angulo<90) | (angulo>270)) ){
	Accelerate(-0.5);
    }else{
	Accelerate(2);
    }
    Brake(1);
}

inline void APorGalleta(double angulo){
    aquemededico=GALLETAS;
    //RotateTo(7,666,0.0);
    RotateAmount(1,666,angulo);
    //while(haciendoalgo){
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    Accelerate(2.0);
    tiempogalleta=time(NULL)+3;
    sleep(1);
    //}
}

inline void EsquivoPared(double distancia){
    if (distancia<1){
	Accelerate(-0.5);
	Brake(1);
	RotateAmount(1,666.0,190.0);
    }
}

inline void RobotOption(int opcion, int valor){
    printf("RobotOption %i %i\n",opcion,valor);fflush(stdout);fsync(fileno(stdout));
}

inline void Colour(int color, int coloralternativo){
    printf("Colour %i %i\n",color,coloralternativo);fflush(stdout);fsync(fileno(stdout));
}

inline void Rotate(int querotar, double velocidad){
    printf("Rotate %i %lf\n",querotar,velocidad);fflush(stdout);fsync(fileno(stdout));
}

inline void RotateTo(int querotar, double velocidad, double angulofinal){
    printf("RotateTo %i %lf %lf \n",querotar,velocidad,angulofinal);fflush(stdout);fsync(fileno(stdout));
}

inline void RotateAmount(int querotar, double velocidad, double angulorelativo){
    printf("RotateAmount %i %lf %lf \n",querotar,velocidad,angulorelativo);fflush(stdout);fsync(fileno(stdout));
}

inline void Sweep(int querotar, double velocidad, double anguloinicial, double angulofinal){
    printf("Sweep %i %lf %lf %lf\n",querotar,velocidad,anguloinicial,angulofinal);fflush(stdout);fsync(fileno(stdout));
}

inline void Accelerate(double aceleracion){
    printf("Accelerate %lf\n",aceleracion);fflush(stdout);fsync(fileno(stdout));
}

inline void Brake(double frenado){
    printf("Brake %lf\n",frenado);fflush(stdout);fsync(fileno(stdout));
}

inline void Shoot(double potencia){
    printf("Shoot %lf\n",potencia);fflush(stdout);fsync(fileno(stdout));
}

inline void liberabloqueos(){
    sigrelse(SIGUSR1);
    funlockfile(stdout);
    sigprocmask(SIG_UNBLOCK, &allsignals, NULL);
}

inline int analizamensaje(char *buffer){
    if(strncmp(buffer,"Ra",2)==0){
	return(RADAR);
    }else if(strncmp(buffer,"Col",3)==0){
	return(COLLISION);
    }else if(strncmp(buffer,"Rot",3)==0){
	return(ROTATION_REACHED);
    }else if(strncmp(buffer,"E",1)==0){
	return(ENERGY);
    }else if(strncmp(buffer,"RobotI",6)==0){
	return(ROBOT_INFO);
    }else if(strncmp(buffer,"GameS",5)==0){
	return(GAME_STARTS);
    }else if(strncmp(buffer,"Ini",3)==0){
	return(INITIALIZE);
    }else if(strncmp(buffer,"Inf",3)==0){
	return(INFO);
    }else if(strncmp(buffer,"W",1)==0){
	return(WARNING);
    }else if(strncmp(buffer,"GameO",5)==0){
        return(GAME_OPTION);
    }else if(strncmp(buffer,"Coo",3)==0){
	return(COORDINATES);
    }else if(strncmp(buffer,"Robots",6)==0){
	return(ROBOTS_LEFT);
    }else if(strncmp(buffer,"D",1)==0){
        return(DEAD);
    }else if(strncmp(buffer,"GameF",5)==0){
	return(GAME_FINISHES);
    }else if(strncmp(buffer,"Ex",2)==0){
	return(EXIT_ROBOT);
    }else if(strncmp(buffer,"YourN",5)==0){
	return(YOUR_NAME);
    }else if(strncmp(buffer,"YourC",5)==0){
	return(YOUR_COLOUR);
    }else if(buffer==NULL){
        return(666);
    }
}

