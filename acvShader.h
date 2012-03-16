#ifndef __acvshader__
#define __acvshader__

class ACVShader
{
 public:
  //int readShaderSource(GLuint shader, const char *name);
  void printShaderInfoLog(GLuint shader);
  void printProgramInfoLog(GLuint program);
  GLuint loadShader();
};

#endif
