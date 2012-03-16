#include "acvconf.h"
#include "acvShader.h"

#ifdef SHADER330
#include "pointsprite330.h"
#elif SHADER120
#include "pointsprite120.h"
#endif


//--------------------------------------------------
void
ACVShader::printShaderInfoLog(GLuint shader)
{
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}
//--------------------------------------------------
void
ACVShader::printProgramInfoLog(GLuint program)
{
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:\n" << infoLog << '\n' << std::endl;
    delete[] infoLog;
  }
}
//--------------------------------------------------
GLuint
ACVShader::loadShader(){

  GLuint svert = glCreateShader(GL_VERTEX_SHADER);
  GLuint sfrag = glCreateShader(GL_FRAGMENT_SHADER);
#ifdef SHADER330  
  const GLchar *vert = (char*)pointsprite330_vert;
  const GLint vert_len = pointsprite330_vert_len;
  glShaderSource(svert, 1, &vert, &vert_len);

  const GLchar *frag = (char*)pointsprite330_frag;
  const GLint frag_len = pointsprite330_frag_len;
  glShaderSource(sfrag, 1, &frag, &frag_len);
#elif SHADER120
  const GLchar *vert = (char*)pointsprite120_vert;
  const GLint vert_len = pointsprite120_vert_len;
  glShaderSource(svert, 1, &vert, &vert_len);

  const GLchar *frag = (char*)pointsprite120_frag;
  const GLint frag_len = pointsprite120_frag_len;
  glShaderSource(sfrag, 1, &frag, &frag_len);
#endif  
  GLint compiled, linked;
  glCompileShader(svert);
  glGetShaderiv(svert,GL_COMPILE_STATUS,&compiled);
  printShaderInfoLog(svert);
  if (compiled == GL_FALSE) {
    std::cerr << "Compile error in vertex shader." << std::endl;
    exit(1);
  }
  
  glCompileShader(sfrag);
  glGetShaderiv(sfrag,GL_COMPILE_STATUS,&compiled);
  printShaderInfoLog(sfrag);
  if (compiled == GL_FALSE) {
    std::cerr << "Compile error in fragment shader." << std::endl;
    exit(1);
  }
  
  GLuint gl2Program = glCreateProgram();
  glAttachShader(gl2Program,svert);
  glAttachShader(gl2Program,sfrag);
  glDeleteShader(svert);
  glDeleteShader(sfrag);
  
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program,GL_LINK_STATUS,&linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    std::cerr << "Link error." << std::endl;
    exit(1);
  }
  
  return gl2Program;
}
