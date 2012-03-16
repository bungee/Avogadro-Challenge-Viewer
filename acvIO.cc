#include "acvIO.h"

#define BUFFER_LENGTH 256

//------------------------------------------------------------
//  Option Parser
//------------------------------------------------------------
void 
Option::parseOptions(const int argc,char **argv, int &numfiles,System &sys, Type &typ){
  std::string arg;

  std::string param="";
  if(argc>2)
    param=std::string(argv[1]);

  File io;
  numfiles=0;
#ifndef DEMO
  if(argc==1){
    help();
    exit(1);
  }
#endif
  for(int i=1; i<argc; i++){
    arg = std::string( argv[i] );
    std::string::size_type opt = arg.find("-");
    if(opt!=0){
      numfiles++;
      continue;
    }
    if(arg.find("c") != std::string::npos){
      param = std::string(argv[++i]);
      continue;
    }
    setOptions(arg.substr(opt+1), sys,typ);
  }
#ifndef DEMO
  sys.TYPE_MAX = io.MaxNumberofTypes(param);
#endif
  typ.Init(sys.TYPE_MAX);
  io.loadParameters(sys,typ, param);

}

//------------------------------------------------------------
//  Set Option values
//------------------------------------------------------------
void 
Option::setOptions(std::string opt, System &sys, Type &typ){
  std::string value;
  File io;
  if(opt.find("-generate") != std::string::npos){
    io.saveParameter(sys,typ);
    std::cout<<"Generate default parameter file : \"system_information.cfg\" \n";
    std::cout<<"  1) rename and edit parameter file.\n";
    std::cout<<"     ex) $ mv system_information.cfg param.cfg\n";
    std::cout<<"  2) execute following command.\n";
    std::cout<<"     ex) $ ./acv -c param.cfg data00001.dat data00002.dat ..."<<std::endl;
  }else if(opt.find("-help") != std::string::npos){
    help();
    exit(1);
  }else if(opt.find("-version") != std::string::npos){
    std::cout<<"Version "<< _VERSION_ <<std::endl;
  }else if(opt.find("2D") != std::string::npos){
    sys.ortho = 1;
    sys.quatanion[0] = 0.5;
    sys.quatanion[1] = 0.5;
    sys.quatanion[2] = 0.5;
    sys.quatanion[3] = 0.5;
  }else if(opt.find("multi") != std::string::npos){
    sys.multi = 1;
  }else if(opt.find("stereo") != std::string::npos){
    sys.stereo = 1;
  }else if(opt.find("d") != std::string::npos){
    value = opt.substr(opt.find("d")+1, opt.size());
    sys.detail = atoi(value.c_str());
    if(sys.detail < 6 ) sys.detail = 6;
  }else if(opt.find("h") != std::string::npos){
    value = opt.substr(opt.find("h")+1, opt.size());
    sys.windowSize[1] = atoi(value.c_str());
  }else if(opt.find("j") != std::string::npos){
    sys.ortho = 1;
  }else if(opt.find("l") != std::string::npos){
    value = opt.substr(opt.find("l")+1, opt.size());
    sys.distanceX =(GLfloat) atof(value.c_str());
  }else if(opt.find("m") != std::string::npos){
    value = opt.substr(opt.find("m")+1, opt.size());
    sys.numberParticle = atoi(value.c_str());
  }else if(opt.find("p") != std::string::npos){
    typ.visibleReverseAll();
  }else if(opt.find("r") != std::string::npos){
    value = opt.substr(opt.find("r")+1, opt.size());
    for(int i=0;i<typ.types;i++){
      typ.rad[i] = (GLfloat)atof(value.c_str());
      typ.c_rad[i] = (GLfloat)atof(value.c_str());
    }
  }else if(opt.find("w") != std::string::npos){
    value = opt.substr(opt.find("w")+1, opt.size());
    sys.windowSize[0] = atoi(value.c_str());
  }else if(opt.find("x") != std::string::npos){
    value = opt.substr(opt.find("x")+1, opt.size());
    sys.windowPosition[0] = atoi(value.c_str());
  }else if(opt.find("y") != std::string::npos){
    value = opt.substr(opt.find("y")+1, opt.size());
    sys.windowPosition[1] = atoi(value.c_str());
  }else if(opt.find("f") != std::string::npos){
    value = opt.substr(opt.find("f")+1, opt.size());
    sys.fps = atoi(value.c_str());
  }else if(opt.find("v") != std::string::npos){
    value = opt.substr(opt.find("v")+1, opt.size());
    int direction = atoi(value.c_str());
    sys.ConeDirection[direction]=1;
    sys.ConeDisplay=true;
  }
}

//------------------------------------------------------------
//  Display Help Information
//------------------------------------------------------------
void 
Option::help(void)
{
  std::cout<<"Usage: acv [options] file...  \n";
  std::cout<<"Options  \n";
  std::cout<<"    --generate \t Generate default parameter file (system_information.cfg). \n";    
  std::cout<<"    --version \t Display ACV version information. \n";    
  std::cout<<"    --help \t Display this information. \n";    
  std::cout<<"    -w<num>  \t <num>:window width (default 640) \n";
  std::cout<<"    -h<num>  \t <num>:window height (default 480) \n";
  std::cout<<"    -x<num>  \t <num>:horizontal position (default 100) \n";
  std::cout<<"    -y<num>  \t <num>:vertical position (default 100) \n";
  std::cout<<"    -d<num>  \t <num>:polygon mesh (default 15) \n";
  std::cout<<"    -l<num>  \t <num>:eye position (default auto) \n";
  std::cout<<"    -r<num>  \t <num>:particle radius (default 0.7) \n";
  std::cout<<"    -c <file>\t <file>:visualization information file \n";    
  std::cout<<"    -m<num> \t <num>:maximul number of particles (default 1000000) \n";    
  std::cout<<"    -f<num>  \t <num>: FPS for movie (default 30) \n";
  std::cout<<"    -p    \t position preview mode. \n";    
  std::cout<<"    -j    \t orthogonal projection mode. \n"; 
  std::cout<<"    -2D   \t XY-plane mode \n"; 
  std::cout<<"    -multi \t multi drawing mode \n"; 
  std::cout<<std::endl;
}

//------------------------------------------------------------
File::File(void){
  numfiles=0;
  currentfile=0;
  file_num_length=0;
}



//------------------------------------------------------------
//   Analyze File Name Format
//------------------------------------------------------------
void 
File::setFilenameFormat(std::string filename){
  file_ext = filename.substr(filename.rfind(".")+1);
  std::string name = filename.substr(0, filename.rfind("."));
  char fn[name.length()];
  strcpy(fn,name.c_str());
  file_num_length=0;
  int num = name.length()-1;
  while( isdigit(fn[num]) ){
    file_num_length++;
    num--;
  }
  file_head = name.substr(0,num+1);
  if( num <  (int)name.length() - 1){
    currentfile =atoi(name.substr(num+1).c_str()); 
  }else{
    currentfile = 0;
  }
}


//------------------------------------------------------------
//   Get File Name
//------------------------------------------------------------
std::string 
File::getCurrentFilename(void){
  std::ostringstream filename;
  if(numfiles>1){
    filename << filenames[currentfile];
  }else{
    filename <<file_head<<std::setfill('0')<<std::setw(file_num_length)<<currentfile<<"."<<file_ext;
  }
  
  return filename.str();
}

std::string 
File::getCurrentFilename(const std::string ext){
  std::ostringstream filename;
  filename <<file_head<<std::setfill('0')<<std::setw(file_num_length)<<currentfile<<"."<<ext;
  
  return filename.str();
}


//------------------------------------------------------------
//   Load Parameter File
//------------------------------------------------------------
void 
File::loadParameters(System &sys, Type &typ, std::string path){
  std::ifstream in;
  std::string line;
  in.open(path.c_str());
  while(getline(in, line)){
    std::string str,name,value;
    std::string::size_type comment = line.find("#");
    if(comment == std::string::npos && line.length()!= 0){
      in.close();
      return;
    }
    while(line.find("=") != std::string::npos){
      str = line.substr(comment+1);
      name = str.substr(0,str.find("="));
      value = str.substr(str.find("=")+1);
      if(value.find("=") != std::string::npos){
	line = value.substr(value.find(","));
	value = value.substr(0, value.find(","));
      }else{
	line = value;
      }
      setParameter(sys, typ,  name, value);
    }
  }
  in.close();
}

//------------------------------------------------------------
//   Max number of types
//------------------------------------------------------------
int
File::MaxNumberofTypes(std::string path){
  int max_num_type = 40;

  std::ifstream in;
  std::string line;
  in.open(path.c_str());
  while(getline(in, line)){
    std::string str,name,value;
    std::string::size_type comment = line.find("#");
    if(comment == std::string::npos && line.length()!= 0){
      in.close();
      return max_num_type;
    }
    while(line.find("=") != std::string::npos){
      str = line.substr(comment+1);
      name = str.substr(0,str.find("="));
      value = str.substr(str.find("=")+1);
      if(value.find("=") != std::string::npos){
	line = value.substr(value.find(","));
	value = value.substr(0, value.find(","));
      }else{
	line = value;
      }

      int type_num=0;
      if(name.find("color")!=std::string::npos){
	type_num = atoi(name.substr(5).c_str())+1;
      }else if(name.find("radius")!=std::string::npos){
	type_num = atoi(name.substr(6).c_str())+1;
      }else if(name=="TYPE_MAX"){
	//type_num = atoi(value.c_str());
	max_num_type = atoi(value.c_str());
	return max_num_type;
      }
      
      if(type_num > max_num_type){
	max_num_type = type_num;
      }
    }
  }
  in.close();
  return max_num_type;
}



//------------------------------------------------------------
//   Analyze Parameter File
//------------------------------------------------------------
void 
File::setParameter(System &sys,Type &typ, std::string name, std::string value){
  std::string vector[4];
  if(name=="windowPosition"){
    splitVector(value, vector);
    for(int i=0;i<2;i++)
      sys.windowPosition[i] = atoi(vector[i].c_str());
  }else if(name=="windowSize"){
    splitVector(value, vector);
    for(int i=0;i<2;i++)
      sys.windowSize[i] = atoi(vector[i].c_str());
  }
  else if(name=="bgColor"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.bgColor[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="specular"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.specular[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="ambient"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.ambient[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="shininess"){
    sys.shininess[0] = (GLfloat)atof(value.c_str());
  }
  else if(name=="lightPosition"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.lightPosition[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="numberParticle"){
    sys.numberParticle = atoi(value.c_str());
  }
  else if(name=="systemSize"){
    splitVector(value, vector);
    for(int i=0;i<3;i++)
      sys.systemSize[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="systemCenter"){
    splitVector(value, vector);
    for(int i=0;i<3;i++)
      sys.systemCenter[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="boxColor"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.boxColor[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="boxStart"){
    splitVector(value, vector);
    for(int i=0;i<3;i++)
      sys.boxStart[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="boxEnd"){
    splitVector(value, vector);
    for(int i=0;i<3;i++)
      sys.boxEnd[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="boxWidth"){
    sys.boxWidth = (GLfloat)atof(value.c_str());
  }
  else if(name=="boxVisible"){
    sys.boxVisible = atoi(value.c_str());
  }
  else if(name=="boxDetail"){
    sys.boxDetail = atoi(value.c_str());
  }
  else if(name=="distanceX"){
    sys.distanceX = (GLfloat)atof(value.c_str());
  }
  else if(name=="distanceY"){
    sys.distanceY = (GLfloat)atof(value.c_str());
  }
  else if(name=="distanceZ"){
    sys.distanceZ = (GLfloat)atof(value.c_str());
  }
  else if(name=="quatanion"){
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      sys.quatanion[i] = (GLfloat)atof(vector[i].c_str());
  }
  else if(name=="axis"){
    splitVector(value, vector);
    for(int i=0;i<3;i++)
      sys.axis[i] =(GLfloat) atof(vector[i].c_str());
  }
  else if(name=="angle"){
    sys.angle = atoi(value.c_str());
  }
  else if(name=="detail"){
    sys.detail = atoi(value.c_str());
  }
  else if(name=="scale"){
    sys.scale = (GLfloat)atof(value.c_str());
  }
  else if(name=="shadow"){
    sys.shadow = atoi(value.c_str());
  }
  else if(name=="fps"){
    sys.fps = atoi(value.c_str());
  }
  else if(name.find("color")!=std::string::npos){
    int type_num;
    type_num = atoi(name.substr(5).c_str());
    splitVector(value, vector);
    for(int i=0;i<4;i++)
      typ.color[type_num][i] =(GLfloat) atof(vector[i].c_str());
    typ.clock[type_num] = 1;
  }
  else if(name.find("radius")!=std::string::npos){
    int type_num;
    type_num = atoi(name.substr(6).c_str());
    typ.rad[type_num] = (GLfloat)atof(value.c_str());
    typ.rlock[type_num] = 1;
  } 
  else if(name.find("visible")!=std::string::npos){
    int type_num;
    type_num = atoi(name.substr(7).c_str());
    typ.visible[type_num] = atoi(value.c_str());
  } 
  else if(name=="format"){
    splitDataColumn(sys, value);
  }
  else if(name=="box_sx"){
    sys.boxVisible=1;
    sys.boxStart[0] = (GLfloat)atof(value.c_str());
  }
  else if(name=="box_sy"){
    sys.boxStart[1] =(GLfloat) atof(value.c_str());
  }
  else if(name=="box_sz"){
    sys.boxStart[2] = (GLfloat)atof(value.c_str());
  }
  else if(name=="box_ex"){
    sys.boxEnd[0] = (GLfloat)atof(value.c_str());
  }
  else if(name=="box_ey"){
    sys.boxEnd[1] = (GLfloat)atof(value.c_str());
  }
  else if(name=="box_ez"){
    sys.boxEnd[2] = (GLfloat)atof(value.c_str());
  }
  else if(name=="box_wt"){
    sys.boxWidth = (GLfloat)atof(value.c_str());
  }
  else if(name.find("c")!=std::string::npos){
    if(isdigit(name.substr(name.find("c")+1).c_str()[0])){
      int type_num = atoi(name.substr(name.find("c")+1).c_str());
      if(typ.clock[type_num]==1) return;
      splitVector(value, vector);
      for(int i=0;i<4;i++)
	typ.color[type_num][i] = (GLfloat)atof(vector[i].c_str());
    }
  }
  else if(name.find("r")!=std::string::npos){
    if(isdigit(name.substr(name.find("r")+1).c_str()[0])){
      int type_num = atoi(name.substr(name.find("r")+1).c_str());
      if(typ.rlock[type_num]==1) return;
      typ.rad[type_num] = (GLfloat)atof(value.c_str());
    }
  }


}


//------------------------------------------------------------
//   Save Parameter File
//------------------------------------------------------------
void 
File::saveParameter(System sys, Type typ){
  std::ofstream out;
  out.open("system_information.cfg");
  out<<"### Window Setting\n";
  out<<"#windowPosition=("<<sys.windowPosition[0]<<","<<sys.windowPosition[1]<<")\n";
  out<<"#windowSize=("<<sys.windowSize[0]<<","<<sys.windowSize[1]<<")\n";
  out<<"#bgColor=("<<sys.bgColor[0]<<","<<sys.bgColor[1]<<","<<sys.bgColor[2]<<","<<sys.bgColor[3]<<")\n";
  out<<"\n";

  out<<"### Light Setting\n";
  out<<"#specular=("<<sys.specular[0]<<","<<sys.specular[1]<<","<<sys.specular[2]<<","<<sys.specular[3]<<")\n";
  out<<"#ambient=("<<sys.ambient[0]<<","<<sys.ambient[1]<<","<<sys.ambient[2]<<","<<sys.ambient[3]<<")\n";
  out<<"#shininess="<<sys.shininess[0]<<"\n";
  out<<"#lightPosition=("<<sys.lightPosition[0]<<","<<sys.lightPosition[1]<<","<<sys.lightPosition[2]<<","<<sys.lightPosition[3]<<")\n";
  out<<"\n";

  out<<"### System Setting\n";
  out<<"#numberParticle="<<1000000<<"\n"; 
  out<<"#systemSize=("<<sys.systemSize[0]<<","<<sys.systemSize[1]<<","<<sys.systemSize[2]<<")\n";
  out<<"#systemCenter=("<<sys.systemCenter[0]<<","<<sys.systemCenter[1]<<","<<sys.systemCenter[2]<<")\n";
  out<<"#TYPE_MAX="<<sys.TYPE_MAX<<"\n";
  out<<"\n";

  out<<"### Box Setting\n";
  out<<"#boxColor=("<<sys.boxColor[0]<<","<<sys.boxColor[1]<<","<<sys.boxColor[2]<<","<<sys.boxColor[3]<<")\n";
  out<<"#boxStart=("<<sys.boxStart[0]<<","<<sys.boxStart[1]<<","<<sys.boxStart[2]<<")\n";
  out<<"#boxEnd=("<<sys.boxEnd[0]<<","<<sys.boxEnd[1]<<","<<sys.boxEnd[2]<<")\n";
  out<<"#boxWidth="<<sys.boxWidth <<"\n"; 
  out<<"#boxVisible="<<sys.boxVisible <<"\n"; 
  out<<"\n";

  out<<"### Orientation Setting\n";
  out<<"#distanceX="<<sys.distanceX <<"\n"; 
  out<<"#distanceY="<<sys.distanceY <<"\n"; 
  out<<"#distanceZ="<<sys.distanceZ <<"\n"; 
  out<<"#quatanion=("<<sys.quatanion[0]<<","<<sys.quatanion[1]<<","<<sys.quatanion[2]<<","<<sys.quatanion[3]<<")\n";
  out<<"\n";

  out<<"### Object Setting\n";  
  out<<"#detail="<<sys.detail<<"\n";
  out<<"#scale="<<sys.scale<<"\n";
  out<<"\n";

  out<<"### Povray Setting\n";  
  out<<"#shadow="<<sys.shadow<<"\n";
  out<<"\n";

  out<<"### Movie Setting\n";  
  out<<"#fps="<<sys.fps<<"\n";
  out<<"\n";

  out<<"### Data File Format\n";  
  out<<"#format=";
  std::string format="", splitter = std::string(&sys.splitter);
  for(int i=0; i<sys.columns;i++){
    if(sys.column[0]==i)format+="id" + splitter;
    else if(sys.column[1]==i)format+="kind" + splitter;
    else if(sys.column[2]==i)format+="x" + splitter;
    else if(sys.column[3]==i)format+="y" + splitter;
    else if(sys.column[4]==i)format+="z" + splitter;
    else if(sys.column[5]==i)format+="r" + splitter;
    else if(sys.column[6]==i)format+="vx" + splitter;
    else if(sys.column[7]==i)format+="vy" + splitter;
    else if(sys.column[8]==i)format+="vz" + splitter;
    else if(sys.column[9]==i)format+="T" + splitter;
    else format+="dummy" + splitter;
  }
  format = format.substr(0,format.length() - 1);
  out<<format<<"\n\n";
  
  out<<"### Particle Color & Radius\n";  
  for(int i=0; i<typ.types; i++){
    //if(typ.clock[i]==1){
    out<<"#color"<<i<<"=("<< typ.color[i][0]<<","<< typ.color[i][1]<<","<< typ.color[i][2]<<","<< typ.color[i][3]<<")\n";
    //}
    //if(typ.rlock[i]==1){
    out<<"#radius"<<i<<"="<<typ.rad[i]<<"\n";
    //}
    out<<"#visible"<<i<<"="<<typ.visible[i]<<"\n";
    out<<"\n";
  }
  out.close();
}


//------------------------------------------------------------
//   Convert String to 4-dimensional Vector
//------------------------------------------------------------
void 
File::splitVector(std::string source, std::string vector[4]){
  std::string tmp;
  tmp = source.substr(source.find("(")+1,source.find(")")-source.find("("));
  vector[0] = tmp.substr(0,tmp.find(","));
  tmp = tmp.substr(tmp.find(",")+1);
  vector[1] = tmp.substr(0,tmp.find(","));
  tmp = tmp.substr(tmp.find(",")+1);
  vector[2] = tmp.substr(0,tmp.find(","));
  tmp = tmp.substr(tmp.find(",")+1);
  vector[3] = tmp.substr(0,tmp.find(","));
}

//------------------------------------------------------------
//   Convert String to Data Column
//------------------------------------------------------------
void 
File::splitDataColumn(System &sys, std::string source){

  std::string tmp=source,cname;
  std::string splitter=" "; 
  if ( source.find(",") != std::string::npos){
    splitter = ",";
    sys.splitter = 0x2C; //comma
  }
  int cols = 0;
  for(int i=0;i<sys.columns;i++){
    sys.column[i] = -1;
  }
    
  tmp = tmp.substr(tmp.find_first_not_of(" "));
  tmp = tmp.substr(0,tmp.find_last_not_of(" ")+1);
  while(true){
    if( tmp.find(splitter) == std::string::npos){
      cname = tmp.substr(0,tmp.find_last_not_of(splitter)+1);
      setDataColumn(sys, cname,cols++);
      break;
    }else{
      cname = tmp.substr(0,tmp.find(splitter));
      tmp = tmp.substr(tmp.find(splitter)+1);
      tmp = tmp.substr(tmp.find_first_not_of(" "));
      tmp = tmp.substr(tmp.find_first_not_of(splitter));
      setDataColumn(sys, cname,cols++);
    }
  }

  sys.columns = cols;

}

//------------------------------------------------------------
//   Set Data Column
//------------------------------------------------------------
void 
File::setDataColumn(System &sys, const std::string name, const int col_num){
  if (name == "id")
    sys.column[0] = col_num;
  else if (name == "kind")
    sys.column[1] = col_num;
  else if( name == "x" ) 
    sys.column[2] = col_num;
  else if (name == "y")
    sys.column[3] = col_num;
  else if (name == "z")
    sys.column[4] = col_num;
  else if (name == "r")
    sys.column[5] = col_num;
  else if (name == "vx")
    sys.column[6] = col_num;
  else if (name == "vy")
    sys.column[7] = col_num;
  else if (name == "vz")
    sys.column[8] = col_num;
  else if (name == "T")
    sys.column[9] = col_num;
}


//------------------------------------------------------------
//    Load  Particle Information
//------------------------------------------------------------
bool 
File::loadFile(System &sys, Type &typ, Particle *P, std::string filename){
  int fd = open(filename.c_str(),O_RDONLY);
  if(fd < 0){
    std::cout << " cannot open " << filename<< std::endl;
    close(fd);
    return false;
  }
  
  loadParameters(sys, typ, filename);
  
  float min_x = sizeof(float), min_y = sizeof(float), min_z = sizeof(float);
  float max_x = 0.0, max_y = 0.0, max_z = 0.0;
  float x=0.0f, y=0.0f, z=0.0f;
  
  int num = 0;
  const int col_num = sys.columns;
  std::string str;
  std::string *element;
  element = new std::string[col_num];

  struct stat statbuf;
  fstat(fd, &statbuf);
  
  char *buf,*buf_ini;
  buf = new char[statbuf.st_size+1];
  buf_ini = buf;
  
  int rd = read(fd, buf, statbuf.st_size);
  if(rd < 0){
    std::cout << "Error: cannot read buffer :  "<< filename  << std::endl;
    exit(1);
  }
  close(fd);
  if(buf[statbuf.st_size-1]!='\n')
    buf[statbuf.st_size]='\n';

  char *sub,tmp[BUFFER_LENGTH],tmp2[BUFFER_LENGTH];
  float max_norm=0;
  while((sub=strchr(buf,'\n'))!=NULL || buf-buf_ini <statbuf.st_size){
    *sub='\0';
    sub++;
    strncpy(tmp,buf,sub-buf);
    buf=sub;
    if(strchr(tmp,'#')!=NULL) continue;

    int col=0, len=0;
    for(int i=0;i<BUFFER_LENGTH;i++){
      tmp2[len++]=tmp[i];
      //if(tmp[i]==0x20){
      if(tmp[i]==sys.splitter){
  	if(len > 1){
  	  element[col++] = std::string(tmp2);
  	}
  	len = 0;
      }else if(tmp[i]=='\0'){
	element[col] = std::string(tmp2);
	break;
      }
      if(col == col_num ) break;
    }

    if(sys.column[0]!=-1)
      P[num].id = atoi(element[sys.column[0]].c_str());
    else
      P[num].id = num;

    if(sys.column[1]!=-1)
      P[num].type = atoi(element[sys.column[1]].c_str()) % sys.TYPE_MAX;
    else
      P[num].type = 0;
    
    if(sys.column[2]!=-1)
      x = (float)atof(element[sys.column[2]].c_str());
    else 
      x = 0;
    
    if(sys.column[3]!=-1)
      y = (float)atof(element[sys.column[3]].c_str());
    else 
      y = 0;

    if(sys.column[4]!=-1)
      z = (float)atof(element[sys.column[4]].c_str());
    else 
      z = 0;

    if( x > max_x ) max_x = x;
    else if( x < min_x) min_x = x;
    if( y > max_y ) max_y = y;
    else if( y < min_y) min_y = y;
    if( z > max_z ) max_z = z;
    else if( z < min_z) min_z = z;
    P[num].x[X] = x;
    P[num].x[Y] = y;
    P[num].x[Z] = z;
    
    if(sys.column[5]!=-1)
      P[num].r = (GLfloat)atof(element[sys.column[5]].c_str());
    
    //if(sys.column[6]!=-1 && sys.column[7]!=-1 && sys.column[8]!=- 1){
    if(sys.ConeDisplay){
      P[num].v[X] = (GLfloat)atof(element[sys.column[6]].c_str());
      P[num].v[Y] = (GLfloat)atof(element[sys.column[7]].c_str());
      P[num].v[Z] = (GLfloat)atof(element[sys.column[8]].c_str());
      P[num].norm = sqrt(P[num].v[X]*P[num].v[X] + P[num].v[Y]*P[num].v[Y] + P[num].v[Z]*P[num].v[Z]);
      if(max_norm < P[num].norm) max_norm = P[num].norm;
      if(P[num].norm != 0.0){
	P[num].v[X]/= P[num].norm;
	P[num].v[Y]/= P[num].norm;
	P[num].v[Z]/= P[num].norm;

	float dir = P[num].v[X]*sys.ConeDirection[0]+P[num].v[Y]*sys.ConeDirection[1]+P[num].v[Z]*sys.ConeDirection[2];
	P[num].theta = acosf(dir)*57.2957795130823f;
      }else{
	P[num].theta = 0;
      }
    }
    
    num++;
  }
  buf = buf_ini;
  delete []buf;
  
  if(num==0){
    std::cerr<<filename<<" does not have particle information."<<std::endl;
    return false;
  }
  
  sys.numberParticle = num;
  if(sys.loaded==0){
    sys.systemSize[0] = (float)(max_x - min_x);
    sys.systemSize[1] = (float)(max_y - min_y);
    sys.systemSize[2] = (float)(max_z - min_z);
    sys.systemCenter[0] = (float)((max_x + min_x)*0.5);
    sys.systemCenter[1] = (float)((max_y + min_y)*0.5);
    sys.systemCenter[2] = (float)((max_z + min_z)*0.5);
    sys.loaded = 1;
  }

  for(int i=0;i<num;i++){
    P[i].x[X] -= sys.systemCenter[0];
    P[i].x[Y] -= sys.systemCenter[1];
    P[i].x[Z] -= sys.systemCenter[2];
    if(max_norm != 0)
      P[i].norm /= max_norm;
  }
  /*
  if(sys.columns>=6 && sys.column[1]==-1){
    for(int i=0;i<10;i++){
      typ.color[i][0]=(float)(i)/10.0f;
      typ.color[i][1]=0;
      typ.color[i][2]=((float)(10.0f - i)/10.0f);
      typ.color[i][3]=1.0;
    }
    for(int i=10;i<20;i++){
      typ.color[i][0]=(float)(20.0f-i)/10.0f;
      typ.color[i][1]=(float)(i-10.0f)/10.0f;
      typ.color[i][2]=0.0f;
      typ.color[i][3]=1.0f;
    }

    for(int i=20;i<30;i++){
      typ.color[i][0]=(float)(i-20.0f)/10.0f;
      typ.color[i][1]=(float)(i-20.0f)/10.0f;
      typ.color[i][2]=(float)(30.0f-i)/10.0f;
      typ.color[i][3]=1.0f;
    }
    for(int i=30;i<40;i++){
      typ.color[i][0]=(float)(40.0f-i)/10.0f;
      typ.color[i][1]=1.0f;
      typ.color[i][2]=0.0f;
      typ.color[i][3]=1.0f;
    }
    
  }
  */

  delete []element;
  return true;
}


//------------------------------------------------------------
//    Generate FCC Configuration  for DEMO
//------------------------------------------------------------
void 
File::setFCC_DEMO(System &sys, Type &typ, Particle *P){
  float radius = 1.0;
  float Lx=40, Ly=40, Lz=40, dL = 2;
  int ngx = static_cast<int>(Lx/dL);
  int ngy = static_cast<int>(Ly/dL);
  int ngz = static_cast<int>(Lz/dL);

  int num=0;
  for(int i=0;i<ngx;i++){
    for(int j=0;j<ngy;j++){
      for(int k=0;k<ngz;k++){
	////// (0 , 0 , 0)
	P[num].id = num;
	P[num].type = 0;
	P[num].x[X] = i*dL+radius;
	P[num].x[Y] = j*dL+radius;
	P[num].x[Z] = k*dL+radius;
	P[num].r = radius;
	num++;
	////// (1/2 , 1/2 , 0)
	P[num].id = num;
	P[num].type = 1;
	P[num].x[X] = (i+0.5f)*dL+radius;
	P[num].x[Y] = (j+0.5f)*dL+radius;
	P[num].x[Z] = k*dL+radius;
	P[num].r = radius;

	num++;
	////// (1/2 , 0 , 1/2)
	P[num].id = num;
	P[num].type = 2;
	P[num].x[X] = (i+0.5f)*dL+radius;
	P[num].x[Y] = j*dL+radius;
	P[num].x[Z] = (k+0.5f)*dL+radius;
	P[num].r = radius;

	num++;
	////// ( 0  , 1/2 ,  1/2)
	P[num].id = num;
	P[num].type = 3;
	P[num].x[X] = i*dL+radius;
	P[num].x[Y] = (j+0.5f)*dL+radius;
	P[num].x[Z] = (k+0.5f)*dL+radius;
	P[num].r = radius;

	num++;
      }
    }
  }

  sys.numberParticle = num;  
  sys.systemSize[0] = Lx;
  sys.systemSize[1] = Ly;
  sys.systemSize[2] = Lz;
  sys.systemCenter[0] = Lx*0.5f;
  sys.systemCenter[1] = Ly*0.5f;
  sys.systemCenter[2] = Lz*0.5f;
  sys.boxStart[0] = 0;
  sys.boxStart[1] = 0;
  sys.boxStart[2] = 0;
  sys.boxEnd[0] = Lx;
  sys.boxEnd[1] = Ly;
  sys.boxEnd[2] = Lz;

  for(int i=0;i<num;i++){
    P[i].x[X] -= sys.systemCenter[0];
    P[i].x[Y] -= sys.systemCenter[1];
    P[i].x[Z] -= sys.systemCenter[2];
  }

  sys.setInit();
  saveDemoConfig(sys,P);
}


//------------------------------------------------------------
//    Generate Random Configuration  for DEMO
//------------------------------------------------------------
void 
File::setRandom_DEMO(System &sys, Type &typ,Particle *P){
  float radius = 1.0;
  float Lx=80, Ly=80, Lz=100, dL = 2;
  int ngx = static_cast<int>(Lx/dL);
  int ngy = static_cast<int>(Ly/dL);
  int ngz = static_cast<int>(Lz/dL);
  int num = ngx*ngy*ngz*4;

  srand((unsigned) time(NULL));

  for(int i=0;i<num;i++){
    P[i].id = i;
    P[i].type = 0;
    P[i].x[X] = Lx*static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    P[i].x[Y] = Ly*static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    P[i].x[Z] = Lz*static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    P[i].r = radius;
  }
  

  sys.numberParticle = num;  
  sys.systemSize[0] = Lx;
  sys.systemSize[1] = Ly;
  sys.systemSize[2] = Lz;
  sys.systemCenter[0] = Lx*0.5f;
  sys.systemCenter[1] = Ly*0.5f;
  sys.systemCenter[2] = Lz*0.5f;
  sys.boxStart[0] = 0;
  sys.boxStart[1] = 0;
  sys.boxStart[2] = 0;
  sys.boxEnd[0] = Lx;
  sys.boxEnd[1] = Ly;
  sys.boxEnd[2] = Lz;

  for(int i=0;i<num;i++){
    P[i].x[X] -= sys.systemCenter[0];
    P[i].x[Y] -= sys.systemCenter[1];
    P[i].x[Z] -= sys.systemCenter[2];
  }

  sys.setInit();
  saveDemoConfig(sys,P);
}


//------------------------------------------------------------
//    Save Configuration  for DEMO
//------------------------------------------------------------
void 
File::saveDemoConfig(System sys, Particle *P){
  std::ofstream out;
  out.open("demo_config.dat");
  for(int i=0;i<sys.numberParticle;i++){
    out<<P[i].id<<"  "<<P[i].type<<"  "<<P[i].x[X]+sys.systemCenter[0]<<"  "<<P[i].x[Y]+sys.systemCenter[1]<<"  "<<P[i].x[Z]+sys.systemCenter[2]<<std::endl;
  }
  out.close();
}

//------------------------------------------------------------
//    Save Bitmap
//------------------------------------------------------------
int 
File::saveBitmap(const char* filename,const int width,const  int height){
  int alignmentParam;
  GLubyte* pixel_data;  
  int glByteWidth;
  
  glGetIntegerv( GL_PACK_ALIGNMENT,&alignmentParam);

  if( (width*3)%alignmentParam == 0)
    glByteWidth = width*3;
  else
    glByteWidth = width*3 + alignmentParam - (width*3)%alignmentParam;
  
  pixel_data = new GLubyte[(glByteWidth)*(height)*(sizeof(GLubyte))];
  glReadPixels(0, 0, width,  height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);

  int checkByte = width&3;  /// Check number of bytes (multiples of 4 )
  int writeWidth = width*3 + checkByte;

  //std::ofstream out(filename, std::ios::binary);
  FILE* const out = std::fopen(filename, "w");
  if(out==NULL){
    delete []pixel_data;
    return -1;
  }
  AVI::bmp_writer bmp(out, width, height, writeWidth*height);
  char pixels[writeWidth*height];

  unsigned char zero=0;
  int count=0;
  for(int y=0 ; y < height ; y++ ){
    for(int x=0 ; x <width ; x++ ){
      pixels[count++] = pixel_data[x*3+y*glByteWidth+2];
      pixels[count++] = pixel_data[x*3+y*glByteWidth+1];
      pixels[count++] = pixel_data[x*3+y*glByteWidth ];
    }
    
    for( int j=0;j<checkByte;j++){
      pixels[count++] = zero;
    }
  }

  bmp.write(pixels);
  //out.close();
  std::fclose(out);

  delete []pixel_data;
  return 0;
}

//------------------------------------------------------------
//   Save Pov-Ray Format File
//------------------------------------------------------------
void 
File::savePovRay(const char* fname,Type typ,  System sys, Particle *P,  GLfloat *rt){  
  std::ofstream out;
  out.open(fname);
  out<<"camera{\n";
  out<< " location <"<<-sys.distanceX<<","<<sys.distanceY<<","<<sys.distanceZ<<">\n";
  out<<" right <"<<(float)sys.windowSize[0]/(float)sys.windowSize[1]<<",0,0>\n";
  out<<"angle 35 \n";
  out<<" up <0,1,0>\n";
  out<<" sky <0,0,1>\n";
  out<<" look_at <0.0,"<< sys.distanceY<<","<< sys.distanceZ <<">\n";
  out<<"}\n";
  out<<"light_source {<"<<-sys.lightPosition[0] <<","<<sys.lightPosition[1]<<","<<sys.lightPosition[2]<<"> color rgb<1,1,1>}\n";
  out<<"background {color rgb<"<< sys.bgColor[0]<<","<< sys.bgColor[1]<<","<< sys.bgColor[2]<<">}\n";

  for(int i = 0; i < sys.numberParticle; i++){
    if(typ.visible[P[i].type] == 1 ){
      float d0 = rt[0]*P[i].x[X] + rt[4]*P[i].x[Y] + rt[8]*P[i].x[Z];
      float d1 = rt[1]*P[i].x[X] + rt[5]*P[i].x[Y] + rt[9]*P[i].x[Z];
      float d2 = rt[2]*P[i].x[X] + rt[6]*P[i].x[Y] + rt[10]*P[i].x[Z];
      out<<"sphere{<"<<-d0 <<","<< d1<<","<< d2<<">,"<< sys.scale*typ.rad[P[i].type]<<"\n";
      out<<" texture{\n";
      out<<"  pigment{ color rgb <"<<typ.color[P[i].type][0] <<","<< typ.color[P[i].type][1]<<","<< typ.color[P[i].type][2]<<">}\n";
      out<<"  finish{\n";
      out<<"   diffuse 1.0\n";
      out<<"   ambient 0.4\n";
      out<<"  }\n}\n ";
      if(sys.shadow==0) out<<"no_shadow\n";
      out<<"}\n";
    }
  }
  out.close();  
}


//------------------------------------------------------------
//   Save AVI Movie
//------------------------------------------------------------
MakeAVI::MakeAVI(const std::string _filename)
{
  out = NULL;
  filename = _filename;
}

void MakeAVI::initAVI(const int _width, const int _height, int fps=30)
{
  if(out==NULL) {
#ifdef FFMPEG    
    const char command_format[] = "ffmpeg -i - -y -sameq %s";
    char command[sizeof(command_format)+128];
    sprintf(command, command_format, filename.c_str());
    this->out = popen(command,"w");
#else
    //out.open(filename.c_str());
    this->out = std::fopen(filename.c_str(),"w");
#endif
  }
  
  width = _width;
  height = _height;
  avi = new AVI::avi_writer(out, width, height, fps);
}

void MakeAVI::recAVI(void)
{
  int alignmentParam;
  GLubyte* pixel_data;
  int glByteWidth;
  glGetIntegerv( GL_PACK_ALIGNMENT,&alignmentParam);
  
  if( width*3%alignmentParam == 0)
    glByteWidth = width*3;
  else
    glByteWidth = width*3 + alignmentParam - (width*3)%alignmentParam;
  
  pixel_data = new GLubyte[(glByteWidth)*(height)*(sizeof(GLubyte))];
  glReadPixels(0, 0, width,  height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
  
  char pixels[ width * height * 3];
  int count=0;
  for(int y=0 ; y < height ; y++ ){
    for(int x=0 ; x < width ; x++ ){
      pixels[count++] = pixel_data[x*3+3*y*width+2];
      pixels[count++] = pixel_data[x*3+3*y*width+1];
      pixels[count++] = pixel_data[x*3+3*y*width  ];
    }
  }
  
  avi->write((const char*)pixels);
  delete []pixel_data;
}

void MakeAVI::finishAVI(void)
{
#ifdef FFMPEG
  pclose(out);
#else
  fclose(out);
#endif
}
