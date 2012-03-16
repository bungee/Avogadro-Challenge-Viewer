#ifndef _acvio_h_
#define _acvio_h_
#include "acvconf.h"

class Option
{
 private:
  void setOptions(std::string opt, System &sys, Type &typ);  
  void help(void);
 public:
  void parseOptions(const int argc, char **argv, int &num, System &sys, Type &typ);
};
//--------------------------------------------------
class File
{
 private:
  //File parameter
  std::string *filenames;
  int numfiles, currentfile, file_num_length;
  std::string file_head, file_ext;

 public:
  File(void);
  void setNumOfFile(const int num){numfiles = num; }
  int getNumOfFile(void){return numfiles; }
  int getCurrentFileNumber(void){return currentfile; }
  void setCurrentFileNumber(const int num){currentfile = num; }
  std::string* setFileNames(const int num){filenames = new std::string[num]; return filenames;}
  void setFilenameFormat(std::string filename);
  std::string getCurrentFilename(void);
  std::string getCurrentFilename(const std::string ext);
  bool loadFile( System &sys, Type &typ, Particle *P, std::string filename);
  void loadParameters(System &sys, Type &typ, std::string path);
  int MaxNumberofTypes(std::string path);
  void setParameter(System &sys, Type &typ, std::string name, std::string value);
  void saveParameter(System sys, Type typ);
  void splitVector(std::string source, std::string vector[4]);
  void splitDataColumn(System &sys,std::string source);
  void setDataColumn(System &sys, const std::string name, const int col_num);
  void setFCC_DEMO(System &sys, Type &typ,Particle *P);
  void setRandom_DEMO(System &sys, Type &typ, Particle *P);
  void saveDemoConfig(System sys, Particle *P);
  int saveBitmap(const char* filename,const int width,const  int height);
  void savePovRay(const char* fname,Type typ,  System sys, Particle *P, GLfloat *rt);

};
//--------------------------------------------------
class MakeAVI{
 private:
  AVI::avi_writer *avi;
  //std::ofstream out;
  FILE* out;
  std::string filename;
  int fps;
  int width;
  int height;

 public:
  MakeAVI(const std::string filename);
  void initAVI(const int _width,const  int _height, int fps);
  void recAVI(void);
  void finishAVI(void);
};
#endif
