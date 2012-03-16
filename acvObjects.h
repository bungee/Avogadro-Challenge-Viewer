#ifndef _acvobjects_h_
#define _acvobjects_h_
#include "acvconf.h"
#include "acvShader.h"

class ACVO
{
  private:
  //// Sphere Buffer
  GLuint *sphereVertexBuffer;
  GLuint sphereNormalBuffer;
  GLuint sphereFaceBuffer;
  GLuint *sphereColorBuffer;
  
  //// cone Buffer
  GLuint *coneVertexBuffer;
  GLuint coneNormalBuffer;
  GLuint coneFaceBuffer;
  GLuint *coneColorBuffer;
  
  //// Cylinder Buffer
  GLuint *cylinderVertexBuffer;
  GLuint cylinderNormalBuffer;
  GLuint cylinderFaceBuffer;
  GLuint *cylinderColorBuffer;
  
  //// System Box Buffer
  GLuint BoxList;
  GLuint boxVertexBuffer[2];
  GLuint boxNormalBuffer[2];
  GLuint boxFaceBuffer[2];
  GLuint boxColorBuffer[2];
  
  //// Text DisplayList
  GLuint TextList;

  Type typ;
  Type typ_box;
  System sys;
  
  void changeDetail(void);
  void changeObjects(void);
  void setObjects(void);
  void generateObjects(void);
  void deleteObjects(void);

  void setSphereNormals(const int mesh);
  void setSphereVertices( const float scale, const int mesh);
  void setSphereFaces(const int mesh);
  void setSphereColors(const int mesh);

  void setConeNormals(const int mesh);
  void setConeVertices(const float scale, const int mesh);
  void setConeFaces(const int mesh);
  void setConeColors(const int mesh);

  void setCylinderNormals(const int mesh);
  void setCylinderVertices(const float scale, const int mesh);
  void setCylinderFaces(const int mesh);
  void setCylinderColors(const int mesh);

  void setBoxSphereNormals(const int mesh);
  void setBoxSphereVertices(const float scale, const int mesh);
  void setBoxSphereFaces(const int mesh);
  void setBoxSphereColors(const int mesh);
  void setBoxCylinderNormals(const int mesh);
  void setBoxCylinderVertices(const float scale, const int mesh);
  void setBoxCylinderFaces(const int mesh);
  void setBoxCylinderColors(const int mesh);
  void genCellBox(void);
  void delCellBox(void);
  void boxSphere(const int mesh);
  void boxCylinder(const int mesh);

  ACVShader *shd;
  GLuint pointsprite;
  GLuint positionLoc,colorLoc,viewMatrixLoc,projMatrixLoc,lightLoc;
  //GLint sizeLoc, backLoc, viewportLoc;
  
public:
  ACVO(Type _typ, System _sys);
  ~ACVO();

  void resetObjects(System _sys);
  void changeObjects(System _sys);

  void Sphere(const int type, const int mesh);
  void Cone(const int type, const int mesh);
  void Cylinder(const int type, const int mesh);

  void resetCellBox(void);
  void setCellBox(void);
  void drawCellBox(void);

  void resetText(void);
  void setText(const int list_num, const char *string);
  void drawText(const int list_num);

  void initPointSprite330();
  void initPointSprite120();
  void drawPointSprite330(Particle *P, System sys);
  void drawPointSprite120(Particle *P, System sys);
  void initParticleTexture();
  void drawParticleTexture(Particle *P, System sys,GLfloat *vm, Type typ);

};
#endif
