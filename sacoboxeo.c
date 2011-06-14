#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syscall.h>
#include <signal.h>
#include "/usr/include/realtimebattle/Messagetypes.h"
//#include <asm/system.h>

int inicializado=0;
int gamestart=0;
void prueba();
void entradadatos(int sig);

int main (int argc, char *argv[]){
    sigset_t allsignals;

    sigemptyset(&allsignals);
    sigaddset(&allsignals, SIGUSR1);
    //sigprocmask(SIG_UNBLOCK, &usr1set, NULL);

    sigfillset(&allsignals);
    sigprocmask(SIG_BLOCK, &allsignals, NULL);
    fflush(stdout);

    siginterrupt(SIGUSR1,0);
    struct sigaction action, oldaction;
    action.sa_handler=entradadatos;
    action.sa_mask=allsignals;
    action.sa_flags=SA_RESTART;
    sigaction (SIGUSR1,&action, NULL);
    siginterrupt(SIGUSR1,0);

    //(void) signal(SIGUSR1,entradadatos);
    printf("Name sacoboxeo\n");
    printf("Colour F0F0F0 0F0F0F\n");
    printf("RobotOption %i 1\n",USE_NON_BLOCKING);
    printf("RobotOption %i %i\n",SIGNAL,SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &allsignals, NULL);
    while(1){
        fflush(stdout);
    }
return(0);
}

void entradadatos(int sig){
    char buffer[120];
    fgets(buffer,120,stdin);
    fprintf(stderr,"%s",buffer);
    //sighold(SIGUSR1);
    if (strncmp(buffer,"Warni",5)==0) {
	fprintf(stderr,"                              RadaaaaaR!\n");
    }
    if (strncmp(buffer,"Initi",5)==0) {
	inicializado=1;
        printf("Print Ash nazg durbatuluuk, ash nazg gimbatul, ash nazg trakatuluuk agh burzum-ishi krimpatul!\n");
    }
    if (strncmp(buffer,"GameS",5)==0 ) {
        sigset_t allsignals;
	// sigfillset(&allsignals);
        sigemptyset(&allsignals);
	sigaddset(&allsignals, SIGUSR1);

         //fflush(stdout);
	 while(1) {
	     //fflush(stdout);
            // block_signals();
	     //sighold(SIGUSR1);
             siginterrupt(SIGUSR1,0);
	     sigprocmask(SIG_BLOCK, &allsignals, NULL);
             siginterrupt(SIGUSR1,0);
	     //printf("Rotate 1 666\n");
             //fprintf(stderr,"Testeando que cojones pasa\n");
	     printf("Shoot 2\n");
             printf("Brake 1\n");
	     //if (1){prueba();}
	     //fflush(stdout);
             //sigrelse(SIGUSR1);
	     sigprocmask(SIG_UNBLOCK, &allsignals, NULL);
	 }
    }
}

void prueba(){
    sigset_t allsignals;

    sigemptyset(&allsignals);
    sigaddset(&allsignals, SIGUSR1);
        //sigfillset(&allsignals);
        sigprocmask(SIG_BLOCK, &allsignals, NULL);
	printf("Rotate 1 666\n");
	printf("Shoot 1\n");
             //fflush(stdout);
}