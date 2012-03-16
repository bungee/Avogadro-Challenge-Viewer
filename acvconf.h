#ifndef _config_h_
#define _config_h_
#define _VERSION_ "1.6"

#define GL_GLEXT_PROTOTYPES
#if defined( __APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "avi.h"


#define PI 3.141592f
#define LEFT 1
#define RIGHT 2
#define LEFT_RIGHT 3
#define X 0
#define Y 1
#define Z 2

//--------------------------------------------------
class Type
{
 public:
  int R, G, B, alpha;
  int types;
  int *visible;
  int *clock, *rlock;
  GLfloat (*color)[4];
  GLfloat *rad;
  GLfloat *c_rad;

  void Init(const int TYPE_MAX){
    R=0; G=1; B=2; alpha=3;
    types =TYPE_MAX;
    visible = new int[types];
    for(int i=0;i<types;i++) visible[i] = 1;
    clock = new int[types];
    for(int i=0;i<types;i++) clock[i] = 0;
    rlock = new int[types];
    for(int i=0;i<types;i++) rlock[i] = 0;

    color = new GLfloat[types][4];
    const float degree = 6.0f/static_cast<float>(types);
    for(int i=0;i<types;i++){
      const float h = i*degree;
      const int h_i = static_cast<int>(h);
      const float f = h - static_cast<float>(h_i);
      const float V=1.0f;
      const float S=1.0f;
      const float p=V*(1.0f-S);
      const float q=V*(1.0f-f*S);
      const float t=V*(1.0f-(1.0f-f)*S);
      
      switch(h_i){
      case 0:
	color[i][R]=V;   color[i][G]=t;   color[i][B]=p;   color[i][alpha]=1.0f;
	break;
      case 1:
	color[i][R]=q;   color[i][G]=V;   color[i][B]=p;   color[i][alpha]=1.0f;
	break;
      case 2:
	color[i][R]=p;   color[i][G]=V;   color[i][B]=t;   color[i][alpha]=1.0f;
	break;
      case 3:
	color[i][R]=p;   color[i][G]=q;   color[i][B]=V;   color[i][alpha]=1.0f;
	break;
      case 4:
	color[i][R]=t;   color[i][G]=p;   color[i][B]=V;   color[i][alpha]=1.0f;
	break;
      case 5:
	color[i][R]=V;   color[i][G]=p;   color[i][B]=q;   color[i][alpha]=1.0f;
	break;
      }
      
    }

    rad = new GLfloat[TYPE_MAX];
    for(int i=0;i<TYPE_MAX;i++){
      rad[i] = 1.0f;
    }
    
    c_rad = new GLfloat[TYPE_MAX];
    for(int i=0;i<TYPE_MAX;i++){
      c_rad[i] = 1.0f;
    }
    
  }

  void visibleAll(){
    for(int i=0;i<types;i++)visible[i] = 1;
  }

  void alphaAll(){
    static float alp=1.0;
    alp *= 0.8f;
    if(alp<0.0001) alp=1.0;
    for(int i=0;i<types;i++)color[i][alpha] = alp;
  }
  
  void visibleReverseAll(){
    for(int i=0;i<types;i++)visible[i] ^= 1;
  }
  
  void visibleReverse(const int num){
    visible[num] ^= 1;
  }

  void alphaRotate(const int num){
    color[num][alpha] *= 0.8f;
    if(color[num][alpha]<0.0001 &&visible[num]== 0 )
      color[num][alpha]=1.0;

    if(color[num][alpha]<0.0001)
      visible[num] = 0;
    else
      visible[num] = 1;
  }
  

};


//--------------------------------------------------
class System
{
 public:
  //// Window infomation
  GLuint windowPosition[2];
  GLuint windowSize[2];
  GLfloat bgColor[4];

  //// Light information
  GLfloat specular[4];
  GLfloat ambient[4];
  GLfloat shininess[1];
  GLfloat lightPosition[4];

  //// System information
  int numberParticle;
  GLfloat systemSize[3];
  GLfloat systemCenter[3];
  GLfloat boxColor[4];
  GLfloat boxStart[3], boxEnd[3];
  GLfloat boxWidth;
  GLuint boxVisible;
  int boxDetail;
  int TYPE_MAX;

  //// Orientation information
  GLfloat distanceX, distanceY, distanceZ;
  GLfloat quatanion[4];
  GLfloat axis[3];
  GLint angle;

  //// Object information
  GLuint detail;
  GLfloat scale;

  //// Povray Information
  int shadow;

  //// Flag
  int loaded;

  //// Data Column
  int column[10];
  int columns;
  char splitter;

  //// Movie
  int fps;

  //Display
  int ortho;
  int multi ;
  int stereo;

  //Cone
  int ConeDirection[3];
  bool ConeDisplay;
  
  System(void){
    loaded = 0;

    ///Window
    windowPosition[0] = 100;  windowPosition[1]=100;
    windowSize[0] = 640;  windowSize[1] = 480;
    bgColor[0] = 0.0f;  bgColor[1] = 0.0f;  bgColor[2] = 0.0f;  bgColor[3] = 1.0f;

    ///Light
    specular[0] = 0.2f;  specular[1] = 0.2f;  specular[2] = 0.2f;  specular[3] = 1.0f;
    ambient[0] = 0.1f;  ambient[1] = 0.1f;  ambient[2] =0.1f;  ambient[3] = 1.0f;
    shininess[0] = 64.0f;
    lightPosition[0]=100000.0f; lightPosition[1]=100.0f; lightPosition[2]=100000.0f; lightPosition[3]=0.0f;

    ///System
    numberParticle = 100000000;
    systemSize[0] = 0.0f; systemSize[1] = 0.0f; systemSize[2] = 0.0f;
    systemCenter[0] = 0.0f; systemCenter[1] = 0.0f; systemCenter[2] = 0.0f;
    boxColor[0] = 1.0f; boxColor[1] = 1.0f; boxColor[2] = 1.0f;  boxColor[3] = 1.0f;
    boxStart[0] = 0.0f; boxStart[1] = 0.0f; boxStart[2] = 0.0f;
    boxEnd[0] = 1.0f; boxEnd[1] = 1.0f; boxEnd[2] = 1.0f;
    boxWidth = 0.1f;
    boxVisible = 0;
    boxDetail = 20;
#ifdef DEMO
    TYPE_MAX = 5;
#else
    TYPE_MAX = 40;
#endif

    ///Orientation
    distanceX = 0.0;
    distanceY = 0.0;
    distanceZ = 0.0;
    quatanion[0] = 1.0; quatanion[1] = 0.0; quatanion[2] = 0.0; quatanion[3] = 0.0;
    axis[0] = 0.0; axis[1] = 0.0; axis[2] = 0.0;
    angle = 0;

    ///Object
    detail = 15;
    scale = 1.0;

    ///Povray
    shadow = 0;

    //// Data Column
    columns = 5;
    column[0] = 0; //id
    column[1] = 1; //kind
    column[2] = 2; //x
    column[3] = 3; //y
    column[4] = 4; //z
    column[5] = -1; //r
    column[6] = -1; //vx
    column[7] = -1; //vy
    column[8] = -1; //vz
    column[9] = -1; //T
    splitter = 0x20;//space

    //// Movie
    fps = 30;

    ////Display
    ortho = 0;
    multi = 0;
    stereo = 0;

    ////ConeDirection
    ConeDirection[0] = 0;
    ConeDirection[1] = 0;
    ConeDirection[2] = 0;
    ConeDisplay=false;
  }
  
  void setInit(void){
    if(distanceX == 0.0){
      float max=systemSize[1]*0.5f;
      if(max < systemSize[2]*0.5f) max = systemSize[2]*0.5f;
      
      float distance = (max + 2.0f)*tanf(75.0f/180.0f*PI)*1.3f;
      if(distanceX < distance)
	distanceX = distance;
    }
    if( angle != 0 ){
      float radian = static_cast<float>(angle) * PI / 180.0f * 0.5f;
      quatanion[0] = cos(radian);
      quatanion[1] = axis[0]*sin(radian);
      quatanion[2] = axis[1]*sin(radian);
      quatanion[3] = axis[2]*sin(radian);
    }
  }
};

//--------------------------------------------------
class Particle
{
 public:
  int id, type;
  GLfloat x[3], r;
  GLfloat v[3],norm,theta;
};


/**********************************************
   acv.cpp
 *********************************************/
void displayCore(void);
void displayRecord(void);
void display(void);
void displayMulti(void);
void displayStereo(void);
void drawLeftDisplay(void);
void drawRightDisplay(void);
void drawRightEyeDisplay(float left,float  right,float  bottom,float  top,float  near,float  far,float  translation, float depth);
void drawLeftEyeDisplay(float left,float  right,float  bottom,float  top,float  near,float  far,float  translation, float depth);
void display_select_mode(void);
void resize(int w, int h);
void resizeMulti(int w, int h);
void resizeStereo(int w, int h);
void init(void);
void multQuatanion(GLfloat r[], const GLfloat p[], const GLfloat q[]);
void rotatQuotanion(GLfloat r[], GLfloat q[]);
void KeyboardEvent(unsigned char key, int x, int y);
void SpecialKeyEvent(int key, int x, int y);
void MouseEvent(int button, int state, int x, int y);
void idle(void);
void motion(int x, int y);
void selectParticles(const int x, const int y);
float calcDistance( const int p1, const int p2);
void displayText();
void displayPosition();
bool loadNextFile(const int count);


#endif
