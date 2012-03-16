#include "acvconf.h"
#include "acvObjects.h"
#include "acvIO.h"

//----------------------------------------
//mouse event
int clicked=-1;
int clickedX, clickedY;
int targetBoxVisible=0,targetBoxKeep=0;
int targetBoxP[3];
int activeShift=0,activeCtrl=0,activeAlt=0;
//key event
int animation = 0,outputPict = 0, outputPov = 0;
//quatanion
GLfloat tq[4];
GLfloat rt[16];
float aspect;
//Classes
System sys;
Type typ;
Particle *P;
//Calculate frame per sec
GLfloat GLframe=0.0,GLtimenow=0.0,GLtimebase=0.0;
//AVI
MakeAVI *aout;
int avirec = 0;
//Object 
ACVO *obj;
File io;
Option param;
GLuint list;

int main(int argc, char **argv)
{
  //std::cout<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
  //return 0;
  int numfiles=0;
  param.parseOptions(argc, argv,numfiles,sys,typ);
  std::string *filenames = io.setFileNames(numfiles);
  P = new Particle[sys.numberParticle];
#ifdef DEMO
  io.setFCC_DEMO(sys, typ, P);
  //setRandom_DEMO(sys, typ, P);
  std::cout<<" -------------  FCC DEMO ---------------"<<std::endl;
  std::cout<<"   Number of particle ="<<sys.numberParticle<<std::endl;
  std::cout<<"\n< FPS Benchmark >"<<std::endl;
#else

  //// Load Files
  numfiles = 0;
  for(int i=1; i<argc; i++){
    const std::string arg = std::string( argv[i] );
    if( arg.find("-")!=0){
      filenames[numfiles]= std::string(argv[i]);
      std::cout<<"reading "<< filenames[numfiles++]<<std::endl;
    }else{
      if( arg.find("-c")!=std::string::npos)i++;
    }
  }
  if( numfiles == 0 ) exit(0);
  if( !io.loadFile(sys,typ, P, filenames[0]) ) exit(0);
  io.setNumOfFile(numfiles);
  io.setFilenameFormat(filenames[0]);
  sys.setInit();

#endif

  glutInitWindowPosition(sys.windowPosition[0], sys.windowPosition[1]);
  glutInitWindowSize(sys.windowSize[0], sys.windowSize[1]);
  glutInit(&argc, argv);
  if(sys.multi == 1){
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow("Avogadro Challenge Viewer (MULTI VIEW MODE)");
    glutDisplayFunc(displayMulti);
    glutReshapeFunc(resizeMulti);
  }else if(sys.stereo == 1) {
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH ); // for quad-buffer unsupported
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
    glutCreateWindow("Avogadro Challenge Viewer (STEREO MODE)");
    glutDisplayFunc(displayStereo);
    glutReshapeFunc(resizeStereo);
  }else {
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutCreateWindow("Avogadro Challenge Viewer ");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
  }
  glutMotionFunc(motion);
  glutMouseFunc(MouseEvent);
  glutKeyboardFunc(KeyboardEvent);
  glutSpecialFunc(SpecialKeyEvent);
  init();

  glutMainLoop();

  return 0;
}


//------------------------------------------------------------
//  Core method for display function
//------------------------------------------------------------
void displayCore(void){
  glMultMatrixf(rt);

#if !(defined(FAST)|| defined(DEMO))
  glShadeModel(GL_SMOOTH);
  /// System Box
  if(sys.boxVisible)
    obj->drawCellBox();
#endif

  /// Particles
  GLfloat dx, dy, dz;
  dx = 0;  dy = 0;  dz = 0;
#ifdef SHADER330
  obj->drawPointSprite330(P, sys);
#elif SHADER120
  obj->drawPointSprite120(P, sys);
#endif
#ifdef TEXTURE
  obj->drawParticleTexture(P, sys,rt,typ);
#endif

#if !(defined(TEXTURE)||defined(SHADER120)||defined(SHADER330))
  for(int i=0;i<sys.numberParticle;i++){
  #if !(defined(FAST)|| defined(DEMO))
    if( typ.visible[P[i].type] == 0) continue;
  #endif
    glTranslatef( P[i].x[X] - dx, P[i].x[Y] - dy, P[i].x[Z] - dz);

  #if !(defined(FAST)|| defined(DEMO))
    if(!sys.ConeDisplay){
  #endif
      obj->Sphere(P[i].type, sys.detail);
  #if !(defined(FAST)|| defined(DEMO))
    }else{
      glPushMatrix();
      GLfloat ax = sys.ConeDirection[1]*P[i].v[Z] - sys.ConeDirection[2]*P[i].v[Y];
      GLfloat ay = sys.ConeDirection[2]*P[i].v[X] - sys.ConeDirection[0]*P[i].v[Z];
      GLfloat az = sys.ConeDirection[0]*P[i].v[Y] - sys.ConeDirection[1]*P[i].v[X];
      glRotatef(P[i].theta ,ax,ay,az);
      GLfloat sign=ax*sys.ConeDirection[0]+ay*sys.ConeDirection[1]+az*sys.ConeDirection[2];
      glScalef(P[i].norm,P[i].norm,P[i].norm);
      //cone(P[i].type, sys.detail);
      //obj->Cone((int)(P[i].norm*20-1), sys.detail);
      if(sys.column[1]==-1){
	if(sign>0)
	  obj->Cone((int)(((P[i].theta-90.0)/90.0)*10), sys.detail);
	else
	  obj->Cone((int)(((P[i].theta-90.0)/90.0)*10+20), sys.detail);
      }else{
	obj->Cone(P[i].type, sys.detail);	
      }
      glPopMatrix();
    }
  #endif

    dx = P[i].x[X];  dy = P[i].x[Y];  dz = P[i].x[Z];
  }
#endif

#if !(defined(FAST)|| defined(DEMO))
  // Selected Particles
  if(targetBoxVisible!=0){
    glDisable(GL_LIGHTING);
    obj->drawText(targetBoxP[0]);
    for(int i=0;i<targetBoxP[0];i++){
      int num= targetBoxP[i+1];
      glTranslated( P[num].x[X] - dx, P[num].x[Y] - dy, P[num].x[Z] - dz);
      glLineWidth(2);
      glColor3f(1.0,1.0,0.0);
      glutWireCube(typ.rad[P[num].type]*sys.scale*2);
      dx = P[num].x[X];  dy = P[num].x[Y];  dz = P[num].x[Z];    
    }
    glEnable(GL_LIGHTING);
  }
#endif

}

//------------------------------------------------------------
//  Record of outputs
//------------------------------------------------------------
void displayRecord(void)
{
#if !(defined(FAST)|| defined(DEMO))
  if(avirec==1) aout->recAVI();

  if( animation !=0 ){
    if(!loadNextFile(animation) || io.getCurrentFileNumber() == 0){
      animation = 0;
      outputPict = 0;
      outputPov=0;
    }
    if(targetBoxVisible){
      displayText();
    }
    if(outputPict==1){
      std::string fname = io.getCurrentFilename("bmp");
      io.saveBitmap(fname.c_str() ,sys.windowSize[0],sys.windowSize[1]);
      std::cout<<"save snapshot : " << fname <<std::endl;
    }else if(outputPov==1){
      std::string fname = io.getCurrentFilename("pov");
      io.savePovRay(fname.c_str() ,typ, sys, P, rt);
      std::cout<<"save povray file : " << fname <<std::endl;
    }

    glutPostRedisplay();
  }
   
#endif

}


//------------------------------------------------------------
//  Call Back: Main Routine
//------------------------------------------------------------
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  gluLookAt(sys.distanceX, sys.distanceY, sys.distanceZ, 0.0, sys.distanceY, sys.distanceZ, 0.0, 0.0,1.0);

  displayCore();

  glutSwapBuffers();

  displayRecord();

}


//------------------------------------------------------------
//  Call Back: Main Routine for Multi-Viewports
//------------------------------------------------------------
void displayMulti(void ){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  list = glGenLists(1);
  drawLeftDisplay();
  drawRightDisplay();
  glDeleteLists(list,1);
  glutSwapBuffers();
}

//------------------------------------------------------------
//  Call Back: Main Routine for Stereo-Viewports
//------------------------------------------------------------
void displayStereo(void ){
  float znear = 1.0;
  float zfar = 2000.0;
  float screen = 1000.0;
  float depth = -10.0;
  float fovy = 45.0; 
  float IOD = 0.2;

  float top = znear * tan(fovy* 0.5 * PI / 180.0);
  float right = aspect * top;
  float frustumshift = IOD*0.5*znear/screen;

  list = glGenLists(1);
  drawLeftEyeDisplay(-right+frustumshift, right+frustumshift, -top, top, znear, zfar, IOD*0.5, depth);
  drawRightEyeDisplay(-right-frustumshift, right-frustumshift, -top, top, znear, zfar, -IOD*0.5, depth);
  glDeleteLists(list,1);
  glutSwapBuffers();
}

//------------------------------------------------------------
//  Draw Left Display
//------------------------------------------------------------
void drawLeftDisplay(void)
{
  glViewport(0, 0, sys.windowSize[0]/2, sys.windowSize[1]);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float scale = tan(30.0 * 0.5 * PI / 180.0)/sys.windowSize[1]*2;
  float x =  scale *  static_cast<GLfloat>(sys.windowSize[0]/2);
  float y =  scale *  static_cast<GLfloat>(sys.windowSize[1]);
  glFrustum(-x, x,-y, y, 1, 2000);

  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);


  glLoadIdentity();
  gluLookAt(sys.distanceX, sys.distanceY, sys.distanceZ, 0.0, sys.distanceY, sys.distanceZ, 0.0, 0.0,1.0);

  glNewList(list, GL_COMPILE_AND_EXECUTE);

  displayCore();

  glEndList();

  displayRecord();
}

//------------------------------------------------------------
//  Draw Right Display
//------------------------------------------------------------
void drawRightDisplay(void)
{
  glViewport(sys.windowSize[0]/2, 0, sys.windowSize[0]/2, sys.windowSize[1]);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float scale = tan(30.0 * 0.5 * PI / 180.0)/sys.windowSize[1]*2;
  float x =  scale *  static_cast<GLfloat>(sys.windowSize[0]/2);
  float y =  scale *  static_cast<GLfloat>(sys.windowSize[1]);
  glFrustum(-x, x,-y, y, 1, 2000);

  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  gluLookAt(sys.distanceY, -sys.distanceX, sys.distanceZ, 0.0, sys.distanceY, sys.distanceZ, 0.0, 0.0,1.0);

  glCallList(list);

  displayRecord();
}

//------------------------------------------------------------
//  Draw Left Eye Display for Stereo
//------------------------------------------------------------
void drawLeftEyeDisplay(float left,float  right,float  bottom,float  top,float  znear,float  zfar,float  translation, float depth)
{
  glDrawBuffer(GL_BACK_LEFT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, bottom, top, znear, zfar);
  
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);
  glLoadIdentity();
  gluLookAt(sys.distanceX,sys.distanceY, sys.distanceZ,0,sys.distanceY,sys.distanceZ,0,0,1);
  glTranslated(translation, 0.0, depth);

  //glPolygonStipple((GLubyte *)emask);
  glNewList(list, GL_COMPILE_AND_EXECUTE);

  displayCore();
  
  glEndList();

  displayRecord();

}

//------------------------------------------------------------
//  Draw Right Eye Display for Stereo
//------------------------------------------------------------
void drawRightEyeDisplay(float left,float  right,float  bottom,float  top,float  znear,float  zfar,float  translation, float depth)
{
  glDrawBuffer(GL_BACK_RIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, bottom, top, znear, zfar);

  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);
  glLoadIdentity();
  gluLookAt(sys.distanceX,sys.distanceY, sys.distanceZ,0,sys.distanceY,sys.distanceZ,0,0,1);
  glTranslated(translation, 0.0, depth);
  
  //glPolygonStipple((GLubyte *)omask);
  glCallList(list);

  displayRecord();

}

//------------------------------------------------------------
//  Display Function for Select Buffer
//------------------------------------------------------------
void display_select_mode(void)
{
  glLoadIdentity();
  gluLookAt(sys.distanceX, sys.distanceY, sys.distanceZ, 0.0, sys.distanceY, sys.distanceZ, 0.0, 0.0,1.0);
  
  glMultMatrixf(rt);

  GLfloat dx, dy, dz;

  dx = 0;  dy = 0;  dz = 0;
  for(int i=0;i<sys.numberParticle;i++){
    if( typ.visible[P[i].type] == 0) continue;
    glTranslatef( P[i].x[X] - dx, P[i].x[Y] - dy, P[i].x[Z] - dz);
    glLoadName(i);
    glutSolidSphere(typ.rad[P[i].type]*sys.scale, 5, 5);
    dx = P[i].x[X];  dy = P[i].x[Y];  dz = P[i].x[Z];
  }
}


//------------------------------------------------------------
//  Window Resize
//------------------------------------------------------------
void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //gluPerspective(30.0, static_cast<GLfloat>(w)/static_cast<GLfloat>(h), 1.0, 2000.0);

  if(sys.ortho == 1){  
    aspect = static_cast<GLfloat>(w)/static_cast<GLfloat>(h);
    float x = (sys.distanceX - sys.systemSize[0])*0.5*aspect;
    float y = (sys.distanceX - sys.systemSize[0])*0.5;  
    glOrtho(-x, x,-y, y, 1,2000);
  }else{
    float scale = tan(30.0 * 0.5 * PI / 180.0)/sys.windowSize[1];
    float x =  scale *  static_cast<GLfloat>(w);
    float y =  scale *  static_cast<GLfloat>(h);
    glFrustum(-x, x,-y, y, 1, 2000);
  }

  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);
  
  sys.windowSize[0] = w;
  sys.windowSize[1] = h;
}

//------------------------------------------------------------
//  Window Resize For Multi-Viewports
//------------------------------------------------------------
void resizeMulti(int w, int h)
{
  aspect = static_cast<GLfloat>(w)/static_cast<GLfloat>(h);
  sys.windowSize[0] = w;
  sys.windowSize[1] = h;
}

//------------------------------------------------------------
//  Window Resize For Stereo-Viewports
//------------------------------------------------------------
void resizeStereo(int w, int h)
{
  aspect = static_cast<GLfloat>(w)/static_cast<GLfloat>(h);
  glViewport(0, 0, w, h);
  sys.windowSize[0] = w;
  sys.windowSize[1] = h;
}

//------------------------------------------------------------
//  Initialization of OpenGL
//------------------------------------------------------------
void init(void)
{
  glClearColor(sys.bgColor[0],sys.bgColor[1],sys.bgColor[2],sys.bgColor[3]);
#ifdef ALPHA
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE); //addition
  //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // alpha blend
#else
  glEnable(GL_DEPTH_TEST);
#endif
  //glEnable(GL_NORMALIZE);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  // if(sys.stereo==1){
  //   for (int i = 0; i < 32; ++i)
  //     emask[i] = ~(omask[i] = -(i & 1));
  //   glEnable(GL_POLYGON_STIPPLE);
  // }

  glLightfv(GL_LIGHT0, GL_SPECULAR, sys.specular);
  glLightfv(GL_LIGHT0, GL_SHININESS, sys.shininess);
  glLightfv(GL_LIGHT0, GL_AMBIENT, sys.ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, sys.lightPosition);
  
  obj = new ACVO(typ, sys);
  rotatQuotanion(rt, sys.quatanion);
  //generateObjects(typ,sys);
#if !(defined(FAST)|| defined(DEMO))
  if(sys.boxVisible)
    obj->setCellBox();
#endif

  tq[0] = 1.0; tq[1] = 0.0; tq[2] = 0.0; tq[3] = 0.0;
#ifdef FFMPEG
  aout = new MakeAVI("movie.mp4");
#else
  aout = new MakeAVI("movie.avi");
#endif


#ifdef SHADER330
  obj->initPointSprite330();
#elif SHADER120
  obj->initPointSprite120();
#endif
#ifdef TEXTURE
  obj->initParticleTexture();
#endif
 
}


//------------------------------------------------------------
//  Quatanion Multiply
//------------------------------------------------------------
void multQuatanion(GLfloat r[], const GLfloat p[], const GLfloat q[])
{
  r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
  r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
  r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
  r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}
  
//------------------------------------------------------------
//  Quatanion to Rotation Matrix
//------------------------------------------------------------
void rotatQuotanion(GLfloat r[], GLfloat q[])
{
  const GLfloat x2 = q[1] * q[1] * 2.0;
  const GLfloat y2 = q[2] * q[2] * 2.0;
  const GLfloat z2 = q[3] * q[3] * 2.0;
  const GLfloat xy = q[1] * q[2] * 2.0;
  const GLfloat yz = q[2] * q[3] * 2.0;
  const GLfloat zx = q[3] * q[1] * 2.0;
  const GLfloat xw = q[1] * q[0] * 2.0;
  const GLfloat yw = q[2] * q[0] * 2.0;
  const GLfloat zw = q[3] * q[0] * 2.0;
  
  r[ 0] = 1.0 - y2 - z2;
  r[ 1] = xy + zw;
  r[ 2] = zx - yw;
  r[ 4] = xy - zw;
  r[ 5] = 1.0 - z2 - x2;
  r[ 6] = yz + xw;
  r[ 8] = zx + yw;
  r[ 9] = yz - xw;
  r[10] = 1.0 - x2 - y2;
  r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0;
  r[15] = 1.0;
}



//------------------------------------------------------------
//  Keyboard Event
//------------------------------------------------------------
void KeyboardEvent(unsigned char key, int x, int y)
{
  std::string fname;
  
  for(int i=1;i<=9;i++){
    if((key-'0') == i){
#ifdef ALPHA
      typ.alphaRotate(i-1);
#else
      typ.visibleReverse(i-1);
#endif
      glutPostRedisplay();
      return;
    }
  }

  switch(key){
  case '0':
#ifdef ALPHA
    typ.alphaAll();
#else
    typ.visibleAll();
#endif

    glutPostRedisplay();
    break;
  case '-':
    typ.visibleReverseAll();
    glutPostRedisplay();
    break;
  case 'q':
    delete []P;
    if(avirec==1) aout->finishAVI();
    exit(0);
    break;
  case 'Q':
    delete []P;
    io.saveParameter(sys,typ);
    if(avirec==1) aout->finishAVI();
    exit(0);
    break;

  case 'd':
    sys.detail -= 1;
    if(sys.detail < 6) sys.detail = 6;
    obj->resetObjects(sys);
    glutPostRedisplay();
    break;
  case 'D':
    sys.detail += 1;
    if(sys.detail >= 60) sys.detail = 60;
    obj->resetObjects(sys);
    glutPostRedisplay();
    break;

  case 'c':
    loadNextFile(1);
    if(targetBoxVisible){
      displayText();
    }
    glutPostRedisplay();
    break;

  case 'C':
    animation = 1;
    glutPostRedisplay();
    break;

  case 'e':
    for(int i=0;i<3;i++)
      sys.bgColor[i]=(1-sys.bgColor[i]);
    glClearColor(sys.bgColor[0],sys.bgColor[1],sys.bgColor[2],sys.bgColor[3]);
    glutPostRedisplay();
    break;

  case 'z':
    loadNextFile(-1);
    if(targetBoxVisible){
      displayText();
    }
    glutPostRedisplay();
    break;

  case 'Z':
    animation = -1;
    glutPostRedisplay();
    break;

  case 'x':
  case 'X':
    animation = 0;
    outputPict = 0;
    glutPostRedisplay();
    break;

  case 'W':
    fname = io.getCurrentFilename( "bmp");
    io.saveBitmap(fname.c_str() ,sys.windowSize[0],sys.windowSize[1]);
    std::cout<<"save snapshot : " << fname <<std::endl;
    glutPostRedisplay();
    break;

  case 'S':
    animation = 1;
    outputPict = 1;
    glutPostRedisplay();
    break;

  case 'A':
    animation = -1;
    outputPict = 1;
    glutPostRedisplay();
    break;

  case 'r':
    sys.scale *= 0.8;
    obj->changeObjects(sys);
    glutPostRedisplay();
    break;
  case 'R':
    sys.scale *= 1.2;
    obj->changeObjects(sys);
    glutPostRedisplay();
    break;
  case 'o':
    fname = io.getCurrentFilename("pov");
    io.savePovRay(fname.c_str() ,typ, sys, P, rt);
    std::cout<<"save povray file : " << fname <<std::endl;
    glutPostRedisplay();
    break;
  case 'O':
    animation = 1;
    outputPov = 1;
    glutPostRedisplay();
    break;

  case 'M':
    animation^=1;
  case 'm':
    avirec ^=1;
    if(avirec==1) std::cout<<"Rec"<<std::endl;
    if(avirec==0) std::cout<<"Pause"<<std::endl;
    aout->initAVI(sys.windowSize[0],sys.windowSize[1],sys.fps);
    aout->recAVI();
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

//------------------------------------------------------------
//  Special Key Event
//------------------------------------------------------------
void SpecialKeyEvent(int key, int x, int y)
{

  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    activeShift = 1;
  else
    activeShift = 0;

  if(glutGetModifiers() == GLUT_ACTIVE_ALT)
    activeAlt = 1;
  else
    activeAlt = 0;
  /*
  if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
    speed = 3.0;
  else
    speed = 1.0;
  */
  GLfloat dx=0,dy=0,dr;
  switch(key){
  case GLUT_KEY_LEFT:
    if(activeShift){
      sys.distanceY += 0.3;
    }else{
      dx = -2.0/static_cast<GLfloat>(sys.windowSize[0]);
    }
    break;
  case GLUT_KEY_RIGHT:
    if(activeShift){
      sys.distanceY -= 0.3;
    }else{
      dx = 2.0/static_cast<GLfloat>(sys.windowSize[0]);
    }
    break;
  case GLUT_KEY_UP:
    if(activeShift){
      sys.distanceZ -= 0.3;
    }else if(activeAlt){
      sys.distanceX -=1;
    }else{
      dy = -2.0/static_cast<GLfloat>(sys.windowSize[1]);
    }

    break;
  case GLUT_KEY_DOWN:
    if(activeShift){
      sys.distanceZ += 0.3;
    }else if(activeAlt){
      sys.distanceX +=1;
    }else{
      dy = 2.0/static_cast<GLfloat>(sys.windowSize[1]);
    }
    break;
  }

  if(!activeShift){
    dr = sqrt(dx * dx + dy * dy);
    if (dr != 0.0) {
      GLfloat ar = dr * PI;
      GLfloat as = sin(ar) / dr;
      GLfloat dq[4] = {cos(ar), 0.0 , dy * as, dx * as };
      multQuatanion(tq, dq, sys.quatanion);
      rotatQuotanion(rt, tq);
    }
    targetBoxKeep=2;
    
    glutIdleFunc(idle);
    sys.quatanion[0] = tq[0];
    sys.quatanion[1] = tq[1];
    sys.quatanion[2] = tq[2];
    sys.quatanion[3] = tq[3];
  }
  
}
//------------------------------------------------------------
//  Mouse Event
//------------------------------------------------------------
void MouseEvent(int button, int state, int x, int y)
{

  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    activeShift = 1;
  else
    activeShift = 0;
  
  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state==GLUT_DOWN){
      clicked = LEFT;
      clickedX = x;
      clickedY = y;
      glutIdleFunc(idle);
      targetBoxKeep=1;
    }else{
      clicked = -1;
      glutIdleFunc(idle);
      sys.quatanion[0] = tq[0];
      sys.quatanion[1] = tq[1];
      sys.quatanion[2] = tq[2];
      sys.quatanion[3] = tq[3];

#if !(defined(FAST)|| defined(DEMO))
      if( clickedX == x && clickedY == y )
	selectParticles(x, y);
#endif

    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if(state==GLUT_DOWN){
      clicked = LEFT_RIGHT;
      clickedX = x;
      clickedY = y;
      glutIdleFunc(idle);
    }else{
      clicked = -1;
      glutIdleFunc(0);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if(state==GLUT_DOWN){
      clicked = RIGHT;
      clickedX = x;
      clickedY = y;
      glutIdleFunc(idle);
    }else{
      clicked = -1;
      glutIdleFunc(0);
    }
    break;
  case 3: // scroll up
    break;
  case 4: // scroll down
    break;
  default:
    break;
  }
}

//------------------------------------------------------------
//  Idle Function
//------------------------------------------------------------
void idle(void)
{
#if defined(FAST)|| defined(DEMO)
  GLframe++;
  GLtimenow = glutGet(GLUT_ELAPSED_TIME);
  if (GLtimenow - GLtimebase > 1000)
    {
      printf("  %0.5f [fps]\n",GLframe*1000.0/(GLtimenow-GLtimebase));
      GLtimebase = GLtimenow;
      GLframe = 0;
    }
#endif

  if(sys.ortho == 1 ){
    glMatrixMode(GL_PROJECTION);  
    glMultMatrixf(rt);
    glLoadIdentity();
    float x = (sys.distanceX - sys.systemSize[0])*0.5*aspect;
    float y = (sys.distanceX - sys.systemSize[0])*0.5;  
    glOrtho(-x, x,-y, y, -1,2000);
    glMatrixMode(GL_MODELVIEW);
  }

  glutPostRedisplay();
}

//------------------------------------------------------------
//  Mouse Drag
//------------------------------------------------------------
void motion(int x, int y)
{
  GLfloat dx, dy, dr;
  switch(clicked){
  case LEFT:

    if(activeShift){
      sys.distanceY += static_cast<GLfloat>(clickedX - x)*0.3;
      sys.distanceZ -= static_cast<GLfloat>(clickedY - y)*0.3;
      clickedX = x;
      clickedY = y;
    }else{
      dx = static_cast<GLfloat>(x - clickedX)/static_cast<GLfloat>(sys.windowSize[0]);
      dy = static_cast<GLfloat>(y - clickedY)/static_cast<GLfloat>(sys.windowSize[1]);
      dr = sqrt(dx * dx + dy * dy);
      
      if (dr != 0.0) {
	GLfloat ar = dr * PI;
	GLfloat as = sin(ar) / dr;
	GLfloat dq[4] = {cos(ar), 0.0 , dy * as, dx * as };
	multQuatanion(tq, dq, sys.quatanion);
	rotatQuotanion(rt, tq);
      }
      
      targetBoxKeep=2;
    }
    
    break;

  case RIGHT:
    sys.distanceX -=static_cast<GLfloat>(clickedY - y)*0.3;
    clickedY = y;
    break;

  case LEFT_RIGHT:
    sys.distanceY += static_cast<GLfloat>(clickedX - x)*0.3;
    sys.distanceZ -= static_cast<GLfloat>(clickedY - y)*0.3;
    clickedX = x;
    clickedY = y;
    break;
  }
}

 
//------------------------------------------------------------
//  Select Particles
//------------------------------------------------------------
void selectParticles(const int x, const int y){
  const int BUFSIZE = 1024;
  GLuint selectBuf[BUFSIZE],*ptr;
  GLint hits, viewport[4];
  
  glSelectBuffer(BUFSIZE, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(-1);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluPickMatrix(x, viewport[3] - y , 1.0, 1.0, viewport);
  gluPerspective(30.0, (float)viewport[2] / (float)viewport[3], 1.0,100.0);
  glMatrixMode(GL_MODELVIEW);
  display_select_mode();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  hits = glRenderMode(GL_RENDER);
  
  ptr = selectBuf;
  int min =  0xffffffff;
  int min_num=0;
  for(int i=0;i<hits; i++){
    ptr++;
    if(min > (int)*ptr){
      min = (int)*ptr;
      min_num = *(ptr+2);
    }
    ptr+=3;
  }
  
  if(hits!=0){
    targetBoxP[0]=1;
    if(activeShift){
      targetBoxP[0]=2;
    }
    targetBoxVisible=1;
    targetBoxP[targetBoxP[0]] = min_num;
  }else{
    targetBoxVisible = --targetBoxKeep;
  }
  displayText();
}

//------------------------------------------------------------
//  Calculate Distance betwee Twe Selected Particles
//------------------------------------------------------------
float calcDistance(const int p1, const int p2){
  float dx = P[p1].x[X] - P[p2].x[X];
  float dy = P[p1].x[Y] - P[p2].x[Y];
  float dz = P[p1].x[Z] - P[p2].x[Z];
  float dr = sqrt(dx*dx + dy*dy + dz*dz);
  return dr;
};

//------------------------------------------------------------
//  Display Information Text of Selected Particles
//------------------------------------------------------------
void displayText(){
  obj->resetText();
  char str[256];
  for(int i=0; i<targetBoxP[0]; i++){
    int num = targetBoxP[i+1];
    sprintf(str, "%d  %d  ( %0.2f, %0.2f, %0.2f)", P[num].id, P[num].type, P[num].x[X]+sys.systemCenter[0], P[num].x[Y]+sys.systemCenter[1], P[num].x[Z]+sys.systemCenter[2]) ;
    obj->setText(2-i, str);
  }
  
  if( targetBoxP[0]==2){
    float dr = calcDistance(targetBoxP[1], targetBoxP[2]);
    sprintf(str, "distance  %0.5f", dr) ;
    obj->setText(0, str);
  }
}

//------------------------------------------------------------
//  Display Information Text of Position
//------------------------------------------------------------
void displayPosition(){
  obj->resetText();
  char str[256];
  sprintf(str, "Position ( %0.2f, %0.2f, %0.2f)", sys.distanceX, sys.distanceY, sys.distanceZ) ;
  obj->setText(2, str);
  sprintf(str, "Look At ( %0.2f, %0.2f, %0.2f)", sys.distanceX, sys.distanceY, sys.distanceZ) ;
  obj->setText(2, str);
}

//------------------------------------------------------------
//  Load Next / Previous File
//------------------------------------------------------------
bool loadNextFile(const int direction){
  std::string fname;
  const int numfiles = io.getNumOfFile();
  int currentfile = io.getCurrentFileNumber();
  if(numfiles>1){
    currentfile += direction;
    if( currentfile >= numfiles ) currentfile=0;
    else if ( currentfile < 0 ) currentfile = numfiles - 1;
    io.setCurrentFileNumber(currentfile);
    fname = io.getCurrentFilename();
    io.loadFile(sys,typ, P, fname);
    std::cout<<"reading  "<< fname<<std::endl;
  }else{
    currentfile+=direction;
    if(currentfile < 0){
      currentfile = 0;
      return false;
    }
    io.setCurrentFileNumber(currentfile);
    fname = io.getCurrentFilename();
    if(!io.loadFile(sys,typ, P, fname)){
      currentfile -= direction;
      io.setCurrentFileNumber(currentfile);
      return false;
    }else{
      std::cout<<"reading  "<< fname<<std::endl;
    }
  }

  obj->changeObjects(sys);
#if !(defined(FAST)|| defined(DEMO))
  if(sys.boxVisible){
    obj->resetCellBox();
  }
#endif
  return true;
}
