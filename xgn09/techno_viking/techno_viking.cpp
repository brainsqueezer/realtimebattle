/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */ 

#include <iostream>
#include <string>
#include <math.h>

#define DEFAULT_ANGLE 1.5

using namespace std;

main()
{ string order,msg;double distance,angle,time,speed,energy;int object_type,side=1,x,y;long pill=0,crash=0;cout<<"RobotOption 3 0"<<endl; cout<<"RobotOption 1 1"<<endl;cout<<"Name Techno_Viking"<<endl;cout<<"Colour 010101 101010"<<endl; while(1)  { cin>>order; if(order=="Radar") { cin>>distance>>object_type>>angle; switch(object_type) { case 0: cout<<"Accelerate 10.0"<<endl; if(angle<0) angle-=1; else angle+=1; cout<<"RotateAmount 1 1.0 "<<angle<<endl; cout<<"Shoot 5"<<endl; if(distance<5){ cout<<"Shoot 25"<<endl;  cout<<"Shoot 5"<<endl; }if(distance<3) {cout<<"Accelerate 10"<<endl;cout<<"Shoot 30"<<endl; } case 1: cout<<"RotateAmount 1 1.0 "<<angle; cout<<"Accelerate 5.0"<<endl; break; case 3: if(angle<0) angle-=0.5; else angle+=0.5; if((distance>5)){cout<<"RotateAmount 1 1.0 "<<angle<<endl; cout<<"Shoot 0.5"<<endl<<"Shoot 0.5"<<endl<<"Shoot 0.5"<<endl; cout<<"Shoot 0.5"<<endl; } else {  cout<<"RotateAmount 1 1.0 "<<angle<<endl; cout<<"Accelerate 10.0"<<endl; } break; case 4:  if(distance<4){ if(angle<0) angle-=0.5; else angle+=0.5; cout<<"RotateAmount 1 1.0 "<<angle<<endl;cout<<"Shoot 0.5"<<endl; cout<<"Shoot 0.5"<<endl;cout<<"Shoot 0.5"<<endl; } break; case 2: if(distance>8){cout<<"Accelerate 1.0"<<endl;pill++; } if(distance<8){  if(angle<0)angle-=0.5; else angle+=0.5; cout<<"Accelerate 0.5"<<endl; cout<<"Brake 0.3"<<endl; cout<<"RotateAmount 1 1.0"<<angle<<endl; cout<<"Brake 0"<<endl;} break;   }}if((order=="Dead")||(order=="GameFinishes")) { cout<<"Print 101 por el culo te la hinco"<<endl;}if(order=="Info"){ cin>>time>>speed>>angle;} if(order=="Energy") {cin>>energy; } if(order=="Collision"){ crash=1;  if(object_type==0) {cout<<"RotateAmount 1 1.0"<<angle<<endl; cout<<"Shoot 30.0"<<endl;}  if(object_type==2) {cout<<"Brake 0.5"<<endl; cout<<"RotateAmount 1 1 "<<side*1.5<<endl;  cout<<"Brake 0"<<endl; cout<<"Accelerate 2"<<endl; } if(object_type==3) {cout<<"Print Me encantan las pastis!"<<endl;}  if(object_type==4) { cout<<"RotateAmount 1 1 "<<side*1.5<<endl;  cout<<"Brake 0"<<endl; cout<<"Print This is Madness!!!"<<endl;}}  if(order=="Warning"){  cin>>x>>msg;} if(order=="GameOption") { cin>>x>>angle;  } if(order=="RobotsLeft") cin>>x;   } return 0;}
