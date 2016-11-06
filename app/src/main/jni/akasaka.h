#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define HORIZON 640
//#define VERTICAL 150
#define VERTICAL 400
#define COLORSNUMBER 3
#define FOURIER_LEN 128
#define COLOR_DEPTH 255
#define SOBEL 1
#define PREWITT 2
#define ORIGINAL 3
#define GAUSSIAN 3
#define FROM_BOTTOM 1
#define FROM_TOP 2
#define FROM_LEFT 3
#define FROM_RIGHT 4
#define PI 3.14159265
#define FORK_THRESHOLD 30
#define FORK_DIVIDED_NUM_X 4
#define FORK_DIVIDED_NUM_Y 4
#define FORK_EDGE_DETECT_TH 32
#define CORRALATION_THRESHOLD 80
#define CONST_HAI_WIDTH 64
#define CONST_HAI_HEIGHT 96
//#define CONST_HAI_HEIGHT 64
#define LINEDETECTOR_WIDTH 15
#define LINEDETECTOR_ACCEPT 10
#define LINEDETECTOR_DEPTH 32
#define NAKIFLAG 1
#define NOTNAKIFLAG 0
#define NORMAL 0
#define NOEXISTENCE 1
#define EXISTENCE 2
#define NAKIEXISTENCE 3

enum AGARI_MODE{MODE_TEHAI,MODE_DORA,MODE_NAKI};
enum GETPOS_MODE{AGARI_GETPOS,SAMPLE_GETPOS,FIRST_AGARI_GETPOS,FIRST_AGARI_GETPOS_CHECK,FIRST_AGARI_NAKI_GETPOS_CHECK,FIRST_SAMPLE_GETPOS,FIRST_SAMPLE_GETPOS_CHECK,FIRST_SAMPLE_GETPOS_INTERNAL_CHECK};
enum FORK_MODE{SMAPLE_FORK_MODE,AGARI_FORK_MODE};
enum FORK_RESULT{FORK_SUCCESS=0,FORK_FAIL};
enum MATCHING_FILTER_TYPE{OUTSIDE_METHOD,PREWITT_METHOD};
// COMPILE SWITCH //
#define NATIVE_DEBUG
//#define DEBUG_MODE
// COMPILE SWITCH //
#define MAX_DORA 12
#define MAX_TEHAI 14
#define MAX_NAKI 16

#define WANZU_START 0
#define SOUZU_START 9
#define PINZU_START 18
#define JIHAI_START 27
#define NONETARGET -1


enum correlationType{
	CORRALATION_NOTFOUND=0,
    CORRALTION_FOUND,
};
enum findingresult{
	ALL_RESERCHED=0,
	NOT_ALL_RESERCHED
};
enum judgeHai{
	judgedHaku=1,
	judgedTyun,
	judgedOther
};

enum CHECKEDFLAG{ALREADY_CHECKED=0,NOT_ALREADY_CHECKED,TEMPORALILY_CHECKED};
enum FINDING_HAI_TYPE{FIND_PINZU=0,FIND_WANZU,FIND_SOUZU,FIND_JIHAI};


#ifdef  NATIVE_DEBUG
#include <jni.h>
#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "native", __VA_ARGS__)
#define printf(...) LOGV(__VA_ARGS__)
void filterEdge(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int flag);
void filterEdgeJint(unsigned char *in, jint *out, int sz_x, int sz_y,int flag);
#endif

class coord{
public:
	int x;
	int y;
	coord(){
		x=0;
		y=0;
	}

};
class HiStatus{
public:
	coord Pos;
	coord Size;
	int feature1;
	int feature2;
	int num;
	HiStatus(){
		feature1=0;
		feature2=0;
		num=0;
	}
};

class AgariHai{
public:

	class HiStatus DoraArray[MAX_DORA];
	class HiStatus TehaiArray[MAX_TEHAI];
	class HiStatus NakiArray[MAX_NAKI];
	int doraNum;
	int tehaiNum;
	int nakiNum;
	int resultDora[MAX_NAKI+1];
	int resultTehai[MAX_NAKI+1];
	int resultNaki[MAX_NAKI+1];
	AgariHai(){
		doraNum=0;
		tehaiNum=0;
		nakiNum=0;
		for(int i=0;i<MAX_NAKI+1;i++){
			resultDora[i]=HAI_END;
			resultTehai[i]=HAI_END;
			resultTehai[i]=HAI_END;
		}
	}
	void showHaiStatus(){
		printf("FOUND! Dora =%d TEHAI =%d NAKI =%d\n",doraNum,tehaiNum,nakiNum);
		printf("----   DORA ---- \n");
		for(int i=0;i<doraNum;i++){
			printf("pos (%d %d) size (%d %d) signal=%d naki =%d",DoraArray[i].Pos.x,DoraArray[i].Pos.y,DoraArray[i].Size.x,DoraArray[i].Size.y,DoraArray[i].feature1,DoraArray[i].feature2);
		}
		printf("----   TEHAI -----\n");
		for(int i=0;i<tehaiNum;i++){
			printf("pos (%d %d) size (%d %d) signal=%d naki =%d",TehaiArray[i].Pos.x,TehaiArray[i].Pos.y,TehaiArray[i].Size.x,TehaiArray[i].Size.y,TehaiArray[i].feature1,TehaiArray[i].feature2);
		}
		printf("----   NAKI -----\n");
		for(int i=0;i<nakiNum;i++){
			printf("pos (%d %d) size (%d %d) signal=%d naki =%d",NakiArray[i].Pos.x,NakiArray[i].Pos.y,NakiArray[i].Size.x,NakiArray[i].Size.y,NakiArray[i].feature1,NakiArray[i].feature2);
		}



	}
};

const int priorityFindingHai[]={PINZU_1,PINZU_2,PINZU_3,PINZU_4,PINZU_5,PINZU_6,PINZU_7,PINZU_8,PINZU_9,
	WANZU_1,WANZU_2,WANZU_3,WANZU_4,WANZU_5,WANZU_6,WANZU_7,WANZU_8,WANZU_9,
	SOUZU_1,SOUZU_2,SOUZU_3,SOUZU_4,SOUZU_5,SOUZU_6,SOUZU_7,SOUZU_8,SOUZU_9,
	TONN,NANN,SHA,PEI,HAKU,HATSU,TYUNN
};
const int sobelFilter_v[9]={-1,0,1,
						  -2,0,2,
						-1,0,1};
const int sobelFilter_h[9]={-1,-2,-1,
							0,0,0,
							1,2,1};
const int prewittFilter_v[9]={-1,0,1,
							 -1,0,1,
							-1,0,1};
const int prewittFilter_h[9]={-1,-1,-1,
								0,0,0,
								1,1,1};

const int originalFilter_v[9]={0,0,0,
							 -1,0,1,
							0,0,0};
const int originalFilter_h[9]={0,-1,0,
								0,0,0,
								0,1,0};
const int gaussianFilter[9]={1,2,1,
							 1,4,1,
							 1,2,1};

int extractAgariHai(int imgWidth,int imgHeight,unsigned char *orgImg,class HiStatus *HiArray,int flag,int &haiHeight);
int LineDetecotor(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int width,int binaryTh, int acceptableRange);
int extractSampleHai(int imgWidth,int imgHeight,unsigned char *orgImg,class HiStatus *HiArray);
int arrangeHai(int imgWidth,int imgHeight,unsigned char *in,int num,unsigned char *out,class HiStatus *HiArray);
void CleanEdge(unsigned char *inout,int sz_x, int sz_y,int width);
void RotateImg(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float rotaiton);
int forkSystemX(unsigned char *in,int sz_x, int sz_y,int &start_X,int flag);
int forkSystemY(unsigned char *in,int sz_x, int sz_y,int &start_Y,int flag);
void filterNegative(unsigned char *in, unsigned char *out, int sz_x, int sz_y);
int u_wtfile(unsigned char *buf, const char *fn, int SZ);
int make_u_char(float *in_buf, unsigned char *out_buf, int S_X, int S_Y);
void getHorizonLine(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start_y,int width);
void getVerticalLine(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start_x,int width);
int getHaiLength(unsigned char *in,int sz_x, int sz_y,int start,int end);
void binarizeImage( unsigned char *in,unsigned char *out,int size_x,int size_y, int threshold);
void changePsrLne(unsigned char *in, int size,int width);
int getDecisionForHaiLength(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start,int end,int flag);
int getSum(unsigned char *in,  int size);
void showLine(unsigned char *in,  int size);
int getRectanglePower(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag);
float getAverage(unsigned char *in,  int size);
void InitialArray(unsigned char *in, int size);
int ChangeView(unsigned char *fixImg,int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag);
void getHorizonLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y);
void getVerticalLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y);
int u_rdfile(unsigned char *buf, const char *fn, int SZ);
int scaleingHai(int imgWidth,int imgHeight,unsigned char *in,unsigned char *out,class HiStatus Hi);
void matching(int imgWidth,int imgHeight,unsigned char *orgImg,class AgariHai agariArray,unsigned char *sampleImg);
HiStatus getPosition(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y, int delta_x,int delta_y);
void filterBandPass(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int lowThre,int highThre);
void filterHighPass(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int threshold);
void differentialEdgeX(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float threshold,int width,int over );
void differentialEdgeY(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float threshold,int width,int over);
void filterBandPass(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int lowThre,int highThre);
void filterEdge(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int flag);
void filterGaussian(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int flag);
void preImageProcess(int imgWidth,int imgHeight,unsigned char *inImg, unsigned char *outImg);
void preImageProcessForMathing(int imgWidth,int imgHeight,unsigned char *inImg, unsigned char *outImg);
int agariMain(int imgWidth,int imgHeight,unsigned char *preProcessImg, class AgariHai &agariHaiArray,int tehaiPosY,int nakiPosY,int guideWakuY,unsigned char* tehaiHai,unsigned char* nakiHai );
void easyMatching(unsigned char *agariImg,int targetNum,unsigned char *sampleImg,int resultArray[],int nakiFlagArray[]);
int u_wtfileEx(unsigned char *buf,  const char *fn, int SZ);
int testHosei(int testHai[]);
char *getHaiName(int haiNo, char *Name);
