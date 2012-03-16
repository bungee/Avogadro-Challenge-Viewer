#include "acvObjects.h"
#include "sphere.h"

ACVO::ACVO(Type _typ, System _sys){
  typ = _typ;
  sys = _sys;
  generateObjects();
  setObjects();
}

ACVO::~ACVO(){
  deleteObjects();
  delCellBox();
}


//------------------------------------------------------------
//       Remake Objects
//------------------------------------------------------------
void 
ACVO::resetObjects(System _sys){
  sys = _sys;
  deleteObjects();
  generateObjects();
  setObjects();
}

void 
ACVO::changeObjects(System _sys){
  sys = _sys;
  changeDetail();
}

void 
ACVO::changeDetail(void){
  const float scale = sys.scale;
  const int mesh = sys.detail;

  /// Sphere
  setSphereVertices(scale,mesh);
  setSphereColors( mesh);

  /// Cone
  setConeVertices(scale, mesh);
  setConeColors( mesh);
  
  /// Cylinder
  setCylinderVertices(scale, mesh);
  setCylinderColors( mesh);
}

//------------------------------------------------------------
//       Make Objects
//------------------------------------------------------------
void 
ACVO::setObjects(void){
  const float scale = sys.scale;
  const int mesh = sys.detail;

  /// Sphere
  setSphereNormals(mesh);
  setSphereVertices(scale,mesh);
  setSphereFaces(mesh);
  setSphereColors( mesh);

  /// Cone
  setConeNormals(mesh);
  setConeVertices(scale, mesh);
  setConeFaces(mesh);
  setConeColors( mesh);
  
  /// Cylinder
  setCylinderNormals(mesh);
  setCylinderVertices(scale, mesh);
  setCylinderFaces(mesh);
  setCylinderColors( mesh);
  
  // Text
  TextList = glGenLists(3);
}

//------------------------------------------------------------
//       Generate Objects Buffer
//------------------------------------------------------------
void 
ACVO::generateObjects(void){
  const int slices = sys.detail, stacks = sys.detail/2;
  const int svertices = (stacks - 1)*slices + 2;
  const int sfaces = 2*(stacks-1)*slices;
  const int cvertices = (stacks - 1)*slices;
  const int cfaces = 2*(stacks-2)*slices;

  //---------- Sphere ------------
  glGenBuffers(1, &sphereNormalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);
  sphereVertexBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types, sphereVertexBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);
  }

  glGenBuffers(1, &sphereFaceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereFaceBuffer); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*sfaces*3, NULL, GL_STATIC_DRAW);

  sphereColorBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types,sphereColorBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*4, NULL, GL_STATIC_DRAW);
  }

  //---------- Cone ------------
  glGenBuffers(1, &coneNormalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, coneNormalBuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);

  coneVertexBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types, coneVertexBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, coneVertexBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);
  }

  glGenBuffers(1, &coneFaceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneFaceBuffer); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*sfaces*3, NULL, GL_STATIC_DRAW);

  coneColorBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types,coneColorBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, coneColorBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*4, NULL, GL_STATIC_DRAW);
  }

  //---------- Cylinder ------------
  glGenBuffers(1, &cylinderNormalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalBuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*3, NULL, GL_STATIC_DRAW);

  cylinderVertexBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types, cylinderVertexBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVertexBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*3, NULL, GL_STATIC_DRAW);
  }

  glGenBuffers(1, &cylinderFaceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderFaceBuffer); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cfaces*3, NULL, GL_STATIC_DRAW);

  cylinderColorBuffer = new GLuint[sys.TYPE_MAX];
  glGenBuffers(typ.types,cylinderColorBuffer);
  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, cylinderColorBuffer[t]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*4, NULL, GL_STATIC_DRAW);
  }

}


//------------------------------------------------------------
//       Delete Objects
//------------------------------------------------------------
void 
ACVO::deleteObjects(void){
  glDeleteBuffers(1, &sphereNormalBuffer);
  glDeleteBuffers(typ.types, sphereVertexBuffer);
  glDeleteBuffers(1, &sphereFaceBuffer);
  glDeleteBuffers(typ.types, sphereColorBuffer);
  glDeleteBuffers(1, &coneNormalBuffer);
  glDeleteBuffers(typ.types, coneVertexBuffer);
  glDeleteBuffers(1, &coneFaceBuffer);
  glDeleteBuffers(typ.types, coneColorBuffer);
  glDeleteBuffers(typ.types, cylinderVertexBuffer);
  glDeleteBuffers(1, &cylinderNormalBuffer);
  glDeleteBuffers(1, &cylinderFaceBuffer);
  glDeleteBuffers(typ.types, cylinderColorBuffer);
  
  delete [] sphereVertexBuffer;
  delete [] sphereColorBuffer;
  delete [] coneVertexBuffer;
  delete [] coneColorBuffer;
  delete [] cylinderVertexBuffer;
  delete [] cylinderColorBuffer;
}

//------------------------------------------------------------
//       Set Normal Buffer of Sphere
//------------------------------------------------------------
void 
ACVO::setSphereNormals(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  float theta, phi;
  const float dtheta = 3.121593f/(float)stacks;
  const float dphi = 2.0f*3.141593f/(float)slices;

  glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  normal[count++] = 0.0f;
  normal[count++] = 0.0f;
  normal[count++] = 1.0f;
  for(int j=0; j<stacks-1; j++){
    theta = dtheta*(j+1);
    for(int i=0; i<slices; i++){
      phi = dphi*(float)i;
      normal[count++] = cos(phi)*sin(theta);
      normal[count++] = sin(phi)*sin(theta);
      normal[count++] = cos(theta);
    }
  }
  normal[count++] = 0.0f;
  normal[count++] = 0.0f;
  normal[count++] = -1.0f;
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Vertex Buffer of Sphere
//------------------------------------------------------------
void 
ACVO::setSphereVertices(const float scale, const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices + 2;

  glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_ARRAY_BUFFER);
 
  GLfloat *vertex, r;
  for(int t=0; t<typ.types; t++){
    r = typ.rad[t]*scale;
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer[t]); 
    vertex = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    for(int j=0;j<vertices*3;j+=3){
      vertex[j] = normal[j]*r;
      vertex[j+1] = normal[j+1]*r;
      vertex[j+2] = normal[j+2]*r;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
}

//------------------------------------------------------------
//       Set Face Buffer of Sphere
//------------------------------------------------------------
void 
ACVO::setSphereFaces(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereFaceBuffer); //face
  GLuint *face = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

  int count=0;
  for(int i=0; i<=slices; i++){
    face[count++] = 0;
    face[count++] = i%slices+1;
  }

  for(int j=1;j<=stacks-2;j++){
    for(int i=0;i<=slices;i++){
      face[count++] = slices*(j-1) + i%slices+1;
      face[count++] = slices*j + i%slices+1;
    }
  }

  int a1=slices*(stacks-1), a2=slices*(stacks-2);
  for(int i=0; i<=slices; i++){
    face[count++] = a2+i%slices+1;
    face[count++] = a1+1;
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Color Buffer of Sphere
//------------------------------------------------------------
void 
ACVO::setSphereColors(const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices + 2;

  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer[t]); 
    GLfloat *color = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    for(int j=0;j<vertices*4;j+=4){
      color[j]=typ.color[t][0];
      color[j+1]=typ.color[t][1];
      color[j+2]=typ.color[t][2];
      color[j+3]=typ.color[t][3];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Draw Sphere
//------------------------------------------------------------
void 
ACVO::Sphere(const int type, const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int faces = 2*(slices+1)*stacks;
  
  glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer[type]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, sphereNormalBuffer);
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer[type]);
  glColorPointer(4, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereFaceBuffer);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawElements(GL_TRIANGLE_STRIP, faces, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
}

//------------------------------------------------------------
//       Set Normal Buffer of Cone
//------------------------------------------------------------
void ACVO::setConeNormals(const int mesh){
  const int slices = mesh;

  float phi;
  const float dphi = 2.0f*3.141593f/(float)slices;

  glBindBuffer(GL_ARRAY_BUFFER, coneNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  normal[count++] = sys.ConeDirection[0]*1.0f;
  normal[count++] = sys.ConeDirection[1]*1.0f;
  normal[count++] = sys.ConeDirection[2]*1.0f;
  for(int i=0; i<slices; i++){
    phi = dphi*(float)i;
    normal[count++] = (sys.ConeDirection[1])*sin(phi) + (sys.ConeDirection[2])*cos(phi);
    normal[count++] = (sys.ConeDirection[0])*cos(phi)+(sys.ConeDirection[2])*sin(phi);
    normal[count++] = (sys.ConeDirection[0])*sin(phi)+(sys.ConeDirection[1])*cos(phi);
  }
  normal[count++] = -sys.ConeDirection[0]*1.0f;
  normal[count++] = -sys.ConeDirection[1]*1.0f;
  normal[count++] = -sys.ConeDirection[2]*1.0f;
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Vertex Buffer of Cone
//------------------------------------------------------------
void
ACVO::setConeVertices(const float scale, const int mesh){
  const int slices = mesh;
  const int vertices = slices + 2;

  glBindBuffer(GL_ARRAY_BUFFER, coneNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_ARRAY_BUFFER);
 
  GLfloat *vertex, r;
  for(int t=0; t<typ.types; t++){
    r = typ.rad[t]*scale;
    glBindBuffer(GL_ARRAY_BUFFER, coneVertexBuffer[t]); 
    vertex = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    vertex[0] = (normal[0]-sys.ConeDirection[0]*0.5f)*2*r;
    vertex[1] = (normal[1]-sys.ConeDirection[1]*0.5f)*2*r;
    vertex[2] = (normal[2]-sys.ConeDirection[2]*0.5f)*2*r;
    for(int j=3;j<(vertices-1)*3;j+=3){
      vertex[j] = (normal[j]-sys.ConeDirection[0])*r;
      vertex[j+1] = (normal[j+1]-sys.ConeDirection[1])*r;
      vertex[j+2] = (normal[j+2]-sys.ConeDirection[2])*r;
    }
    vertex[vertices*3 - 3] = (-sys.ConeDirection[0])*r;
    vertex[vertices*3 - 2] = (-sys.ConeDirection[1])*r;
    vertex[vertices*3 - 1] = (-sys.ConeDirection[2])*r;
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
}

//------------------------------------------------------------
//       Set Face Buffer of Cone
//------------------------------------------------------------
void 
ACVO::setConeFaces(const int mesh){
  const int slices = mesh;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneFaceBuffer); //face
  GLuint *face = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

  int count=0;
  for(int i=0; i<=slices; i++){
    face[count++] = 0;
    face[count++] = i%slices+1;
  }
  
  for(int i=0; i<=slices; i++){
    face[count++] = i%slices+1;
    face[count++] = slices+1;
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Color Buffer of Cone
//------------------------------------------------------------
void 
ACVO::setConeColors(const int mesh){
  const int slices = mesh;
  const int vertices = slices + 2;

  for(int t=0; t<typ.types; t++){
    glBindBuffer(GL_ARRAY_BUFFER, coneColorBuffer[t]); 
    GLfloat *color = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    for(int j=0;j<vertices*4;j+=4){
      color[j]=typ.color[t][0];
      color[j+1]=typ.color[t][1];
      color[j+2]=typ.color[t][2];
      color[j+3]=typ.color[t][3];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Draw Cone
//------------------------------------------------------------
void 
ACVO::Cone(const int type, const int mesh){
  const int slices = mesh;
  const int faces = 4*(slices+1);

  glBindBuffer(GL_ARRAY_BUFFER, coneVertexBuffer[type]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, coneNormalBuffer);
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, coneColorBuffer[type]);
  glColorPointer(4, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneFaceBuffer);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawElements(GL_TRIANGLE_STRIP, faces, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
}


//------------------------------------------------------------
//       Set Normal Buffer of Cylinder
//------------------------------------------------------------
void 
ACVO::setCylinderNormals(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  float phi;
  const float dphi = 2.0f*3.141593f/(float)slices;

  glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  for(int j=0; j<stacks-1; j++){
    for(int i=0; i<slices; i++){
      phi = dphi*(float)i;
      normal[count++] = cos(phi);
      normal[count++] = sin(phi);
      normal[count++] = 0.0f;
    }
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------
//       Set Vertex Buffer of Cylinder
//------------------------------------------------------------
void 
ACVO::setCylinderVertices( const float scale, const int mesh){
  const int slices = mesh, stacks = mesh/2;
  
  glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalBuffer); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_ARRAY_BUFFER);

  GLfloat *vertex, r, height;
  const float dheight = 1.0f/(float)(stacks);

  for(int t=0; t<typ.types; t++){
    r = typ.c_rad[t]*scale;
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVertexBuffer[t]); 
    vertex = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    
    int count=0;
    for(int i=0; i<slices; i++){
      vertex[count] = normal[count]*r;
      vertex[count+1] = normal[count+1]*r;
      vertex[count+2] = 1.0f;
      count+=3;
    }
    for(int j=1; j<stacks-2; j++){
      height = 1.0f - dheight*(float)j;
      for(int i=0; i<slices; i++){
	vertex[count] = normal[count]*r;
	vertex[count+1] = normal[count+1]*r;
	vertex[count+2] = height;
	count+=3;
      }
    }
    for(int i=0; i<slices; i++){
      vertex[count] = normal[count]*r;
      vertex[count+1] = normal[count+1]*r;
      vertex[count+2] = 0.0f;
      count+=3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Face Buffer of Cylinder
//------------------------------------------------------------
void 
ACVO::setCylinderFaces(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderFaceBuffer); //face
  GLuint *face = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

  int count=0;
  for(int j=1;j<=stacks-2;j++){
    for(int i=0;i<slices;i++){
      face[count++] = slices*(j-1) + (i+1)%slices;
      face[count++] = slices*(j-1) + i;
      face[count++] = slices*j + i;

      face[count++] = slices*j + i;
      face[count++] = slices*j + (i+1)%slices;
      face[count++] = slices*(j-1) + (i+1)%slices;
    }
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Color Buffer of Cylinder
//------------------------------------------------------------
void 
ACVO::setCylinderColors(const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices;

  GLfloat *color;
  for(int t=0; t<typ.types ; t++){
    glBindBuffer(GL_ARRAY_BUFFER, cylinderColorBuffer[t]); 
    color = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    
    for(int j=0;j<vertices*4;j+=4){
      color[j]=typ.color[t][0];
      color[j+1]=typ.color[t][1];
      color[j+2]=typ.color[t][2];
      color[j+3]=typ.color[t][3];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//------------------------------------------------------------
//       Draw Cylinder
//------------------------------------------------------------
void 
ACVO::Cylinder(const int type,const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int faces = 3*(stacks-1)*slices;

  glBindBuffer(GL_ARRAY_BUFFER, cylinderVertexBuffer[type]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, cylinderNormalBuffer);
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, cylinderColorBuffer[type]);
  glColorPointer(4, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderFaceBuffer);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawElements(GL_TRIANGLES, faces, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
}

//------------------------------------------------------------
//       Set Text
//------------------------------------------------------------
void 
ACVO::resetText(){
  glDeleteLists(TextList,3);
  TextList = glGenLists(3);
}

void 
ACVO::setText(const int list_num,  const char *string){
  int length = strlen(string);
  glNewList(list_num+TextList,GL_COMPILE);
  glRasterPos2f(5.0f, 85.0f + (GLfloat)list_num*5.0f);
  for (int i = 0; i < length; i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
  }
  glEndList();
}

//------------------------------------------------------------
//       Draw Text
//------------------------------------------------------------
void 
ACVO::drawText(int list_num){
  glPushAttrib(GL_ENABLE_BIT);
  glMatrixMode(GL_PROJECTION);

  glPushMatrix();
     glLoadIdentity();
     gluOrtho2D(0, 100, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
           glLoadIdentity();
	   glColor3f(1.0, 1.0, 0.0);
	   for(int i=2;i>=4-list_num*2;i--)
	     glCallList(i+TextList);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
  glPopAttrib();
  glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------
//       Generate and Delete Cell Box
//------------------------------------------------------------
void 
ACVO::resetCellBox(void){
  delCellBox();
  setCellBox();
}

//------------------------------------------------------------
//       Generate and Delete Cell Box
//------------------------------------------------------------
void 
ACVO::genCellBox(void){
  const int mesh = 20;
  const int slices = mesh, stacks = mesh;
  const int svertices = (stacks - 1)*slices + 2;
  const int sfaces = 2*(stacks-1)*slices;
  const int cvertices = (stacks - 1)*slices;
  const int cfaces = 2*(stacks-2)*slices;

  //---------- Cell Box ---------
  glGenBuffers(2, boxVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[0]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[1]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*3, NULL, GL_STATIC_DRAW);

  glGenBuffers(2, boxNormalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[0]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*3, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[1]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*3, NULL, GL_STATIC_DRAW);

  glGenBuffers(2, boxFaceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[0]); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*sfaces*3, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[1]); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cfaces*3, NULL, GL_STATIC_DRAW);

  glGenBuffers(2, boxColorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[0]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*svertices*4, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[1]); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cvertices*4, NULL, GL_STATIC_DRAW);
  
  BoxList = glGenLists(3);
  //BoxList = 1;
} 

//------------------------------------------------------------
void 
ACVO::delCellBox(){
  glDeleteBuffers(2, boxVertexBuffer);
  glDeleteBuffers(2, boxNormalBuffer);
  glDeleteBuffers(2, boxFaceBuffer);
  glDeleteBuffers(2, boxColorBuffer);
  glDeleteLists(BoxList,3);
}

//------------------------------------------------------------
//       Create Buffer Objects of System Box
//------------------------------------------------------------
void 
ACVO::setCellBox(void){
  genCellBox();
  typ_box.Init(sys.TYPE_MAX);
  const int mesh = sys.boxDetail;
  typ_box.types = 1;
  for(int i=0;i<4;i++){
    typ_box.color[0][i] = sys.boxColor[i];
  }
  typ_box.rad[0] = sys.boxWidth;
  typ_box.c_rad[0] = sys.boxWidth;
  
  setBoxSphereNormals(mesh);
  setBoxSphereVertices(1, mesh);
  setBoxSphereFaces(mesh);
  setBoxSphereColors(mesh);
  setBoxCylinderNormals(mesh);
  setBoxCylinderVertices(1, mesh);
  setBoxCylinderFaces(mesh);
  setBoxCylinderColors(mesh);

  float dx =  fabs(sys.boxEnd[0]-sys.boxStart[0]);
  float dy =  fabs(sys.boxEnd[1]-sys.boxStart[1]);
  float dz =  fabs(sys.boxEnd[2]-sys.boxStart[2]);
  glNewList(BoxList,GL_COMPILE);
    glPushMatrix();
      glScalef(1.0,1.0,dx);
      boxCylinder(mesh);
    glPopMatrix();
  glEndList();
  glNewList(BoxList+1,GL_COMPILE);
    glPushMatrix();
      glScalef(1.0,1.0,dy);
      boxCylinder(mesh);
    glPopMatrix();
  glEndList();
  glNewList(BoxList+2,GL_COMPILE);
    glPushMatrix();
      glScalef(1.0,1.0,dz);
      boxCylinder(mesh);
    glPopMatrix();
  glEndList();

}

//------------------------------------------------------------
//       Draw System Box
//------------------------------------------------------------
void 
ACVO::drawCellBox(void){
  const int mesh = sys.boxDetail;
  float dx =  fabs(sys.boxEnd[0]-sys.boxStart[0]);
  float dy =  fabs(sys.boxEnd[1]-sys.boxStart[1]);
  float dz =  fabs(sys.boxEnd[2]-sys.boxStart[2]);
  
  glPushMatrix();
    glTranslatef(sys.boxStart[0]-sys.systemCenter[0],sys.boxStart[1]-sys.systemCenter[1],sys.boxStart[2]-sys.systemCenter[2]);
    
    boxSphere(mesh);
    glCallList(BoxList+2);
    glPushMatrix();
      glTranslatef( 0.0, 0.0, dz);
      boxSphere(mesh);
      glRotatef(90, 0.0, 1.0, 0.0);
      glCallList(BoxList);
      glRotatef(-90, 1.0, 0.0, 0.0);
      glCallList(BoxList+1);
    glPopMatrix();
  
    glTranslatef(dx, 0.0, 0.0);
    boxSphere(mesh);
    glCallList(BoxList+2);
    glPushMatrix();
      glRotatef(-90, 0.0, 1.0, 0.0);
      glCallList(BoxList);
      glRotatef(-90, 1.0, 0.0, 0.0);
      glCallList(BoxList+1);
    glPopMatrix();
    glPushMatrix();
      glTranslatef( 0.0, 0.0, dz);
      boxSphere(mesh);
    glPopMatrix();

    glTranslatef( 0.0, dy, 0.0);
    boxSphere(mesh);
    glCallList(BoxList+2);
    glPushMatrix();
      glTranslatef( 0.0, 0.0, dz);
      boxSphere(mesh);
      glRotatef(-90, 0.0, 1.0, 0.0);
      glCallList(BoxList);
      glRotatef(90, 1.0, 0.0, 0.0);
      glCallList(BoxList+1);
    glPopMatrix();
    
    glTranslatef( -dx, 0.0, 0.0);
    boxSphere(mesh);
    glCallList(BoxList+2);
    glPushMatrix();
      glRotatef(90, 0.0, 1.0, 0.0);
      glCallList(BoxList);
      glRotatef(90, 1.0, 0.0, 0.0);
      glCallList(BoxList+1);
    glPopMatrix();
    glPushMatrix();
      glTranslatef( 0.0, 0.0, dz);
      boxSphere(mesh);
    glPopMatrix();

  glPopMatrix();

}

//------------------------------------------------------------
//       Draw Corner Sphere of System Box
//------------------------------------------------------------
void
ACVO::boxSphere(const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int faces = 2*(stacks-1)*slices*3;
  
  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[0]);
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[0]);
  glColorPointer(4, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[0]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawElements(GL_TRIANGLES, faces, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
}

//------------------------------------------------------------
//       Draw Side Cylinder of System Box
//------------------------------------------------------------
void 
ACVO::boxCylinder(const int mesh){

  const int slices = mesh, stacks = mesh/2;
  const int faces = 2*(stacks-2)*slices*3;

  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[1]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[1]);
  glNormalPointer(GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[1]);
  glColorPointer(4, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[1]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawElements(GL_TRIANGLES, faces, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
}

//------------------------------------------------------------
//       Set Normal Buffer of Box Sphere
//------------------------------------------------------------
void 
ACVO::setBoxSphereNormals(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  float theta, phi;
  const float dtheta = 3.121593f/(float)stacks;
  const float dphi = 2.0f*3.141593f/(float)slices;

  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[0]);
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  normal[count++] = 0.0f;
  normal[count++] = 0.0f;
  normal[count++] = 1.0f;
  for(int j=0; j<stacks-1; j++){
    theta = dtheta*(j+1);
    for(int i=0; i<slices; i++){
      phi = dphi*(float)i;
      normal[count++] = cos(phi)*sin(theta);
      normal[count++] = sin(phi)*sin(theta);
      normal[count++] = cos(theta);
    }
  }
  normal[count++] = 0.0f;
  normal[count++] = 0.0f;
  normal[count++] = -1.0f;
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Vertex Buffer of Box Sphere
//------------------------------------------------------------
void 
ACVO::setBoxSphereVertices(const float scale, const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices + 2;

  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[0]); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_ARRAY_BUFFER);
 
  GLfloat *vertex, r;
  r = typ_box.rad[0]*scale;
  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[0]); 
  vertex = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  for(int j=0;j<vertices*3;j+=3){
    vertex[j] = normal[j]*r;
    vertex[j+1] = normal[j+1]*r;
    vertex[j+2] = normal[j+2]*r;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
}

//------------------------------------------------------------
//       Set Face Buffer of Box Sphere
//------------------------------------------------------------
void 
ACVO::setBoxSphereFaces(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[0]); //face
  GLuint *face = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

  int count=0;
  for(int i=1; i<=slices; i++){
    face[count++] = 0;
    face[count++] = i;
    face[count++] = i%slices + 1;
  }

  for(int j=1;j<=stacks-2;j++){
    for(int i=1;i<=slices;i++){
      face[count++] = slices*(j-1) + i%slices+1;
      face[count++] = slices*(j-1) + i;
      face[count++] = slices*j + i;

      face[count++] = slices*j + i;
      face[count++] = slices*j + i%slices+1;
      face[count++] = slices*(j-1) + i%slices+1;
    }
  }

  int a1=slices*(stacks-1), a2=slices*(stacks-2);
  for(int i=1; i<=slices; i++){
    face[count++] = a2+i%slices+1;
    face[count++] = a2+i;
    face[count++] = a1+1;
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Color Buffer of Box Sphere
//------------------------------------------------------------
void 
ACVO::setBoxSphereColors(const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices + 2;

  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[0]); 
  GLfloat *color = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  for(int j=0;j<vertices*4;j+=4){
    color[j]=typ_box.color[0][0];
    color[j+1]=typ_box.color[0][1];
    color[j+2]=typ_box.color[0][2];
    color[j+3]=typ_box.color[0][3];
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//------------------------------------------------------------
//       Set Normal Buffer of Box Cylinder
//------------------------------------------------------------
void 
ACVO::setBoxCylinderNormals(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  float phi;
  const float dphi = 2.0f*3.141593f/(float)slices;

  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[1]); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  for(int j=0; j<stacks-1; j++){
    for(int i=0; i<slices; i++){
      phi = dphi*(float)i;
      normal[count++] = cos(phi);
      normal[count++] = sin(phi);
      normal[count++] = 0.0f;
    }
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------
//       Set Vertex Buffer of Box Cylinder
//------------------------------------------------------------
void 
ACVO::setBoxCylinderVertices( const float scale, const int mesh){
  const int slices = mesh, stacks = mesh/2;
    
  glBindBuffer(GL_ARRAY_BUFFER, boxNormalBuffer[1]); 
  GLfloat *normal = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_ARRAY_BUFFER);

  GLfloat *vertex, r, height;
  const float dheight = 1.0f/(float)(stacks);

  r = typ_box.c_rad[0]*scale;
  glBindBuffer(GL_ARRAY_BUFFER, boxVertexBuffer[1]); 
  vertex = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  int count=0;
  for(int i=0; i<slices; i++){
    vertex[count] = normal[count]*r;
    vertex[count+1] = normal[count+1]*r;
    vertex[count+2] = 1.0f;
    count+=3;
  }
  for(int j=1; j<stacks-2; j++){
    height = 1.0f - dheight*(float)j;
    for(int i=0; i<slices; i++){
      vertex[count] = normal[count]*r;
      vertex[count+1] = normal[count+1]*r;
      vertex[count+2] = height;
      count+=3;
    }
  }
  for(int i=0; i<slices; i++){
    vertex[count] = normal[count]*r;
    vertex[count+1] = normal[count+1]*r;
    vertex[count+2] = 0.0f;
    count+=3;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Face Buffer of Box Cylinder
//------------------------------------------------------------
void 
ACVO::setBoxCylinderFaces(const int mesh){
  const int slices = mesh, stacks = mesh/2;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxFaceBuffer[1]); //face
  GLuint *face = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

  int count=0;
  for(int j=1;j<=stacks-2;j++){
    for(int i=0;i<slices;i++){
      face[count++] = slices*(j-1) + (i+1)%slices;
      face[count++] = slices*(j-1) + i;
      face[count++] = slices*j + i;

      face[count++] = slices*j + i;
      face[count++] = slices*j + (i+1)%slices;
      face[count++] = slices*(j-1) + (i+1)%slices;
    }
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------
//       Set Color Buffer of Box Cylinder
//------------------------------------------------------------
void 
ACVO::setBoxCylinderColors(const int mesh){
  const int slices = mesh, stacks = mesh/2;
  const int vertices = (stacks - 1)*slices;

  GLfloat *color;
  glBindBuffer(GL_ARRAY_BUFFER, boxColorBuffer[1]); 
  color = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  
  for(int j=0;j<vertices*4;j+=4){
    color[j]=typ_box.color[0][0];
    color[j+1]=typ_box.color[0][1];
    color[j+2]=typ_box.color[0][2];
    color[j+3]=typ_box.color[0][3];
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//------------------------------------------------------------
//       Draw Point Sprite
//------------------------------------------------------------
void 
ACVO::initPointSprite330(){
  shd = new ACVShader();
  pointsprite = shd->loadShader();
  positionLoc = glGetAttribLocation(pointsprite,"position");
  colorLoc = glGetAttribLocation(pointsprite,"color");
  viewMatrixLoc = glGetUniformLocation(pointsprite,"viewMatrix");
  projMatrixLoc = glGetUniformLocation(pointsprite,"projMatrix");
  lightLoc = glGetUniformLocation(pointsprite,"LightSource");
  glUniform4fv(lightLoc,1,sys.lightPosition);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glUseProgram(pointsprite);
}

void 
ACVO::initPointSprite120(){
  shd = new ACVShader();
  pointsprite = shd->loadShader();
  //sizeLoc = glGetUniformLocation(pointsprite,"size");
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glEnable(GL_POINT_SPRITE);
  glUseProgram(pointsprite);
}

//------------------------------------------------------------
void 
ACVO::drawPointSprite330(Particle *P, System sys){
  GLfloat pm[16],vm[16];
  glGetFloatv(GL_MODELVIEW_MATRIX,vm);
  glUniformMatrix4fv(viewMatrixLoc, 1, false,vm);
  glGetFloatv(GL_PROJECTION_MATRIX,pm);
  glUniformMatrix4fv(projMatrixLoc, 1, false,pm);
  

  const GLint p_num = sys.numberParticle;
  GLfloat *vert = new GLfloat[p_num*4];
  GLfloat *col = new GLfloat[p_num*4];
  int num = 0;
  for(int i=0;i<p_num;i++){
    const int type = P[i].type;
    if(typ.visible[type]==0)continue;
    vert[num*4  ] = P[i].x[X];
    vert[num*4+1] = P[i].x[Y];
    vert[num*4+2] = P[i].x[Z];
    vert[num*4+3] = typ.rad[type]*sys.scale*2000.0f;
    col[num*4  ] = typ.color[type][0];
    col[num*4+1] = typ.color[type][1];
    col[num*4+2] = typ.color[type][2];
    col[num*4+3] = typ.color[type][3];
    num++;
  }
  glEnableVertexAttribArray(positionLoc);
  glEnableVertexAttribArray(colorLoc);
  glVertexAttribPointer(positionLoc,4,GL_FLOAT,0,0,vert);
  glVertexAttribPointer(colorLoc,4,GL_FLOAT,0,0,col);
  glDrawArrays(GL_POINTS,0,num);
  glDisableVertexAttribArray(positionLoc);
  glDisableVertexAttribArray(colorLoc);
  delete []vert;
  delete []col;
}

void 
ACVO::drawPointSprite120(Particle *P, System sys){
  const GLint p_num = sys.numberParticle;
  GLint num=0;
  GLfloat *vert = new GLfloat[p_num*3];
  GLfloat *col = new GLfloat[p_num*4];
  for(int i=0;i<p_num;i++){
    const int type = P[i].type;
    if(typ.visible[type]==0)continue;
    vert[num*3  ] = P[i].x[X];
    vert[num*3+1] = P[i].x[Y];
    vert[num*3+2] = P[i].x[Z];
    col[num*4  ] = typ.color[type][0];
    col[num*4+1] = typ.color[type][1];
    col[num*4+2] = typ.color[type][2];
    col[num*4+3] = typ.rad[type]*sys.scale*2000.0f;
    num++;
  }
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,vert);
  glColorPointer(4,GL_FLOAT,0,col);
  glDrawArrays(GL_POINTS,0,num);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  delete []vert;
  delete []col;
}

//------------------------------------------------------------
//       Draw Particle Texture
//------------------------------------------------------------
void 
ACVO::initParticleTexture(){
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sphere_width, sphere_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, sphere_raw);
  glAlphaFunc(GL_GREATER, 0.5);
}

//------------------------------------------------------------
void 
ACVO::drawParticleTexture(Particle *P, System sys,GLfloat *rt, Type typ){
  const GLint p_num = sys.numberParticle;
  GLfloat imv[16];
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      imv[i*4+j] = rt[j*4+i];
  }}
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_TEXTURE_2D);
  for(int i=0;i<p_num;i++){
    if( typ.visible[P[i].type] == 0) continue;
    glColor4fv(typ.color[P[i].type]);
    glPushMatrix();
    glTranslated(P[i].x[X],P[i].x[Y],P[i].x[Z]);
    glMultMatrixf(imv);
    const float r = typ.rad[P[i].type]*sys.scale;
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f,0.0f); glVertex3f(0.0f,-r,-r);
    glTexCoord2f(0.0f,1.0f); glVertex3f(0.0f,r,-r);
    glTexCoord2f(1.0f,1.0f); glVertex3f(0.0f,r,r);
    glTexCoord2f(1.0f,0.0f); glVertex3f(0.0f,-r,r);
    glEnd();
    glPopMatrix();
  }
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_ALPHA_TEST);
}


