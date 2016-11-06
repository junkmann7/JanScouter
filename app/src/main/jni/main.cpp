#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "akasaka.h"

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
void scalingSize(unsigned char *in,unsigned char *out,int toSizeW,int toSizeH,int fromSizeW,int fromSizeH)
{
	unsigned char **temp;
	
	float x_scale=(float)toSizeW/(float)fromSizeW;
	float y_scale=(float)toSizeH/(float)fromSizeH;
	
	temp=new unsigned char  *[fromSizeW];
	for(int i=0;i<fromSizeW;i++)
			temp [i]=new unsigned char [fromSizeH];
	//printf("From =%d %d To = %d %d Scale=%f %f\n",fromSizeW,fromSizeH,toSizeW,toSizeH,x_scale,y_scale);
	
	int s_xx,s_yy;
	for(int i=0;i<fromSizeH;i++)
	{
		for(int j=0;j<fromSizeW;j++)
		{
			
			//printf("%d %d\n",j,i);
			temp[j][i]=in[j+i*fromSizeW];
		}
	}
	

	for(int i=0;i<toSizeH;i++)
	{
		for(int j=0;j<toSizeW;j++)
		{
			s_xx=(int)(j/(float)x_scale);
			s_yy=(int)(i/(float)y_scale);
			
			if(s_xx<0)
				s_xx=0;
			if(s_yy<0)
				s_yy=0;
			if(s_xx>=fromSizeW)
				s_xx=toSizeW;
			if(s_yy>=fromSizeH)
				s_yy=toSizeH;

			if(s_xx>=0 && s_yy>=0 && s_xx<fromSizeW && s_yy < fromSizeH){
				//printf("%d %d => %d %d signal=%d \n",j,i,s_xx,s_yy,temp[s_xx][s_yy]);
				out[j+i*toSizeW]=temp[s_xx][s_yy];
			}else {
				printf("Error %d	%d\n",s_xx,s_yy);
			}
		}
	}
	
	//printf("sample %d %d\n",fromSizeW,fromSizeH);
	
	for(int i=0;i<fromSizeW;i++)
	{
		//printf("%d ",i);
		delete [] temp[i];
		
	}

	delete [] temp;
	//exit(0);
	
}		
void kaiten(unsigned char *bufmoto,unsigned char *bufout,int SZ_X,float rotation)
{
//http://www.sm.rim.or.jp/~shishido/kaiten.html

	
	

	double s;
	s=(2*PI)/(360)*rotation;

	for(int i=0;i<SZ_X*SZ_X;i++)
		bufout[i]=0;

	
	
		
		int xx,yy;

		for (int i=0;i<SZ_X;i++)
			for(int j=0;j<SZ_X;j++) {

			// 回転後に対応する回転前のピクセルを求める 

			
				xx=((j-SZ_X/2)*cos(-s)-(i-SZ_X/2)*sin(-s)+SZ_X/2);
				yy=(j-SZ_X/2)*sin(-s)+(i-SZ_X/2)*cos(-s)+SZ_X/2;

			// 回転後のピクセルに、対応する回転前のピクセルを描画 
				if (xx>=0 && yy>=0 && xx<SZ_X && yy<SZ_X)
					bufout[j+i*SZ_X]=bufmoto[xx+yy*SZ_X];
			}
	
	
};
void CopyArray(unsigned char *in,unsigned char *out,int sz_x,int sz_y){
	for(int i=0;i<sz_x*sz_y;i++){
		out[i]=in[i];
	}
}
void RotateImg(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float rotaiton){
	unsigned char *org;
	unsigned char *rot;
	
	org=new unsigned char[sz_x*sz_x];
	rot=new unsigned char[sz_x*sz_x];
	
	if(sz_x<sz_y){
		printf("Error:Cannnot RotateImgsize\n");
		exit(0);
	}

	for(int i=0;i<sz_x*sz_x;i++){
		org[i]=0;
		rot[i]=0;
	}
	
	for(int j=0;j<sz_x;j++){
		for(int i=0;i<sz_y;i++){
			org[j+(i+(sz_x-sz_y)/2)*sz_x]=in[j+i*sz_x];
		}
	}

	
	kaiten(org,rot,sz_x,rotaiton);
	//u_wtfile(rot,"org2.raw",sz_x*sz_x);
	for(int j=0;j<sz_x;j++){
		for(int i=0;i<sz_y;i++){
				out[j+i*sz_x]=rot[j+(i+(sz_x-sz_y)/2)*sz_x];
		}
	}
	
	delete [] org;
	delete [] rot;
	
}
void CleanEdge(unsigned char *inout,int sz_x, int sz_y,int width){
	int i,j;
	i=0;
	for(j=0;j<sz_x;j++){
		inout[j+i*sz_x]=0;
	}
	i=sz_y-1;
	for(j=0;j<sz_x;j++){
		inout[j+i*sz_x]=0;
	}
	j=0;
	for(i=0;i<sz_y;i++){
		inout[j+i*sz_x]=0;
	}
	j=sz_x-1;
	for(i=0;i<sz_y;i++){
		//printf("%d,%d\n",j,i);
		inout[j+i*sz_x]=0;
	}
	
}
int u_rdfile(unsigned char *buf, const char *fn, int SZ)
{
	FILE	*fp;
	int ret=0;
	if(NULL== (fp = fopen(fn, "rb")))
	{
		printf(" Read file error!\n\7");
		exit(true);
	}
	ret=fread(buf, sizeof(unsigned char), SZ, fp);
	fclose(fp);

	return (int)true;
}
void getVerticalLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y){
	for(int i=0;i<sz_y;i++){
		out[i]=in[lineNum+i*sz_x];
	}
}
void getHorizonLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y){
	for(int j=0;j<sz_x;j++){
		out[j]=in[j+lineNum*sz_x];
	}
}

int u_wtfile(unsigned char *buf, const char *fn, int SZ)
{
	#ifndef NATIVE_DEBUG
	FILE	*fp;

	if(NULL == (fp = fopen(fn,"wb")))
	{
		printf(" Write file error!%s\n\7",fn);
		exit(true);
	}

	fwrite(buf, sizeof(unsigned char), SZ, fp);

	fclose(fp);
	#endif
	return true;

}
int getSum(unsigned char *in,  int size){
	int getSum=0;
	for(int i=0;i<size;i++){
		getSum+=in[i];
	}
	return getSum;
}
void showLine(unsigned char *in,  int size){
	printf("----show the line\n");
	for(int i=0;i<size;i++){
		printf("%d\n",in[i]);
	}
	printf("----end\n");
}
	
float getAverage(unsigned char *in,  int size){
	int sum=0;
	sum=getSum(in,size);
	return float(sum)/(float)size;
}
void InitialArray(unsigned char *in, int size){
	for(int i=0;i<size;i++){
		in[i]=0;
	}
}
int getImageCheck(unsigned char *in,int input_x,int input_y, int sz_x, int sz_y, int ExceptionValue){
	int temp;
		if(input_y<0 || input_y>= sz_y){
				temp=ExceptionValue;
			printf("Error %d %d\n",input_x,input_y);
			}else if(input_x<0 || input_x>=sz_x){
				temp=ExceptionValue;
			printf("Error %d %d\n",input_x,input_y);
			}else{
				temp=in[input_x+input_y*sz_x];
			}
	return temp;
	
}
void filterNegative(unsigned char *in, unsigned char *out, int sz_x, int sz_y){
	for(int i=0;i<sz_x*sz_y;i++){
		out[i]=255-in[i];
	}
}
void binarizeImage( unsigned char *in,unsigned char *out,int size_x,int size_y, int threshold){
	for(int i=0;i<size_x*size_y;i++){
		if(in[i]>threshold){
			out[i]=255;
		}else{
			out[i]=0;
		}
	}
}

int calcEdge(int x,int y,int sz_x,int sz_y,unsigned char *in,int flag){
	int temp=0;
	int gravitiy_count=0;
	int element_h=0;
	int element_v=0;
	float square_element=0.0;
	int element_edge=0;
	for(int i=y-1;i<=y+1;i++){
		for(int j=x-1;j<=x+1;j++){
			if(i<0 || i>= sz_y){
				temp=0;
			}else if(j<0 || j>=sz_x){
				temp=0;
			}else{
				temp=in[j+i*sz_x];
			}
			if(flag==SOBEL){
				element_v+=sobelFilter_v[gravitiy_count]*temp;
				element_h+=sobelFilter_h[gravitiy_count]*temp;
			}else if(flag==PREWITT){
				
				element_v+=prewittFilter_v[gravitiy_count]*temp;
				element_h+=prewittFilter_h[gravitiy_count]*temp;
			}else if(flag==ORIGINAL){
				element_v+=originalFilter_v[gravitiy_count]*temp;
				element_h+=originalFilter_h[gravitiy_count]*temp;
			}else{
				printf("No filter \n");
				exit(0);
			}
			
				
			gravitiy_count++;
		}
	}
	square_element=element_v*element_v+element_h*element_h;
	element_edge=(int)sqrt(square_element);
	return element_edge;
	
}
void changePsrLne(unsigned char *in, int size,int width){
	if(width==0){
		printf("Error:no meaning\n");
		exit(0);
	}
	int avg=0;
	int psr=0;
	for(int i=0;i<size;i++){
		avg=0;
		for(int k=i-width;k<i+width;k++){
			avg+=getImageCheck(in,k,0,size,1,255);
		}
		avg/=(2*width+1);
		if(avg==0){
			psr=0;
		}else{
			psr=in[i]/avg;
		}
		printf("%d\n",psr);
	}
}

int getHaiLength(unsigned char *in,int sz_x, int sz_y,int start,int end){
	int distanceMAX=0;
	int set_pos=0;
	int distanceTemp=0;
	for(int i=start;i<end;i++){
		if(in[i]>0){
			distanceTemp=i-set_pos;
			set_pos=i;
			//printf("i=%d %d\n",i,distanceTemp);
			if(distanceMAX<distanceTemp){
				distanceMAX=distanceTemp;
				
			}
		}
	}
	
	return distanceMAX;
}
void getHorizonLine(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start_y,int width){
	int lineResult=0;
	int temp=0;
	if(width==0){
		printf("Error:width is 0\n");
		exit(0);
	}
	//printf("-----getHorizon----\n");
	for(int j=0;j<sz_x;j++){
	//	printf("here\n");
		lineResult=0;
		for(int i=start_y;i<start_y+width;i++){
	//		printf("here?\n");
			lineResult+=getImageCheck(in,j,i,sz_x,sz_y,0);
		}
		out[j]=lineResult/width;
	//	printf("%d\n",lineResult/width);
	}
	//printf("---- end -------------\n");
}
void getVerticalLine(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start_x,int width){
	int lineResult=0;
	int temp=0;
	if(width==0){
		printf("Error:width is 0\n");
		exit(0);
	}
	//printf("----- getVertical----\n");
	for(int i=0;i<sz_y;i++){
		//printf("here?%d\n",i);
		lineResult=0;
		for(int j=start_x;j<start_x+width;j++){
			lineResult+=getImageCheck(in,j,i,sz_x,sz_y,0);
		}
		out[i]=lineResult/width;
	//	printf("%d\n",lineResult/(width));
	}
		
		
	
	//printf("---- end -------------\n");
}

void filterEdge(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int flag){
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			out[j+i*sz_x]=calcEdge(j,i,sz_x,sz_y,in,flag);
		}
	}
	
}
void filterEdgeJint(unsigned char *in, jint *out, int sz_x, int sz_y,int flag){
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			out[j+i*sz_x] = (int)calcEdge(j,i,sz_x,sz_y,in,flag);
		}
	}
	
}
int GetPeakLine(unsigned char *in, int sz_x, int sz_y){
	
}
int getDecisionForHaiLength(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start,int end,int flag){
	float min=1000;
	float temp=1000;
	int min_pos=0;
	unsigned char *tempLine;
	if(flag==sz_y){
		tempLine=new unsigned char[sz_y];
	}else{
		tempLine=new unsigned char[sz_x];
	}
	for(int i=start;i<end;i++){
		if(flag==sz_y){
			getVerticalLine(in,tempLine,sz_x,sz_y,i,1);
			temp=getAverage(tempLine,sz_y);
		}else{
			getHorizonLine(in,tempLine,sz_x,sz_y,i,1);
			temp=getAverage(tempLine,sz_x);
		}
		if(min>temp){
			min=temp;
			min_pos=i;
			//printf("%d= %f \n",temp ,i);
			if(flag==sz_y){
				getVerticalLine(in,out,sz_x,sz_y,i,1);
			}else{
				getHorizonLine(in,out,sz_x,sz_y,i,1);
			}
				
		}

	}
	delete [] tempLine;
	return min_pos;
}
int getVerticalFeatureDetector(unsigned char *in,int pos_x,int pos_y,int sz_x, int sz_y,int width, int binaryTh){
	int ImgElement;
	int count=0;
	for(int j=pos_x-width;j<=pos_x+width;j++){
		if(j>=0 && j<sz_x){
			ImgElement=in[j+pos_y*sz_x];
		}else{
			ImgElement=0;
		}
		//printf("%d ",ImgElement);
		if(ImgElement > binaryTh){
			count++;
		}
	}
	return count;
}
int getHorizonFeatureDetector(unsigned char *in,int pos_x,int pos_y,int sz_x, int sz_y,int width, int binaryTh){
	int ImgElement;
	int count=0;
	for(int i=pos_y-width;i<=pos_y+width;i++){
		if(i>=0 && i<sz_y){
			ImgElement=in[pos_x+i*sz_x];
		}else{
			ImgElement=0;
		}
		if(ImgElement > binaryTh){
			count++;
		}
	}
	return count;
}
int getLineFeatureImg(unsigned char *in, unsigned char *out,int sz_x, int sz_y,int width, int binaryTh,int flag){
	//int count=0;
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			if(flag==HORIZON){
				out[j+i*sz_x]=getVerticalFeatureDetector(in,j,i,sz_x,sz_y,width,binaryTh);
			}else{
				out[j+i*sz_x]=getHorizonFeatureDetector(in,j,i,sz_x,sz_y,width,binaryTh);
			}
		}
	}
	
	return 0;
}
	
int getElement(unsigned char *in,int pos_x,int pos_y,int sz_x, int sz_y){
	if(pos_x >= 0 && pos_x < sz_x && pos_y >= 0 && pos_y <sz_y){
		return in[pos_x+pos_y*sz_x];
	}else{
		return 0;
	}
}
int LineDetecotor(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int width,int binaryTh, int acceptableRange){
	unsigned char *verticalFeatureImg;
	unsigned char *horizonFeatureImg;
	verticalFeatureImg = new unsigned char [sz_x*sz_y];
	horizonFeatureImg =  new unsigned char [sz_x*sz_y];
	
		
	int currentFeature=0;
	int preFeature=255;
	int nextFeature=255;
	int preFeature2=255;
	int nextFeature2=255;
	
	getLineFeatureImg(in,horizonFeatureImg,sz_x,sz_y,width,binaryTh,HORIZON);


	InitialArray(out,sz_x*sz_y);
	
	#if 1
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			currentFeature=getElement(horizonFeatureImg,j,i,sz_x,sz_y);
			preFeature=getElement(horizonFeatureImg,j,i-2,sz_x,sz_y);
			nextFeature=getElement(horizonFeatureImg,j,i+2,sz_x,sz_y);
			preFeature2=getElement(horizonFeatureImg,j,i-1,sz_x,sz_y);
			nextFeature2=getElement(horizonFeatureImg,j,i+1,sz_x,sz_y);
			if((currentFeature > width*2-acceptableRange) && ( preFeature < width || nextFeature < width || preFeature2 < width || nextFeature2 < width)){
				out[j+i*sz_x]=255;
			
			}
			
		}
	}
	#endif
	//width*=1.3;
	getLineFeatureImg(in,verticalFeatureImg,sz_x,sz_y,width,binaryTh,VERTICAL);
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			currentFeature=getElement(verticalFeatureImg,j,i,sz_x,sz_y);
			preFeature=getElement(verticalFeatureImg,j-2,i,sz_x,sz_y);
			nextFeature=getElement(verticalFeatureImg,j+2,i,sz_x,sz_y);
			preFeature2=getElement(verticalFeatureImg,j-1,i,sz_x,sz_y);
			nextFeature2=getElement(verticalFeatureImg,j+1,i,sz_x,sz_y);
			if((currentFeature > width*2-acceptableRange) && ( preFeature < width || nextFeature < width || preFeature2 < width || nextFeature2 < width)){
			//if((currentFeature > width*2-acceptableRange) && ( preFeature < width && nextFeature < width )){
				out[j+i*sz_x]=255;
			}
		
		}
	}

	
	delete [] verticalFeatureImg;
	delete [] horizonFeatureImg;
}
HiStatus getPosition(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y, int delta_x,int delta_y){
	int max_power=0;
 	int temp_power=0;
 	class HiStatus st;
	
	for(int k=hai_x-2;k<=hai_x+2;k+=2){
		for(int l=hai_y-2;l<=hai_y+2;l+=2){
			for(int i=pos_y-delta_y;i<pos_y+delta_y;i++){
				for(int j=pos_x-delta_x;j<pos_x+delta_x;j++){
					temp_power=getRectanglePower(in ,sz_x,sz_y,j,i, k,l,1);
					temp_power/=(hai_x)*2+(hai_y)*2-2;
					if(temp_power>max_power){
						max_power=temp_power;
						st.Pos.x=j;
						st.Pos.y=i;
						st.Size.x=k;
						st.Size.y=l;
						st.feature1=max_power;
					}
					
				}
			}
		}
	}
	printf("(%d %d) (%d %d) max_power=%d\n",st.Pos.x,st.Pos.y,st.Size.x,st.Size.y,max_power);
	return st;
}
int getRectanglePower(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag){
	int power=0;
	//左辺+	//右辺
	for(int i=pos_y;i<pos_y+hai_y;i++){
		power+=in[pos_x+i*sz_x];
		power+=in[pos_x+hai_x+i*sz_x];
	
	}
	//上辺+下辺
	for(int j=pos_x;j<pos_x+hai_x;j++){
		power+=(in[j+pos_y*sz_x]);
		power+=(in[j+(pos_y+hai_y)*sz_x]);
	}
	
	
	return power;
}
int ChangeView(unsigned char *fixImg,int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag){
	
	int power=0;
	//左辺+	//右辺
	for(int i=pos_y;i<pos_y+hai_y;i++){
			fixImg[pos_x+i*sz_x]=0;
			fixImg[pos_x+hai_x+i*sz_x]=0;
	}
	//上辺+下辺
	for(int j=pos_x+1;j<pos_x+hai_x-1;j++){
		fixImg[j+pos_y*sz_x]=0;
		fixImg[j+(pos_y+hai_y)*sz_x]=0;
	}
}
//まあるでフォークのようなシステムだから
int forkSystemX(unsigned char *in,int sz_x, int sz_y, int &start_X,int flag){
	int fork_x[4]={0};
	int fork_max=0;
	int count=0;
	int temp=0;
	int avg=0;
	for(int i=0;i<4;i++){
		fork_x[i]=9999;
	}
	if(flag==FROM_TOP){
		for(int i=0;i<4;i++){
			
			//for(int j=10;j<sz_x/10;j++){
				for(int j=10;j<sz_x/FORK_DIVIDED_NUM_X;j++){

				//printf("%d ",in[j+(i+1)*VERTICAL/5*sz_x]);
				if(in[j+(i+2)*sz_y/7*sz_x]>FORK_EDGE_DETECT_TH){
					fork_x[i]=j;
					break;
				}
			}
			//printf("why\n");
		}
	}else if(flag==FROM_BOTTOM){
		for(int i=0;i<4;i++){
//			for(int j=sz_x-10;j>sz_x*9/10;j--){
				for(int j=sz_x-10;j>sz_x*(FORK_DIVIDED_NUM_X-1)/FORK_DIVIDED_NUM_X;j--){
				//printf("%d ",in[j+(i+1)*VERTICAL/5*sz_x]);
				if(in[j+(i+2)*sz_y/7*sz_x]>FORK_EDGE_DETECT_TH){
					fork_x[i]=j;
					break;
				}
			}
			//printf("why\n");
		}
	}else{
		printf("Error forksystem\n");
		exit(0);
	}
	for(int i=0;i<4;i++){
		
		avg+=fork_x[i];
	}
	printf("%d %d %d %d",fork_x[0],fork_x[1],fork_x[2],fork_x[3]);
	//printf("end\n");
	
	for(int i=0;i<4;i++){
		for(int j=i+1;j<4;j++){
			temp=abs(fork_x[i]-fork_x[j]);
			if(fork_max<temp){
				fork_max=temp;
			}
			
		}
	}
	avg/=4;
	start_X=avg;
//	if(fork_max>15){
		if(fork_max>FORK_THRESHOLD){
		return fork_max;
	}else{
		return 0;
	}
	
}
//まあるでフォークのようなシステムだから
int forkSystemY(unsigned char *in,int sz_x, int sz_y,int &start_Y,int flag){
	int fork_y[4]={0};
	int fork_y_new[3]={0};
	int fork_max=0;
	int count=0;
	int temp=0;
	int avg=0;
	for(int i=0;i<4;i++){
		fork_y[i]=9999;
	}
	
	if(flag==FROM_LEFT){
		for(int j=0;j<4;j++){
			//for(int i=10;i<sz_y/10;i++){
				for(int i=10;i<sz_y/FORK_DIVIDED_NUM_Y;i++){
		
				//
				if(in[(j+2)*sz_x/7+i*sz_x]>FORK_EDGE_DETECT_TH){
					fork_y[j]=i;
					break;
				}
			}
			//printf("why\n");
		}
	}else if(flag==FROM_RIGHT){
		for(int j=0;j<4;j++){
			//for(int i=sz_y-10;i>sz_y*9/10;i--){
				for(int i=sz_y-10;i>sz_y*(FORK_DIVIDED_NUM_Y-1)/FORK_DIVIDED_NUM_Y;i--){

				//printf("%d %d\n",i,in[(j+1)*HORIZON/5+i*sz_x]);
				if(in[(j+2)*sz_x/7+i*sz_x]>FORK_EDGE_DETECT_TH){
					fork_y[j]=i;
					break;
				}
			}
			//printf("why\n");
		}
	}
		
	for(int i=0;i<4;i++){
//		printf("%d ",fork_y[i]);
		avg+=fork_y[i];
	}
	printf("%d %d %d %d",fork_y[0],fork_y[1],fork_y[2],fork_y[3]);

	//printf("end\n");
	
	for(int i=0;i<4;i++){
		for(int j=i+1;j<4;j++){
			temp=abs(fork_y[i]-fork_y[j]);
			if(fork_max<temp){
				fork_max=temp;
			
			}
			
		}
	}
	avg/=4;
	start_Y=avg;
//	if(fork_max>15){
	if(fork_max>FORK_THRESHOLD){
		return fork_max;
	}else{
		return 0;
	}
	
}

#ifndef NATIVE_DEBUG
int main(){
	HiStatus HiArray[36]={0};
	int ret=0;
	int imgWidth=640;
	int imgHeight=390;

	#if 1
	unsigned char *orgImg;
	orgImg = new unsigned char [imgWidth * imgHeight];

	FILE *fp_org;
	if((fp_org=fopen("640_390_new.raw","r"))==NULL){
		printf("Error file open error\n");
		exit(0);
	}
	if(fread(orgImg, sizeof(unsigned char),imgWidth * imgHeight,  fp_org) ==0){
		printf("Error file read error\n");
		exit(0);
	}
	fclose(fp_org);
	#endif
	
	ret=extractSampleHai(imgWidth,imgHeight,orgImg,HiArray);
	
	for(int i=0;i<35;i++){
		printf("No.%d %d %d = %d %d singal=%d\n",i,HiArray[i].Pos.x,HiArray[i].Pos.y,HiArray[i].Size.x,HiArray[i].Size.y,HiArray[i].feature1);
	}
	
	delete [] orgImg;
}
		
#endif

int scaleingHai(int imgWidth,int imgHeight,unsigned char *in,unsigned char *out,class HiStatus Hi){
	unsigned char *extractedHai;
	extractedHai= new unsigned char[Hi.Size.x*Hi.Size.y];
	int count=0;
	for(int i=Hi.Pos.y;i<Hi.Pos.y+Hi.Size.y;i++){
		for(int j=Hi.Pos.x;j<Hi.Pos.x+Hi.Size.x;j++){
			extractedHai[count++]=in[j+i*imgWidth];
		}
	}
	if(Hi.Size.x || Hi.Size.y){
		scalingSize(extractedHai,out,CONST_HAI_WIDTH,CONST_HAI_HEIGHT,Hi.Size.x,Hi.Size.y);
	}else{
		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT;i++){
			out[i]=0;
		}
	}
	delete [] extractedHai;
}

int arrangeHai(int imgWidth,int imgHeight,unsigned char *in,unsigned char *out,class HiStatus *HiArray){
	
	int height=0;
	int width=0;
	int count=0;
	unsigned char *orgnizedHai;
	orgnizedHai=new unsigned char[CONST_HAI_HEIGHT*CONST_HAI_WIDTH];
	for(int i=0;i<34;i++){
		scaleingHai(imgWidth,imgHeight,in,orgnizedHai, HiArray[i]);
		for(int j=0;j<CONST_HAI_WIDTH*CONST_HAI_HEIGHT;j++){
			out[count++]=orgnizedHai[j];
		}
		
	}
	printf("arrangeHaiCount=%d",count);
	
	delete [] orgnizedHai;
}
		
int extractSampleHai(int imgWidth,int imgHeight,unsigned char *orgImg,class HiStatus *HiArray){
	
	unsigned char *negativeImg;
	unsigned char *edgeImg;
	unsigned char *binaryImg;
	unsigned char *verticalLine;
	unsigned char *horizonLine;
	unsigned char *outImg;

	printf("Extracting...  %d %d\n",imgWidth,imgHeight);
	negativeImg = new unsigned char [imgWidth * imgHeight];
	edgeImg = new unsigned char [imgWidth * imgHeight];
	binaryImg =new unsigned char [imgWidth * imgHeight];
	verticalLine =new unsigned char [imgHeight];
	horizonLine =new unsigned char [imgWidth];
	outImg=new unsigned char[imgWidth *imgHeight];
	
	
	u_wtfile(orgImg,"original.raw",imgWidth*imgHeight);
	//反転
	filterNegative(orgImg,negativeImg,imgWidth,imgHeight);
	u_wtfile(negativeImg,"nagative.raw",imgWidth*imgHeight);
	
	//エッジ
	filterEdge(negativeImg,edgeImg,imgWidth,imgHeight,PREWITT);
	u_wtfile(edgeImg,"edge.raw",imgWidth*imgHeight);
	
	
	CleanEdge(edgeImg,imgWidth,imgHeight,1);
	u_wtfile(edgeImg,"edgeClean.raw",imgWidth*imgHeight);
	RotateImg(edgeImg,edgeImg,imgWidth,imgHeight,1.0);
	u_wtfile(edgeImg,"rot.raw",imgWidth*imgHeight);

	coord startPos,endPos;
	int ret_top,ret_bottom,ret_left,ret_right;
	
	
	//フォークシステム
	printf("top\n");
	ret_top=forkSystemX(edgeImg,imgWidth,imgHeight,startPos.x,FROM_TOP);
	printf("bottom\n");
	ret_bottom=forkSystemX(edgeImg,imgWidth,imgHeight,endPos.x,FROM_BOTTOM);
	printf("left\n");
	ret_left=forkSystemY(edgeImg,imgWidth,imgHeight,startPos.y,FROM_LEFT);
	printf("right\n");
	ret_right=forkSystemY(edgeImg,imgWidth,imgHeight,endPos.y,FROM_RIGHT);

	int HaiDecisionLine=0;
	int Hai_x=0;
	int Hai_y=0;
	
	Hai_x=(endPos.x-startPos.x)/9;
	Hai_y=(endPos.y-startPos.y)/4;
	
	printf("Check! %d %d %d %d\n",ret_top,ret_bottom,ret_left,ret_right);
	printf("Start.Pos=(%d,%d), End.Pos=(%d,%d)\n",startPos.x,startPos.y,endPos.x,endPos.y);
	printf("ImageSize=%d %d, HaiSize = %d %d\n",endPos.x-startPos.x,endPos.y-startPos.y,(endPos.x-startPos.x)/9,(endPos.y-startPos.y)/4);

	
	if(ret_top ||  ret_bottom || ret_left || ret_right){
		printf("Error need to adjust phot \n");
		if(ret_top) printf("Top ERROR\n");
		if(ret_bottom) printf("Bottom ERROR\n");
		if(ret_right) printf("RIGHT ERROR\n");
		if(ret_left) printf("LEFT ERROR\n");
		delete [] outImg;
		delete [] verticalLine;
		delete [] horizonLine;
		delete [] negativeImg;
		delete [] edgeImg;
		delete [] binaryImg;
		if(ret_top && !ret_bottom && !ret_right && !ret_left){
			return 0;
		}else if(!ret_top && ret_bottom && !ret_right && !ret_left){
			return 0;
		}else if(!ret_top && !ret_bottom && ret_right && !ret_left){
			return 0;
		}else if(!ret_top && !ret_bottom && !ret_right && ret_left){
			return 0;
		}else{
			return 0;
		}
	}

	if(!(Hai_x > imgWidth/ 18 && Hai_y > imgHeight/8 && Hai_x < imgWidth/ 9&& Hai_y < imgHeight/4)){
		printf("Error not incorrect Hai size %d %d\n",Hai_x,Hai_y);
		delete [] outImg;
		delete [] verticalLine;
		delete [] horizonLine;
		delete [] negativeImg;
		delete [] edgeImg;
		delete [] binaryImg;
		return 0;
	}
	
	//2値化
	//binarizeImage(edgeImg,binaryImg,imgWidth,imgHeight,32);
	//u_wtfile(binaryImg,"binary.raw",imgWidth*imgHeight);
	
	LineDetecotor(edgeImg,binaryImg,imgWidth,imgHeight,LINEDETECTOR_WIDTH,LINEDETECTOR_DEPTH,LINEDETECTOR_ACCEPT);



	
	//RotateImg(orgImg,outImg,imgWidth,imgHeight,1.0);

	int power=0;
	int count=0;
	int currentNum=0;
	
	
	
	int next_pos_y=0;
	int next_size_y=0;
	class HiStatus HiSt;
	class HiStatus nextSt;
	class HiStatus currentSt;
	class HiStatus HiInit={0};

	currentSt.Pos.x=startPos.x;
	currentSt.Pos.y=startPos.y;
	currentSt.Size.x=Hai_x;
	currentSt.Size.y=Hai_y;
	
	
	
	printf("*****Welcome to Extracttion Hai");

	for(int i=0;i<4;i++){
		
		for(int j=0;j<9;j++){
			
			if(!(currentSt.Pos.x>=0 && currentSt.Pos.y >=0 && currentSt.Pos.x+currentSt.Size.x<imgWidth &&  currentSt.Pos.y+currentSt.Size.y<imgHeight)){
				printf("Error Extract Hai (%d %d)\n",j,i);
				return 0;
			}
			
			
			if(j==0){
				HiSt=getPosition(binaryImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 5,5);
				nextSt.Pos.x=currentSt.Pos.x;
				nextSt.Pos.y=currentSt.Pos.y;
				nextSt.Size.x=currentSt.Size.x;
				nextSt.Size.y=currentSt.Size.y;
			}else{
				HiSt=getPosition(binaryImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 3,3);
				
			}/*
			if(i==0 & j==0){
				if(HiSt.feature1<150){
					printf("FIRST HAI ERROR\n)
					return 1;
				}
			}*/
			if(HiSt.feature1>CORRALATION_THRESHOLD){
				count++;
				currentSt=HiSt;
			}else{
				HiSt=HiInit;
			}
			
			HiArray[currentNum++]=HiSt;
			//ChangeView(outImg ,imgWidth,imgHeight,HiSt.Pos.x,HiSt.Pos.y, HiSt.Size.x,HiSt.Size.y,1);
			currentSt.Pos.x+=currentSt.Size.x;
			
			

		}
		currentSt.Pos.x =nextSt.Pos.x;
		currentSt.Pos.y =nextSt.Pos.y+nextSt.Size.y;
		currentSt.Size.x=nextSt.Size.x;
		currentSt.Size.y=nextSt.Size.y;
		
		//return 0;
	}
	u_wtfile(outImg,"outImg.raw",imgWidth*imgHeight);
	printf("SUCCESS!valid_count=%d;",count);
	
	printf(" prgram end\n");
	delete [] outImg;
	delete [] verticalLine;
	delete [] horizonLine;
	delete [] negativeImg;
	delete [] edgeImg;
	delete [] binaryImg;
	return count;
}