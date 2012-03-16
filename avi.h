#ifndef __AVI_H_
#define __AVI_H_

#include <stdint.h>
#include <cstdio>

namespace AVI
{

  typedef uint8_t BYTE;
  typedef uint16_t WORD;
  typedef uint32_t DWORD;
  typedef int32_t LONG;

  static inline uint32_t fourcc(const char a, const char b, const char c, const char d)
  {
    uint32_t ret;
    char* const ptr = (char*)&ret;
    ptr[0] = a;
    ptr[1] = b;
    ptr[2] = c;
    ptr[3] = d;
    return ret;
  }

  typedef uint32_t FOURCC;

  const uint32_t FOURCC_RIFF = fourcc('R', 'I', 'F', 'F');
  const uint32_t FOURCC_LIST = fourcc('L', 'I', 'S', 'T');
  
  /* form types, list types, and chunk types */
  const uint32_t formtypeAVI = fourcc('A', 'V', 'I', ' ');
  const uint32_t listtypeAVIHEADER = fourcc('h', 'd', 'r', 'l');
  const uint32_t ckidAVIMAINHDR = fourcc('a', 'v', 'i', 'h');
  const uint32_t listtypeSTREAMHEADER = fourcc('s', 't', 'r', 'l');
  const uint32_t ckidSTREAMHEADER = fourcc('s', 't', 'r', 'h');
  const uint32_t ckidSTREAMFORMAT = fourcc('s', 't', 'r', 'f');
  const uint32_t ckidSTREAMHANDLERDATA = fourcc('s', 't', 'r', 'd');
  const uint32_t ckidSTREAMNAME = fourcc('s', 't', 'r', 'n');
  const uint32_t ckid00DB = fourcc('0','0','d','b');
  

  const uint32_t listtypeAVIMOVIE = fourcc('m', 'o', 'v', 'i');
  const uint32_t listtypeAVIRECORD = fourcc('r', 'e', 'c', ' ');
  
  const uint32_t ckidAVIOLDINDEX = fourcc('i', 'd', 'x', '1');
  
  
  struct AVIMAINHEADER
  {
    FOURCC fcc; //FOURCC コードを指定する。値は avih でなければならない。
    DWORD  cb; //構造体のサイズを指定する。最初の 8 バイト分を差し引いた値を指定する。
    DWORD  dwMicroSecPerFrame; //フレーム間の間隔をマイクロ秒単位で指定する。
    DWORD  dwMaxBytesPerSec; //ファイルの概算最大データ レートを指定する。
    DWORD  dwPaddingGranularity; //データのアライメントをバイト単位で指定する。
    DWORD  dwFlags; //フラグの 0 個以上のビットごとの組み合わせ
    DWORD  dwTotalFrames; //ファイル内のデータのフレームの総数を指定する。
    DWORD  dwInitialFrames; //インターリーブされたファイルの開始フレームを指定する。
    DWORD  dwStreams; //ファイル内のストリーム数を指定する。
    DWORD  dwSuggestedBufferSize; //ファイルを読み取るためのバッファ サイズを指定する。
    DWORD  dwWidth; //AVI ファイルの幅を指定する (ピクセル単位)。
    DWORD  dwHeight; //AVI ファイルの高さを指定する (ピクセル単位)。
    DWORD  dwReserved[4]; //予約済み。この配列はゼロに設定する。
    
    AVIMAINHEADER(const DWORD frame_no=0, const DWORD dwWidth=320, const DWORD dwHeight=240) 
      : fcc(ckidAVIMAINHDR),
	cb(sizeof(AVIMAINHEADER) - 8),
	dwMicroSecPerFrame(0), //33366
	dwMaxBytesPerSec(0), //7455232; 
	dwPaddingGranularity(512),
	dwFlags(2064), 
	dwTotalFrames(frame_no),
	dwInitialFrames(0),
	dwStreams(1),
	dwSuggestedBufferSize(0), //230408;
	dwWidth(dwWidth),
	dwHeight(dwHeight)
    { }
  };
  
  struct AVISTREAMHEADER
  {
    
    FOURCC fcc; //FOURCC コードを指定する。値は 'strh' でなければならない。
    DWORD  cb; //構造体のサイズを指定する。最初の 8 バイト分を差し引いた値を指定する。
    FOURCC fccType; //ストリームに含まれるデータのタイプを指定する FOURCC
    FOURCC fccHandler; //特定のデータ ハンドラを示す FOURCC (オプション)
    DWORD  dwFlags; //データ ストリームに対するフラグ
    WORD   wPriority; //ストリーム タイプの優先順位を指定する
    WORD   wLanguage;
    DWORD  dwInitialFrames; //オーディオがビデオからどのくらいスキューされているか
    DWORD  dwScale; //dwRate と共に使って、このストリームが使うタイム スケールを指定する
    DWORD  dwRate; //dwRate を dwScale で割ることにより、フレームレートが求まる
    DWORD  dwStart; //このストリームの開始タイムを指定する
    DWORD  dwLength; //このストリームの長さを指定する
    DWORD  dwSuggestedBufferSize; //このストリームを読み取るために必要なバッファの大きさを指定する
    DWORD  dwQuality; //ストリーム内のデータの品質を0 ～ 10,000 の範囲の値で示す
    DWORD  dwSampleSize; //データの 1 サンプルのサイズを指定する
    struct FRAMESTRUCT{
      short int left;
      short int top;
      short int right;
      short int bottom;
      FRAMESTRUCT(short int left, short int top, short int right, short int bottom)
	: left(left), top(top), right(right), bottom(bottom)
      {}
    } rcFrame;
    
    AVISTREAMHEADER(const DWORD frame_no=0, const DWORD fps=30, const DWORD dwScale = 1000)
      : fcc(ckidSTREAMHEADER),  
	cb(sizeof(AVISTREAMHEADER) - 8),  
	fccType(fourcc('v','i','d','s')), 
	fccHandler(fourcc('D','I','B',' ')), 
	dwFlags(0),  
	wPriority(0),  
	wLanguage(0), 
	dwInitialFrames(0),  
	dwScale(dwScale),  
	dwRate(dwScale*fps),  // fps = dwRate / dwScale
	dwStart(0),  
	dwLength(frame_no),  
	dwSuggestedBufferSize(0), //230408
	dwQuality(0),  
	dwSampleSize(0),
	rcFrame(0,0,0,0)    
    {}
  };
  
  struct BITMAPINFOHEADER
  {
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
    
    BITMAPINFOHEADER(const LONG biWidth=320, const LONG biHeight=240)
      : biSize(sizeof(BITMAPINFOHEADER)), 
	biWidth(biWidth), 
	biHeight(biHeight), 
	biPlanes(1), 
	biBitCount(24), 
	biCompression(0), 
	biSizeImage(0), //230408
	biXPelsPerMeter(0), 
	biYPelsPerMeter(0), 
	biClrUsed(0), 
	biClrImportant(0)
    {}
  };
  
  struct BITMAPHEADER
  {
    BYTE   bfDistinct1; 
    BYTE   bfDistinct2;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
    
    BITMAPHEADER(const DWORD bfSize=(320*240*3+54))
      : bfDistinct1('B'),
	bfDistinct2('M'),
	bfSize(bfSize),
	bfReserved1(0),
	bfReserved2(0),
	bfOffBits(54)
    {} 
  }__attribute__((packed));

  struct RGBQUAD
  {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
    RGBQUAD(const BYTE rgbBlue=105, const BYTE rgbGreen=110, const BYTE rgbRed=100, const BYTE rgbReserved=120)
      : rgbBlue(rgbBlue), rgbGreen(rgbGreen), rgbRed(rgbRed), rgbReserved(rgbReserved)
    {}
  };
  
  struct BITMAPINFO
  {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors; // array length = 1
    BITMAPINFO(const LONG biWidth=320, const LONG biHeight=240)
      : bmiHeader(biWidth, biHeight), bmiColors(105,110,100,120)
    {}
  };
  
  struct STREAMFORMAT
  {
    FOURCC fcc;
    DWORD  cb;
    BITMAPINFO bmi;
    STREAMFORMAT(){};
    STREAMFORMAT(const LONG biWidth, const LONG biHeight)
      : fcc(ckidSTREAMFORMAT), 
	cb(sizeof(STREAMFORMAT) - 8), 
	bmi(biWidth, biHeight)
    {}
  };
  
  //AVI 1.0のインデックス構造体
  struct AVIOLDINDEXENTRY
  {
    DWORD   dwChunkId;
    DWORD   dwFlags;
    DWORD   dwOffset;
    DWORD   dwSize;
  };
  
  struct AVIOLDINDEX
  {
    FOURCC  fcc;
    DWORD   cb;
    AVIOLDINDEXENTRY **aIndex;
  };
  
  struct STREAMHEADERLIST
  {
    FOURCC str_list;
    DWORD listSize;
    FOURCC listType;
    AVISTREAMHEADER strh;
    STREAMFORMAT strf;
    STREAMHEADERLIST(const DWORD frame_no=0, const DWORD width=320, const DWORD height=240, const DWORD fps=30)
      : str_list(FOURCC_LIST),
	listSize(sizeof(STREAMHEADERLIST) - 8), 
	listType(listtypeSTREAMHEADER), 
	strh(frame_no, fps),
	strf(width, height)
    {}
  };
  
  struct AVIHEADERLIST
  {
    FOURCC str_list;
    DWORD listSize;
    FOURCC listType;
    AVIMAINHEADER avih;
    STREAMHEADERLIST strl;
    AVIHEADERLIST(const DWORD frame_no=0, const DWORD width=320, const DWORD height=240, const DWORD fps=30)
      : str_list(FOURCC_LIST),
	listSize(sizeof(AVIHEADERLIST) - 8), 
	listType(listtypeAVIHEADER), 
	avih(frame_no, width, height), 
	strl(frame_no, width, height, fps)
    {}
  };
  
  struct RGB
  {
    union
    {
      unsigned char val[3];
      struct { unsigned char blue, green, red;};
    };
    unsigned char& operator[](std::size_t i) {return val[i];}
    const unsigned char operator[](std::size_t i) const{return val[i]; }
  };

}

namespace AVI
{
  class bmp_writer
  {
  protected:
    FILE* out;
    const DWORD width, height;
    const DWORD frameSize;
    const DWORD fileSize;

  public:
    bmp_writer(FILE* const out,const DWORD width = 320, const DWORD height = 240, const DWORD frameSize = 320*240*3 )
      : out(out), width(width), height(height), frameSize(frameSize), fileSize(frameSize+54)
    {
      const BITMAPHEADER bmhd(fileSize);
      fwrite((const char*)&bmhd,sizeof(BITMAPHEADER),1,out);
      const BITMAPINFOHEADER bmif(width, height);
      fwrite((const char*)&bmif, sizeof(BITMAPINFOHEADER),1,out);
    }

    void write(const RGB* const pixels)
    {
      fwrite((const char*)pixels,1,frameSize,out);
    }
    void write(const char* const pixels)
    {
      fwrite(pixels,1,frameSize,out);
    }

  };

  class avi_writer
  {
  protected:
    FILE* out;
    const DWORD width, height;
    const DWORD fps;
    // frame info
    //const static FOURCC frame_ckID;
    const  FOURCC frame_ckID;
    const DWORD frame_ckSize;
  public:
    avi_writer(FILE* const out, const DWORD width = 320, const DWORD height = 240, const DWORD fps = 30)
      : out(out), width(width), height(height), fps(fps),frame_ckID(ckid00DB), frame_ckSize(width*height*sizeof(RGB))
    {
      // write riff header
      fwrite((const char*)&FOURCC_RIFF, sizeof(FOURCC), 1, out);
      const DWORD zero = 0;
      fwrite((const char*)&zero, sizeof(DWORD), 1, out);
      fwrite((const char*)&formtypeAVI, sizeof(DWORD), 1, out);
      
      // header list
      const AVIHEADERLIST hdrl(0, width, height, fps);
      fwrite((const char*)&hdrl, sizeof(AVIHEADERLIST), 1, out);
      
      // list
      const FOURCC movi_str_list = FOURCC_LIST;
      fwrite((const char*)&movi_str_list, sizeof(FOURCC), 1, out);
      const DWORD movi_listSize = 0;
      fwrite((const char*)&movi_listSize, sizeof(DWORD), 1, out); //とりあえず書き込む
      const FOURCC movi_listType = listtypeAVIMOVIE;
      fwrite((const char*)&movi_listType, sizeof(FOURCC), 1, out);
    }
    void write(const RGB* const pixels)
    {
      fwrite((const char*)&frame_ckID, sizeof(FOURCC), 1, out);
      fwrite((const char*)&frame_ckSize, sizeof(DWORD), 1, out);
      fwrite((const char*)pixels, frame_ckSize, 1, out);
    }
    void write(const char* const pixels)
    {
      fwrite((const char*)&frame_ckID, sizeof(FOURCC), 1, out);
      fwrite((const char*)&frame_ckSize, sizeof(DWORD), 1, out);
      fwrite(pixels, frame_ckSize, 1, out);
    }

  };
  //const FOURCC avi_writer::frame_ckID = ckid00DB;
}

  
#endif
