/*
    pacopaco - a real time battle bot
    Copyright (C) 2009 Orlando García Feal.

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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

enum state{
  LOOK_FOR = 0,
  FOUND_ROTATING = 1,
  FOUND_FIRE = 2,
  TURN_BACK = 3, //SUX!
  WIDE_SEARCH = 4 
};

using namespace std;

int main(int argc, char *argv[]){
  cout << "RobotOption " << 3 << " " << 0 << endl;
  cout << "RobotOption " << 1 << " " << 1 << endl;
  cout << "Name pacopaco" << endl;
  cout << "Colour FF0000" << endl;


  state cstate=LOOK_FOR;
  char temp[300],tempb[300],command[20];
  int wtimeout=10,timeout=10,rTout=20;float angle=0;
  int waych=2000;
  int direc=1;
  
  //Lemos todo o que haxa!!
  sleep(1);
  while(1){
    if(fgets(temp,300,stdin)!=NULL){
      sscanf(temp,"%s ",command);
      if(strcmp(command,"Radar")==0) break;
    }
  }


  while(1){
    if(fgets(temp,300,stdin)==NULL) continue;
    sscanf(temp,"%s ",command);

    // Comportamento según estado
    switch(cstate){  
    case LOOK_FOR:
      // Buscamos enimigos por medio do barrido do radar
      //cout << "Print Looking for! " <<endl;
      cout << "Sweep 6 2.0 "<< -1.5 <<" "<< 1.5 << endl;
      cout << "Accelerate 1.0" << endl;
      //      waych++;
      //      if(waych%150 == 0)direc*=-1;
      if(waych>0) direc=1;                                
      else direc=-1;                       
      cout << "RotateAmount 1 2.0 "<< direc*3.14 << endl;
      
      if(strcmp(command,"Radar")==0){
	float dist,rangle; int otype;
	sscanf(temp,"%s %f %d %f",command,&dist,&otype,&rangle);

	if(otype==0){
	  //Se e un enimigo cambiamos estado
	  angle=rangle;
	  cstate=FOUND_ROTATING;
	  cout << "Sweep 6 2.0 0.0 0.0" << endl;
	  cout << "Sweep 6 2.0 0.0 0.0" << endl;
	  cout << "Shoot 3.0" << endl;
	  cout << "Shoot 3.0" << endl;
       	}else if(otype==4){
	  cout << "Shoot 0.5" << endl;	  
	}
      }else if(strcmp(command,"Collision")==0){
	//	if(waych%100==0)direc*=-1;
        if(waych>0) direc=-1;
        else direc=1;
	cout << "RotateAmount 1 2.0 "<<direc*3.14 << endl;
      }
      break;
    case FOUND_ROTATING:
      //cout << "Print found rotating" <<endl;
      //Centramonos
      cout << "Sweep 6 2.0 -1.0 1.0" << endl;
      cout<< "RotateAmount 1 2 "<<direc*angle<<endl;
      cout << "Accelerate 1.5" << endl;      
      if(strcmp(command,"Radar")==0){
	float dist,rangle; int otype;
	sscanf(temp,"%s %f %d %f",command,&dist,&otype,&rangle);

	if(otype==0){
	  //Si es un enemigo cambiamos estado
	  cout << "Shoot 2.0" << endl;
	  angle=rangle;
	  cstate=FOUND_FIRE;	
	  rTout=50;
	}else if(otype==4){
	  cout << "Shoot 0.5" << endl;	  
	}
 	if(rTout<=0){
 	  rTout=50;
 	  cstate=LOOK_FOR;
 	}
 	--rTout;
      }	  
      break;
    case FOUND_FIRE:
      //cout << "Print found fire" <<endl;
      cout << "Sweep 6 2.0 0.2 0.2" << endl;
      cout << "Accelerate 1.0" << endl;      
      cout << "Shoot "<< 1.0 << endl;
      if(strcmp(command,"Radar")==0){
	float dist,rangle; int otype;
	sscanf(temp,"%s %f %d %f",command,&dist,&otype,&rangle);
	if(otype==0){
	  cout << "Shoot "<< 10.0/(dist+1.0) << endl;
	  timeout=10;
	}
	if(otype==0 && dist<1){
	  cout << "Break 1" << endl;
	  for(int i=0; i<5;i++){
	    cout << "Shoot 1.0\n";
	  }
	  cout << endl;
	}
	if(otype!=0) --timeout;
	if(otype==4){
	  cout << "Shoot 0.5" << endl;	  
	}
	if(timeout<=100){
	  timeout=10;
	  cstate=WIDE_SEARCH;
	  direc*=-1;
	}
      }	  
      break;
    case WIDE_SEARCH:
      //cout << "Print desesperate search!" <<endl;
      cout << "Accelerate 0.8" << endl;      
      if(strcmp(command,"Radar")==0){
	float dist,rangle; int otype;
	sscanf(temp,"%s %f %d %f",command,&dist,&otype,&rangle);

	if(otype!=0) --wtimeout;
	else cstate=FOUND_FIRE;

	if(otype==4){
	  cout << "Shoot 0.5" << endl;	  
	}
	if(wtimeout%150 == 0){
	  direc*=-1;
	}else if(wtimeout<-450){
	  wtimeout=10;
	  cstate=LOOK_FOR;
	}
	cout << "RotateAmount 1 2.0 "<< direc*2 << endl;

      }	 else if(strcmp(command,"Collision")==0){
	if(wtimeout>-100) cout << "RotateAmount 1 2.0 "<<3.14 << endl;
	else cout << "RotateAmount 1 2.0 "<<-3.14 << endl;
      }
      break;
    default:
      break;
    }
  }

  return 0;
}
