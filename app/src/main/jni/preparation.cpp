#include "akasaka.h"

//サイズのリサイズ
//in 入力画像
//out 出力画像
//toSizeW,toSizeH 出力画像Widthとheight
//fromSizeW,fromSizeH　入力画像widthとheight
void scalingSize(unsigned char *in,unsigned char *out,int toSizeW,int toSizeH,int fromSizeW,int fromSizeH)
{
	unsigned char **temp;
	
	float x_scale=(float)toSizeW/(float)fromSizeW;
	float y_scale=(float)toSizeH/(float)fromSizeH;
	int scalingSizeErrorCount=0;
	temp=new unsigned char  *[fromSizeW];
	for(int i=0;i<fromSizeW;i++){
			temp [i]=new unsigned char [fromSizeH];
	}
	//printf("From =%d %d To = %d %d Scale=%f %f\n",fromSizeW,fromSizeH,toSizeW,toSizeH,x_scale,y_scale);
	
	int s_xx,s_yy;
	for(int i=0;i<fromSizeH;i++)
	{
		for(int j=0;j<fromSizeW;j++)
		{
			
			temp[j][i]=in[j+i*fromSizeW];
		}
	}
	

	for(int i=0;i<toSizeH;i++)
	{
		for(int j=0;j<toSizeW;j++)
		{
			s_xx=(int)(j/(float)x_scale);
			s_yy=(int)(i/(float)y_scale);
			
			if(s_xx<0){
				s_xx=0;
			}else if(s_xx>=fromSizeW){
				s_xx=fromSizeW;
			}
			if(s_yy<0){
				s_yy=0;
			}else if(s_yy>=fromSizeH){
				s_yy=fromSizeH;
			}

			//if(s_xx>=0 && s_yy>=0 && s_xx<fromSizeW && s_yy < fromSizeH){
			if(s_xx>=0 && s_yy>=0 && s_xx<=fromSizeW && s_yy <= fromSizeH){
				//printf("%d %d => %d %d signal=%d \n",j,i,s_xx,s_yy,temp[s_xx][s_yy]);
				out[j+i*toSizeW]=temp[s_xx][s_yy];
			}else {
				scalingSizeErrorCount++;
				printf("ScalingSizeError %d %d: from(%d %d), to(%d %d) j,i(%d %d)\n",s_xx,s_yy,fromSizeW,fromSizeH,toSizeW,toSizeH,j,i);
				break;
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

//回転
//bufmoto 入力画像
//bufout 出力画像
//サイズ sz_x*sz_y
//回転角度　単位は°
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
	
	
}

//配列コピー
void CopyArray(unsigned char *in,unsigned char *out,int sz_x,int sz_y){
	for(int i=0;i<sz_x*sz_y;i++){
		out[i]=in[i];
	}
}
//回転
void RotateImg(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float rotaiton){
	unsigned char *org;
	unsigned char *rot;
	

	
	if(sz_x>sz_y){
		//横長
		org=new unsigned char[sz_x*sz_x];
		rot=new unsigned char[sz_x*sz_x];

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
	}else{
		//縦長
		org=new unsigned char[sz_y*sz_y];
		rot=new unsigned char[sz_y*sz_y];
		
		
		for(int i=0;i<sz_y*sz_y;i++){
			org[i]=0;
			rot[i]=0;
		}
		
		for(int j=0;j<sz_x;j++){
			for(int i=0;i<sz_y;i++){
				org[j+(i+(sz_y-sz_x)/2)*sz_x]=in[j+i*sz_x];
			}
		}

		kaiten(org,rot,sz_y,rotaiton);
		//u_wtfile(rot,"org2.raw",sz_x*sz_x);
		for(int j=0;j<sz_x;j++){
			for(int i=0;i<sz_y;i++){
					out[j+i*sz_x]=rot[j+(i+(sz_y-sz_x)/2)*sz_x];
			}
		}
		
	}
	
	delete [] org;
	delete [] rot;
	
}
//端の画像をカット
//widthはその幅
void CleanEdge(unsigned char *inout,int sz_x, int sz_y,int width){
	int i,j;
	i=0;
	for(int k=0;k<width;k++){
		i=k;
		for(j=0;j<sz_x;j++){
			inout[j+i*sz_x]=0;
		}
		i=sz_y-1-k;
		for(j=0;j<sz_x;j++){
			inout[j+i*sz_x]=0;
		}
		j=k;
		for(i=0;i<sz_y;i++){
			inout[j+i*sz_x]=0;
		}
		j=sz_x-1-k;
		for(i=0;i<sz_y;i++){
			//printf("%d,%d\n",j,i);
			inout[j+i*sz_x]=0;
		}
	}
	
}
//画像読み込み
int u_rdfile(unsigned char *buf,const  char *fn, int SZ)
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

//画像の一列だけゲット
void getVerticalLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y){
	for(int i=0;i<sz_y;i++){
		out[i]=in[lineNum+i*sz_x];
	}
}
//画像の１行だけゲット
void getHorizonLine(unsigned char * in, unsigned char *out,int lineNum, int sz_x, int sz_y){
	for(int j=0;j<sz_x;j++){
		out[j]=in[j+lineNum*sz_x];
	}
}
//画像出力
int u_wtfile(unsigned char *buf,  const char *fn, int SZ)
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
//画像出力 ｺﾝﾊﾟｲﾙスイッチなし。/data用
int u_wtfileEx(unsigned char *buf,  const char *fn, int SZ)
{
	FILE	*fp;

	if(NULL == (fp = fopen(fn,"wb")))
	{
		printf(" Write file error!%s\n\7",fn);
		exit(true);
	}

	fwrite(buf, sizeof(unsigned char), SZ, fp);

	fclose(fp);
	
	return true;

}
//画像のサム値をゲット
int getSum(unsigned char *in,  int size){
	int getSum=0;
	for(int i=0;i<size;i++){
		getSum+=in[i];
	}
	return getSum;
}
//画素を標準出力
void showLine(unsigned char *in,  int size){
	printf("----show the line\n");
	for(int i=0;i<size;i++){
		printf("%d\n",in[i]);
	}
	printf("----end\n");
}
//画像の平均ゲット
float getAverage(unsigned char *in,  int size){
	int sum=0;
	sum=getSum(in,size);
	return float(sum)/(float)size;
}
//配列の初期化
void InitialArray(unsigned char *in, int size){
	for(int i=0;i<size;i++){
		in[i]=0;
	}
}
//画素が範囲外かどうかを判定し、範囲内であればその画素を返す。
//違った場合はExceptionValueを返す。
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
//反転画像
void filterNegative(unsigned char *in, unsigned char *out, int sz_x, int sz_y){
	for(int i=0;i<sz_x*sz_y;i++){
		out[i]=255-in[i];
	}
}
//2値化画像
void binarizeImage( unsigned char *in,unsigned char *out,int size_x,int size_y, int threshold){
	for(int i=0;i<size_x*size_y;i++){
		if(in[i]>threshold){
			out[i]=255;
		}else{
			out[i]=0;
		}
	}
}
//エッジ処理
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
//ガウシアンの処理
int calcGaussian(int x,int y,int sz_x,int sz_y,unsigned char *in,int flag){
	int temp=0;
	int gravitiy_count=0;
	int element=0;
	//float square_element=0.0;
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
			
			
				element+=gaussianFilter[gravitiy_count]*temp;
				
			gravitiy_count++;
		}
	}

	element_edge=element/16;
	return element_edge;
	
}
//Peak to Side robeの取得。現在使われてない
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
//牌の長さを取得。現在使われていない
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
//
void getHorizonLine(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int start_y,int width){
	int lineResult=0;
	//int temp=0;
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
	//int temp=0;
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
	CleanEdge(out,sz_x,sz_y,1);
	
}

void filterGaussian(unsigned char *in, unsigned char *out, int sz_x, int sz_y,int flag){
	for(int i=0;i<sz_y;i++){
		for(int j=0;j<sz_x;j++){
			out[j+i*sz_x]=calcGaussian(j,i,sz_x,sz_y,in,flag);
		}
	}
	
}

#ifdef NATIVE_DEBUG
void filterEdgeJint(unsigned char *in, jint *out, int sz_x, int sz_y,int flag){
	if(flag!=GAUSSIAN){
		for(int i=0;i<sz_y;i++){
			for(int j=0;j<sz_x;j++){
				out[j+i*sz_x] = (int)calcEdge(j,i,sz_x,sz_y,in,flag);
			}
		}
	}
}
#endif


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



HiStatus getPosition(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y, int delta_x,int delta_y,int flag){
	int max_power=-1;
 	int temp_power=0;
	int delta_width=2;
	int delata_height=2;
	class HiStatus st;
	
	if(flag==FIRST_AGARI_GETPOS){
		delata_height=4;
		delta_width=10;
	}

	
	//printf("debug %d %d - %d %d\n",pos_x,pos_y,hai_x,hai_y);
	for(int k=hai_x-delta_width;k<=hai_x+delta_width+1;k+=2){
		for(int l=hai_y-delata_height;l<=hai_y+delata_height+1;l+=2){
			for(int i=pos_y-delta_y;i<pos_y+delta_y;i++){
				for(int j=pos_x-delta_x;j<pos_x+delta_x;j++){
					if(j>=0 && j+k <sz_x && i>=0 && i+l<sz_y){
						temp_power=getRectanglePower(in ,sz_x,sz_y,j,i, k,l,flag);
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
	}
	printf("(%d %d) (%d %d) max_power=%d\n",st.Pos.x,st.Pos.y,st.Size.x,st.Size.y,max_power);
	return st;
}

int getRectanglePower(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag){
	int power=0;
	int count=0;
	
	if(!(pos_x>=0 && pos_y>=0 && pos_x+hai_x<sz_x && pos_y+hai_y<sz_y)){
			printf("Erorr:exception invalid erea because StartPos(%d %d) or EndPos(%d %d) is over (%d %d)\n",pos_x,pos_y,pos_x+hai_x,pos_y+hai_y,sz_x,sz_y);
		if(flag==FIRST_AGARI_GETPOS_CHECK || flag==FIRST_SAMPLE_GETPOS_CHECK){
			return 255;
		}else{
			return 0;
		}
	}
	//線が切れやすいので４隅だけ使用する
	if(flag==SAMPLE_GETPOS){
	
		
		//左右上隅
		for(int i=pos_y;i<pos_y+hai_y/5;i++){
			power+=in[pos_x+i*sz_x];
			power+=in[pos_x+hai_x+i*sz_x];
			count+=2;		
		
		}
		for(int j=pos_x;j<pos_x+hai_x/5;j++){
			power+=(in[j+pos_y*sz_x]);
			power+=(in[j+(pos_y+hai_y)*sz_x]);
			count+=2;
		}
		
		//左右下隅
		for(int i=pos_y+(hai_y/5)*4;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			power+=in[pos_x+hai_x+i*sz_x];
			count+=2;
		}


		for(int j=pos_x+(hai_x/5)*4;j<pos_x+hai_x;j++){
			power+=(in[j+pos_y*sz_x]);
			power+=(in[j+(pos_y+hai_y)*sz_x]);
			count+=2;
		}
	//枠で検出。今は使用されていない
	}else if(flag==AGARI_GETPOS){
		

		//左辺+	//右辺
		for(int i=pos_y;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			power+=in[pos_x+hai_x+i*sz_x];
			count+=2;
		}
		//上辺+下辺
		for(int j=pos_x;j<pos_x+hai_x;j++){
			power+=in[j+pos_y*sz_x];
			power+=in[j+(pos_y+hai_y)*sz_x];
			count+=2;
		}

	//誤検出用上がり牌
	}else if(flag==FIRST_AGARI_GETPOS_CHECK){

		//左辺
		for(int i=pos_y;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			count++;
		}
		//上辺と下辺
		for(int j=pos_x;j<pos_x+hai_x;j++){
			power+=in[j+pos_y*sz_x];
			power+=in[j+(pos_y+hai_y)*sz_x];
			count+=2;
		}
		printf("checkDetectorPowerAgari:%d\n",power);
		
		
	////誤検出用上がり牌の鳴き
	}else if(flag==FIRST_AGARI_NAKI_GETPOS_CHECK){
		
		for(int i=pos_y;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			count++;
		}
		
		//上辺と下辺
		
		/*for(int j=pos_x;j<pos_x+hai_x/2;j++){
			power+=in[j+pos_y*sz_x];
			power+=in[j+(pos_y+hai_y)*sz_x];
			count+=2;
		}*/
		printf("checkDetectorPowerAgariNaki:%d\n",power);
		
	//誤検出用サンプル牌
	}else if(flag==FIRST_SAMPLE_GETPOS_CHECK){
		//左辺
		for(int i=pos_y;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			count++;
		}
		//左辺
		for(int j=pos_x;j<pos_x+hai_x;j++){
			power+=in[j+pos_y*sz_x];
			count++;
		}
		printf("checkDetectorPowerSample:%d\n",power);
		
	}else if(flag==FIRST_SAMPLE_GETPOS_INTERNAL_CHECK){
		int hidari=0;
		int migi=0;
		int ue=0;
		int sita=0;
		//左辺+	//右辺
		for(int i=pos_y;i<pos_y+hai_y;i++){
			power+=in[pos_x+i*sz_x];
			power+=in[pos_x+hai_x+i*sz_x];
			hidari+=in[pos_x+i*sz_x];
			migi+=in[pos_x+hai_x+i*sz_x];

			
			count+=2;
		}
		//上辺+下辺
		for(int j=pos_x;j<pos_x+hai_x;j++){
			power+=in[j+pos_y*sz_x];
			power+=in[j+(pos_y+hai_y)*sz_x];
			ue+=in[j+pos_y*sz_x];
			sita+=in[j+(pos_y+hai_y)*sz_x];
			count+=2;
		}
		printf("checkDetectorPowerSample hidari=%d migi=%d ue=%d sita=%d\n",hidari,migi,ue,sita);
	}else{
		printf("fucking error\n");
	}
	return power/count;
}

int ChangeView(unsigned char *fixImg,int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int flag){
	
	//int power=0;
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
	return 0;
}


int scaleingHai(int imgWidth,int imgHeight,unsigned char *in,unsigned char *out,class HiStatus Hi,int flag){
	unsigned char *extractedHai;
	extractedHai= new unsigned char[Hi.Size.x*Hi.Size.y];
	int count=0;
	int orgSizeX=0;
	int orgSizeY=0;
	if(flag!=NAKIFLAG){
		
		for(int i=Hi.Pos.y;i<Hi.Pos.y+Hi.Size.y;i++){
			for(int j=Hi.Pos.x;j<Hi.Pos.x+Hi.Size.x;j++){
					extractedHai[count++]=in[j+i*imgWidth];
			}
		}
		
		orgSizeX=Hi.Size.x;
		orgSizeY=Hi.Size.y;
		
	}else {
		
		for(int j=Hi.Pos.x;j<Hi.Pos.x+Hi.Size.x;j++)
			for(int i=Hi.Pos.y+Hi.Size.y-1;i>=Hi.Pos.y;i--){{
					extractedHai[count++]=in[j+i*imgWidth];
			}
		}
		orgSizeX=Hi.Size.y;
		orgSizeY=Hi.Size.x;
	}
	
	
	if(Hi.Size.x || Hi.Size.y){
		scalingSize(extractedHai,out,CONST_HAI_WIDTH,CONST_HAI_HEIGHT,orgSizeX,orgSizeY);
	}else{
		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT;i++){
			out[i]=0;
		}
	}
	delete [] extractedHai;
	return 0;
}

int arrangeHai(int imgWidth,int imgHeight,unsigned char *in,int num,unsigned char *out,class HiStatus *HiArray){
	
	//int height=0;
	//int width=0;
	int count=0;
	char str[1024];
	int spaceWidth=0;
	unsigned char *orgnizedHai;
	orgnizedHai=new unsigned char[CONST_HAI_HEIGHT*CONST_HAI_WIDTH];
	memset( orgnizedHai, 0, CONST_HAI_HEIGHT*CONST_HAI_WIDTH);
	HiStatus tempHiArray;
	for(int i=0;i<num;i++){
		printf("ArrangeHai %d %d : %d %d\n",HiArray[i].Pos.x,HiArray[i].Pos.y,HiArray[i].Size.x,HiArray[i].Size.y);
		if(HiArray[i].Size.x==0 || HiArray[i].Size.y==0){
			break;
		}
		spaceWidth=HiArray[i].Size.x/12;
		//マジックナンバー要変更。割合がおかしくなるし
		tempHiArray.Pos.x=HiArray[i].Pos.x+spaceWidth;
		tempHiArray.Pos.y=HiArray[i].Pos.y+spaceWidth;
		tempHiArray.Size.x=HiArray[i].Size.x-spaceWidth*2;
		tempHiArray.Size.y=HiArray[i].Size.y-spaceWidth*2;
		//printf("NAKIHAI_%d =%d\n",i,HiArray->feature2);
		scaleingHai(imgWidth,imgHeight,in,orgnizedHai,tempHiArray,HiArray[i].feature2);
		
		//CleanEdge(orgnizedHai,CONST_HAI_WIDTH,CONST_HAI_HEIGHT,5);
		for(int j=0;j<CONST_HAI_WIDTH*CONST_HAI_HEIGHT;j++){
			out[count++]=orgnizedHai[j];
		}


	//	sprintf(str,"/data/data/jp.tonosama.komoki.janscouter/hai_%d.raw",i);
	//	u_wtfileEx(orgnizedHai, str, CONST_HAI_WIDTH*CONST_HAI_HEIGHT);

	}
	//printf("arrangeHaiCount=%d\n",count);
	
	delete [] orgnizedHai;
	return 0;
}



	
void filterHighPass(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int threshold){
	if(threshold==0 || threshold ==255){
		printf("threshold error \n");
		return;
	}
	for(int j=0;j<sz_x*sz_y;j++){
		if(in[j] < threshold){
			out[j] =0;
		}else{
		//	out[j]=(in[j]-threshold)*255.0/(float)(255.0-threshold);
			out[j]=in[j]-threshold;
		}
	}
}

void filterBandPass(unsigned char *in,unsigned char *out,int sz_x, int sz_y,int lowThre,int highThre){

	for(int j=0;j<sz_x*sz_y;j++){
		if(in[j] < lowThre){
			out[j] =0;
		}else if(in[j] > highThre){
			out[j]=highThre;
		}else{
			out[j]=in[j]-lowThre;
		}
	}
}
		

void setForkFlag(int &ret , int threshold){
	if(ret>threshold){
		ret=1;
	}else{
		ret=0;
	}
}
	

void preImageProcess(int imgWidth,int imgHeight,unsigned char *inImg, unsigned char *outImg){

	unsigned char *binaryImg;
	unsigned char *gausuImg;
	unsigned char *highImg;
	unsigned char *negativeImg;
	unsigned char *edgeImg2;
	
	
	printf("Img processing...  %d %d\n",imgWidth,imgHeight);
	
	binaryImg =new unsigned char [imgWidth * imgHeight];
	gausuImg = new unsigned char[imgWidth *imgHeight];
	highImg = new unsigned char[imgWidth *imgHeight];
	negativeImg = new unsigned char [imgWidth *imgHeight];
	edgeImg2=new unsigned char [imgWidth * imgHeight];
	
	//オリジナル
	u_wtfile(inImg,"inImg.raw",imgWidth*imgHeight);
	
	//ガウシアン
	filterGaussian(inImg,gausuImg,imgWidth,imgHeight,PREWITT);
	u_wtfile(gausuImg,"gausu.raw",imgWidth*imgHeight);
	
	//ハイパスフィルタ
	filterHighPass(gausuImg,highImg,imgWidth,imgHeight,96);
	//filterBandPass(gausuImg,highImg,imgWidth,imgHeight,64,170);
	

	//反転
	filterNegative(highImg,negativeImg,imgWidth,imgHeight);
	u_wtfile(negativeImg,"nagative.raw",imgWidth*imgHeight);
	
	//エッジ
	filterEdge(negativeImg,edgeImg2,imgWidth,imgHeight,PREWITT);
	u_wtfile(outImg,"edge.raw",imgWidth*imgHeight);
	
	//２値化
	binarizeImage(edgeImg2,outImg,imgWidth,imgHeight,16);
	
	
	delete [] binaryImg;
	delete [] gausuImg;
	delete [] highImg;
	delete [] negativeImg;
	delete [] edgeImg2;
}



HiStatus findFirstHai(unsigned char *in, int sz_x, int sz_y){
	HiStatus st;
	int foundFlag=0;
	for(int j=0;j<sz_x/3;j++){
	//	printf("%d\n",in[j+sz_y/2*sz_x]);
		if(in[j+sz_y/2*sz_x]>128){
			st.Pos.x=j;
			foundFlag=1;
			break;
		}
	}
	if(foundFlag==0){
		printf("Not found find FirstHai at left\n");
		return st;
	}
	foundFlag=0;
	for(int i=0;i<sz_y/2;i++){
		if(in[st.Pos.x+40+i*sz_x]>128){
			st.Pos.y=i;
			foundFlag=1;
			break;
		}
	}
	if(foundFlag==0){
		printf("Not found find FirstHai at top\n");
		return st;
	}
	foundFlag=0;
	for(int i=sz_y;i>sz_y/2;i--){
		if(in[st.Pos.x+40+i*sz_x]>128){
			st.Size.y=i-st.Pos.y;
			foundFlag=1;
			break;
		}
	}
	if(foundFlag==0){
		printf("Not found find FirstHai at bottom\n");
		return st;
	}
	foundFlag=0;
	return st;

}
//牌の存在をチェック
int judgeExistenceEx(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int threshold,int &nakiHeight){
	HiStatus st;
	int foundFlag=0;
	int verticalStartPos;
	int vertialEndPos;
	
	verticalStartPos=3;

	//たてに上から串刺しにしてそこに128以上の画素があるかどうか
	printf("judge:pos_x=%d pos_y=%d, hai_x=%d hai_y=%d threshld=%d\n",pos_x,pos_y,hai_x,hai_y,threshold);
	printf("judge:verticalStartPos=%d\n",verticalStartPos);
	for(int i=verticalStartPos;i<verticalStartPos+hai_x;i++){
		if(in[pos_x+threshold+i*sz_x]>128){
			st.Pos.y=i;
			//printf("judge:Under top pre10=%d pre5=%d pre3=%d current=%d",in[pos_x+threshold+(i-10)*sz_x],in[pos_x+threshold+(i-5)*sz_x],in[pos_x+threshold+(i-3)*sz_x],in[pos_x+threshold+i*sz_x]);
			foundFlag=1;
			break;
		}
	}
	if(foundFlag==0){
		printf("Not found find existence at top\n");
		return NOEXISTENCE;
	}
	foundFlag=0;

	vertialEndPos=sz_y-3;

	printf("judge:vertialEndPos=%d",vertialEndPos);
	//たてに↓から串刺しにしてそこに128以上の画素があるかどうか

	for(int i=vertialEndPos;i>vertialEndPos-hai_y;i--){
		// kozawa add s
		if( (pos_x+threshold+i*sz_x) >= (sz_x * sz_y) ){
			printf("debug print: Error! size of in = %d but [pos_x+threshold+i*sz_x] = %d",(sz_x * sz_y),pos_x+threshold+i*sz_x);
		}else{
		// kozawa add e
			if(in[pos_x+threshold+i*sz_x]>128){
				st.Size.y=i-st.Pos.y;
				//printf("judge:Under top pre10=%d pre5=%d pre3=%d current=%d",in[pos_x+threshold+(i+10)*sz_x],in[pos_x+threshold+(i+5)*sz_x],in[pos_x+threshold+(i+3)*sz_x],in[pos_x+threshold+i*sz_x]);
				foundFlag=1;
				break;
			}
		}
	}
	if(foundFlag==0){
		printf("judge:Not found find existence at bottom\n");
		return NOEXISTENCE;
	}
	printf("judge:start,end =%d ,%d\n",verticalStartPos,vertialEndPos);
	printf("judge:EXSITET= (%d %d) orignailSize=%d\n",st.Pos.y,st.Size.y,hai_y);
	if(st.Size.y > hai_y*0.90 && st.Size.y < hai_y*1.20){
		printf("judge:EXSITENTCE!");
		return EXISTENCE;
	}else if(st.Size.y>hai_y*0.5){
		printf("judge:NakiEXSITENCE!");
		nakiHeight=st.Pos.y;
		return NAKIEXISTENCE;
	}else{
		return NOEXISTENCE;
	}

	return NOEXISTENCE;

}
int judgeExistence(unsigned char *in, int sz_x, int sz_y,int pos_x,int pos_y,int hai_x,int hai_y,int &nakiHeight){
	int result;
	int threshold;
	
	threshold=(hai_x*3)/4;
	result=judgeExistenceEx(in, sz_x,sz_y,pos_x,pos_y,hai_x,hai_y,threshold,nakiHeight);
	if(result!=NOEXISTENCE){
		return result;
	}
	/*これもハイを余分に数えてしまってたから消す	
	threshold=(hai_x*2)/4;
	result=judgeExistenceEx(in, sz_x,sz_y,pos_x,pos_y,hai_x,hai_y,threshold,nakiHeight);
	if(result!=NOEXISTENCE){
		return result;
	}
	*
	/* 近すぎて間違え易い気がするので不要
	threshold=hai_x*1/4;
	result=judgeExistenceEx(in, sz_x,sz_y,pos_x,pos_y,hai_x,hai_y,threshold,nakiHeight);
	if(result!=NOEXISTENCE){
		return result;
	}
	*/
	printf("judgeExistence() end");
	return NOEXISTENCE;

}
//まあるでフォークのようなシステムだから
int forkSystemX(unsigned char *in,int sz_x, int sz_y, int &start_X,int flag,int mode){
	int fork_x[4]={0};
	int fork_max=0;
	int count=0;
	int temp=0;
	int avg=0;
	int forkPointY=0;
	for(int i=0;i<4;i++){
		fork_x[i]=9999;
	}
	if(flag==FROM_LEFT){
		for(int i=0;i<4;i++){
			if(mode==SMAPLE_FORK_MODE){
				forkPointY=(i+2)*sz_y/7*sz_x;
			}else{
				forkPointY=(i+2)*sz_y/7*sz_x;
			}
			for(int j=5;j<sz_x-5;j++){
				if(in[j+forkPointY]>FORK_EDGE_DETECT_TH){
					fork_x[i]=j;
					break;
				}
			}
		}
	}else if(flag==FROM_RIGHT){
		for(int i=0;i<4;i++){
			if(mode==SMAPLE_FORK_MODE){
				forkPointY=(i+2)*sz_y/7*sz_x;
			}else{
				forkPointY=(i+4)*sz_y/11*sz_x;
			}
			for(int j=sz_x-5;j>5;j--){
				if(in[j+forkPointY]>FORK_EDGE_DETECT_TH){
					fork_x[i]=j;
					break;
				}
			
			}
		}
	}else{
		printf("Error forksystem\n");
		exit(0);
	}
	int errorCount=0;
	for(int i=0;i<4;i++){
		
		avg+=fork_x[i];
		if(fork_x[i]==9999 || fork_x[i]==0){
			errorCount++;
		}
	}
	if(errorCount>=2){
		return FORK_FAIL;
	}
	printf("forkX:%d %d %d %d",fork_x[0],fork_x[1],fork_x[2],fork_x[3]);
	
	start_X=0;
	//うまく言ってるようだけど。意味分からん
	if(mode==AGARI_FORK_MODE ){
		int ForkFoundCount=0;
		int fork_agari_x[4]={0};
		for(int i=0;i<4;i++){
			if(fork_x[i]!=9999){
				fork_agari_x[i]=fork_x[i];
				ForkFoundCount++;
			}
		}
		//printf("forkFoundCount=%d , start_x=%d\n",ForkFoundCount,start_X);

		if(ForkFoundCount < 1){
			return FORK_SUCCESS;
		}else{
			for(int i=0;i<ForkFoundCount;i++){
				start_X+=fork_agari_x[i];
			}
			start_X/=ForkFoundCount;
			printf("forkFoundCount=%d , start_x=%d\n",ForkFoundCount,start_X);
			return FORK_SUCCESS;
		}
			
			
	}else{
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
		if(fork_max>FORK_THRESHOLD){
			return FORK_SUCCESS;
			//return start_X;
		}else{
			return FORK_SUCCESS;
		}
	}
	
}
//まあるでフォークのようなシステムだから
int forkSystemY(unsigned char *in,int sz_x, int sz_y,int haiEndX,int &start_Y,int flag,int mode){
	int fork_y[4]={0};
	int fork_y_new[3]={0};
	int fork_max=0;
	int count=0;
	int temp=0;
	int avg=0;
	int forkPointX=0;
	for(int i=0;i<4;i++){
		fork_y[i]=9999;
	}
	
	if(flag==FROM_TOP){
		for(int j=0;j<4;j++){
			if(mode==SMAPLE_FORK_MODE){
				forkPointX=(j+2)*haiEndX/7;
			}else{
				forkPointX=(j+2)*haiEndX/7;
			}
				
			for(int i=5;i<sz_y/2;i++){
				if(in[forkPointX+i*sz_x]>FORK_EDGE_DETECT_TH){
					fork_y[j]=i;
					break;
				}
			}
		}
	}else if(flag==FROM_BOTTOM){
		for(int j=0;j<4;j++){
			if(mode==SMAPLE_FORK_MODE){
				forkPointX=(j+2)*haiEndX/7;
			}else{
				forkPointX=(j+3)*haiEndX/9;
			}
			for(int i=sz_y-5;i>sz_y/2;i--){
				if(in[forkPointX+i*sz_x]>FORK_EDGE_DETECT_TH){
					fork_y[j]=i;
					break;
				}
			}
		}
	}
	int errorCount=0;
	for(int i=0;i<4;i++){
//		printf("%d ",fork_y[i]);
		avg+=fork_y[i];
		if(fork_y[i]==9999 || fork_y[i]==0){
			errorCount++;
		}
	}
	if(errorCount>=2){
		return FORK_FAIL;
	}
	
	printf("forkY:%d %d %d %d",fork_y[0],fork_y[1],fork_y[2],fork_y[3]);

	//printf("end\n");
	
	int nearPos1=0;
	int nearPos2=0;
	int absMin=0;
	//近い値を二つ探す
	for(int i=0;i<4;i++){
		for(int j=i+1;j<4;j++){
			if(i!=j){
				temp=abs(fork_y[i]-fork_y[j]);
				if(absMin<temp){
					absMin=temp;
					nearPos1=i;
					nearPos2=j;
				}
			}
		}
	}
	printf("fork Near Pos1=%d Pos2=%d\n",fork_y[nearPos1],fork_y[nearPos2]);
	start_Y=(fork_y[nearPos1]+fork_y[nearPos2])/2;
	return FORK_SUCCESS;
	
}
int forkSystemMain(unsigned char *inImg,int sz_x, int sz_y,class HiStatus & haiSet,int mode){

	coord startPos,endPos;
	int ret;
	int Hai_x=0;
	int Hai_y=0;
	
	//フォークシステム
	//printf("top\n");
	ret=forkSystemX(inImg,sz_x,sz_y,startPos.x,FROM_LEFT,mode);
	//printf("bottom\n");
	ret+=forkSystemX(inImg,sz_x,sz_y,endPos.x,FROM_RIGHT,mode);
	
	if(ret!=FORK_SUCCESS){
		printf("forkX more 9999 error"); 
		return FORK_FAIL;
	}
	//まず横から
	if(endPos.x>=sz_x || startPos.x>=sz_x){
		printf("forkX system over size(%d) error startPos.x=%d endPos.x=%d n", sz_x,startPos.x ,endPos.x);
		return FORK_FAIL;
	}
	ret=0;
	//printf("left\n");
	ret=forkSystemY(inImg,sz_x,sz_y,endPos.x,startPos.y,FROM_TOP,mode);
	//printf("right\n");
	ret+=forkSystemY(inImg,sz_x,sz_y,endPos.x,endPos.y,FROM_BOTTOM,mode);
	
	if(ret!=FORK_SUCCESS){
		
		printf("forkY more 9999 error"); 
		return FORK_FAIL;
	}
	
	haiSet.Pos.x=startPos.x;
	haiSet.Pos.y=startPos.y;
	haiSet.Size.x=(endPos.x-startPos.x);
	haiSet.Size.y=(endPos.y-startPos.y);
	
	//横のサイズと位置を決定したら、次に縦から
	if(endPos.y>=sz_y || startPos.y>=sz_y){
		printf("forkY system over size(%d) error startPos.y=%d endPos.y=%d ", sz_y,startPos.y ,endPos.y);
		return FORK_FAIL;
	}
	
	printf("fork Success! startPos (%d %d) EndPos (%d %d) \n",startPos.x,startPos.y,endPos.x,endPos.y);

	return FORK_SUCCESS;
}
int extractSampleHai(int imgWidth,int imgHeight,unsigned char *judgeImg,class HiStatus *HiArray){
	
	int count=0;
	int currentNum=0;
	int checkDetectorPower=0;
	int checkDetectorPowerInternal=0;
	int retFork=0;
	class HiStatus HiSt;
	class HiStatus currentSt;
	class HiStatus nextSt;
	class HiStatus HiInit;
	class HiStatus firstSt;
	class HiStatus haiSet;
	
	
	
	printf("Sample Hai detecter start...\n");

	retFork=forkSystemMain(judgeImg,imgWidth,imgHeight,haiSet,SMAPLE_FORK_MODE);
	
	if(retFork==FORK_FAIL){
		printf("fork not found error\n");
		return 0;
	}
	
	
	currentSt.Pos.x=haiSet.Pos.x;
	currentSt.Pos.y=haiSet.Pos.y;
	currentSt.Size.x=haiSet.Size.x/9;
	currentSt.Size.y=haiSet.Size.y/4;
	currentSt.feature1=255;
	
	if( currentSt.Size.x == 0 || currentSt.Size.y == 0 ){
		printf("fork size error %d %d\n",currentSt.Size.x,currentSt.Size.y);
		return 0;
	}
	printf("*****Welcome to Extracttion Hai\n");
	
	printf("FirstHai (%d %d) (%d %d) =%d\n",currentSt.feature1,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
	firstSt=currentSt;
	
	currentSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y,10,10,SAMPLE_GETPOS);
	
	if(currentSt.feature1<100){
		printf(" low power \n");
			return 0;
	}
	printf("-----Caliculation -----------n");
	checkDetectorPower=getRectanglePower(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x-10,currentSt.Pos.y-10, currentSt.Size.x+20,currentSt.Size.y+20,FIRST_SAMPLE_GETPOS_CHECK);
	checkDetectorPowerInternal=getRectanglePower(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x+6,currentSt.Pos.y+6, currentSt.Size.x-12,currentSt.Size.y-12,FIRST_SAMPLE_GETPOS_INTERNAL_CHECK);
	printf("checkDetectorPower in Sample=external %d internal %d signal %d\n",checkDetectorPower,checkDetectorPowerInternal,currentSt.feature1);
	if(checkDetectorPower > 20 || checkDetectorPowerInternal >50){
		
		printf("check power is low\n");
		return 0;
	}
		
	// kozawa add s
	printf("debug print: firstSt   -> size.x=%d size.y=%d",firstSt.Size.x,firstSt.Size.y);
	printf("debug print: currentSt -> pos.x=%d pos.y=%d size.x=%d size.y=%d",currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
	if( firstSt.Size.x == 0 || firstSt.Size.y == 0 ){
		printf("size is 0\n");
		return 0;
	}
	// kozawa add e
	
	int flagError=0;
	for(int i=0;i<4;i++){
		for(int j=0;j<9;j++){
			
			if(!(currentSt.Pos.x>=0 && currentSt.Pos.y >=0 && currentSt.Pos.x+currentSt.Size.x<imgWidth &&  currentSt.Pos.y+currentSt.Size.y<imgHeight)){
				printf("Error Extract Hai (%d %d)= (%d %d ) : (%d  %d)\n",j,i,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
				flagError=1;
				break;
			}else if(currentSt.Size.y/firstSt.Size.y < 0.85 && currentSt.Size.y/firstSt.Size.y >1.2){
				printf("Error Size Error %f %f\n",currentSt.Size.x/firstSt.Size.x ,currentSt.Size.x/firstSt.Size.x);
				flagError=1;
				break;
			}
			printf("j=%d i=%d ",j,i);
			
			if(j==0){
				if(i!=0){
					HiSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 10,10,SAMPLE_GETPOS);
				}else{
					//初期牌は↑で計算しているので省く
					HiSt=currentSt;
				}
				nextSt.Pos.x=HiSt.Pos.x;
				nextSt.Pos.y=HiSt.Pos.y;
				nextSt.Size.x=HiSt.Size.x;
				nextSt.Size.y=HiSt.Size.y;
					
			}else{
				HiSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 8,8,SAMPLE_GETPOS);
				
			}
			if(HiSt.feature1>CORRALATION_THRESHOLD){
				count++;
				currentSt=HiSt;
			}else{
				flagError=1;
				printf("The signal is lower Threshold %d \n",HiSt.feature1);
				break;
			}
			
			HiArray[currentNum++]=HiSt;
			//ChangeView(outImg ,imgWidth,imgHeight,HiSt.Pos.x,HiSt.Pos.y, HiSt.Size.x,HiSt.Size.y,1);
			currentSt.Pos.x+=currentSt.Size.x;
			//break;
		}
		if(flagError==1){
			break;
		}
		currentSt.Pos.x =nextSt.Pos.x;
		currentSt.Pos.y =nextSt.Pos.y+nextSt.Size.y;
		currentSt.Size.x=nextSt.Size.x;
		currentSt.Size.y=nextSt.Size.y;
		printf(" Extract Hai = (%d %d ) : (%d  %d)\n",currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
		
	}

	printf("SUCCESS!valid_count=%d;",count);
	
	for(int i=0;i<count;i++){
		ChangeView(judgeImg ,imgWidth,imgHeight,HiArray[i].Pos.x,HiArray[i].Pos.y, HiArray[i].Size.x,HiArray[i].Size.y,1);
		
	}
	u_wtfile(judgeImg,"judgeImgResult.raw",imgWidth*imgHeight);

	
	printf(" prgram end\n");
	
	
	
	return count;
}

int extractAgariHai(int imgWidth,int imgHeight,unsigned char *judgeImg,class HiStatus *HiArray,int flag,int &haiHeight){
	
	int totalLen=0;
	char str[1024];

	printf("debug print: imgWidth = %d, imgHeight = %d",imgWidth,imgHeight); // kozawa add
	static int statiInt=0;
	int count=0;
	int currentNum=0;
	int checkDetectorPower=0;
	int checkDetectorPowerInternal=0;
	int retFork=0;
	int retCount=0;

	class HiStatus HiSt;
	class HiStatus currentSt;
	class HiStatus haiSet;
	
	
	
	//Error Extract Hai (0 0)= (65 40 ) : (58  75)
	printf("*****Welcome to Extracttion Hai...%d %d\n",imgWidth,imgHeight);
	
	retFork=forkSystemMain(judgeImg,imgWidth,imgHeight,haiSet,AGARI_FORK_MODE);
	if(retFork==FORK_FAIL){
		printf("fork error\n");
		
		return 0;
	}
	if(flag==MODE_TEHAI){
		printf("------TEHAI----");
		printf("TEHAI_FORK:pos (%d %d),size (%d %d)\n",haiSet.Pos.x,haiSet.Pos.y,haiSet.Size.x,haiSet.Size.y);
	}else if(flag==MODE_NAKI){
		printf("------NAKI-----");
		printf("NAKI_FORK:pos (%d %d),size (%d %d)\n",haiSet.Pos.x,haiSet.Pos.y,haiSet.Size.x,haiSet.Size.y);
	}else if(flag==MODE_DORA){
		printf("-------DORA----");
		printf("DORA_FORK:pos (%d %d),size (%d %d)\n",haiSet.Pos.x,haiSet.Pos.y,haiSet.Size.x,haiSet.Size.y);
	}else{
		printf("mode Error\n");
	}
	
	
	currentSt.Pos.x=haiSet.Pos.x;
	currentSt.Pos.y=haiSet.Pos.y;
	if(flag!=MODE_NAKI){
		currentSt.Size.y=haiSet.Size.y;
	}else{
		currentSt.Size.y=haiHeight;
	}
		
	currentSt.Size.x=haiSet.Size.y*0.75;
	
	if( currentSt.Size.x == 0 || currentSt.Size.y == 0 ){
		printf("fork size erro r%d %d\n",currentSt.Size.x,currentSt.Size.y);
		
		return 0;
	}
	
	if(currentSt.Size.y < imgHeight/2 || currentSt.Size.y >= imgHeight || currentSt.Pos.x > imgWidth/5){
		printf("ForkSystemError size over height= %d or invalid position pos.x =%d \n",currentSt.Size.y,currentSt.Pos.x);
		
		return 0;
	}
	int nakiFlag=EXISTENCE;
	int nakiHeightPos=0;
	
	if(flag!=MODE_NAKI){
		currentSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y,5,5,SAMPLE_GETPOS);
	}else{
		nakiFlag=judgeExistence(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y,nakiHeightPos);
	
		if(nakiFlag!=NAKIEXISTENCE || !(flag==MODE_NAKI)){
			//鳴き牌以外
			currentSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 5,5,AGARI_GETPOS);
			printf("NAKIHAI_NOT:HERE Extract Hai No.%d (%d %d ) : (%d  %d)\n",currentNum,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
			currentSt.feature2=NOTNAKIFLAG;
		}else{
			//鳴き牌：xとyのサイズが違う
			currentSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,nakiHeightPos, currentSt.Size.y,currentSt.Size.x, 5,5,AGARI_GETPOS);
			printf("NAKIHAI:HERE Extract Hai No.%d (%d %d ) : (%d  %d)\n",currentNum,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
			currentSt.feature2=NAKIFLAG;
		}
	}

	printf("detect Hai No. (%d %d ) : (%d  %d) =max =%d\n",currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y,currentSt.feature1);
	haiHeight=currentSt.Size.y;
	if(currentSt.feature1<100){
		printf("Signal Low (%d %d) (%d %d) =%d\n",currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y,currentSt.feature1);
		return 0;
	}

	if(currentSt.feature2==NOTNAKIFLAG){
		checkDetectorPower=getRectanglePower(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x-10,currentSt.Pos.y-10, currentSt.Size.x+20,currentSt.Size.y+20,FIRST_AGARI_GETPOS_CHECK);
		printf("checkDetectorPower at TEHAI in Agari =%d \n",checkDetectorPower);

	}else{
		checkDetectorPower=getRectanglePower(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x-10,currentSt.Pos.y-10, currentSt.Size.x+20,currentSt.Size.y+20,FIRST_AGARI_NAKI_GETPOS_CHECK);		
		
		printf("checkDetectorPower at NAKI in Agari =%d \n",checkDetectorPower);

	}
	currentSt.Size.x+=(currentSt.Size.x)/30;
		currentSt.Size.y+=(currentSt.Size.y)/30;
	
		
	//checkDetectorPowerInternal=getRectanglePower(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x-5,currentSt.Pos.y-5, currentSt.Size.x+10,currentSt.Size.y+10,FIRST_SAMPLE_GETPOS_INTERNAL_CHECK);
		if(checkDetectorPower>20){
		printf("checkDetectorPower low\n");
		/*
		unsigned char tempImg[256*256]={0};
		char str[1024];
		int tmpCount=0;
		for(int i=currentSt.Pos.y-10;i<currentSt.Size.y+20;i++){
			for(int j=currentSt.Pos.x-10;j<currentSt.Size.x+20;j++){
				tempImg[tmpCount++]=judgeImg[j+i*imgWidth];
			}
		}
		
		statiInt++;
		sprintf(str,"/data/data/jp.tonosama.komoki.janscouter/DetectHai%d.raw",statiInt);
		printf("FILE=%s pos(%d %d) size(%d %d)\n",str,currentSt.Pos.x-10,currentSt.Pos.y-10,currentSt.Size.x+20,currentSt.Size.y+20);
		//u_wtfileEx(tempImg,str,256*256);
		u_wtfileEx(judgeImg,str,imgWidth*imgHeight);
		*/
		return 0;
	}

	//printf("checkDetectorPower in Agari=%d %d in signal=%d\n",checkDetectorPower,getRectanglePowerInternal,currentSt.feature1);
	
	
	printf("*******  FORK OK *******\n\n");
	//return 0;
	while(1){


		if(!(currentSt.Pos.x>=0 && currentSt.Pos.y >=0 && currentSt.Pos.x+currentSt.Size.x<imgWidth &&  currentSt.Pos.y+currentSt.Size.y<imgHeight)){
			printf("judge:break Error Extract Hai No.%d (%d %d ) : (%d  %d)\n",currentNum,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
			break;
		}
		
		if(currentNum!=0){
			//printf("currentNum = %d",currentNum);
			nakiFlag=judgeExistence(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y,nakiHeightPos);
			
			 if(nakiFlag==NOEXISTENCE){
				printf("judge:break Hai not found\n");
				break;
			 }else if(nakiFlag!=NAKIEXISTENCE || !(flag==MODE_NAKI)){
				//鳴き牌以外
				HiSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,currentSt.Pos.y, currentSt.Size.x,currentSt.Size.y, 5,5,SAMPLE_GETPOS);
				printf("NAKIHAI_NOT:HERE Extract Hai No.%d (%d %d ) : (%d  %d)\n",currentNum,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
				HiSt.feature2=NOTNAKIFLAG;

			}else if(nakiFlag==NAKIEXISTENCE){
				//鳴き牌
				HiSt=getPosition(judgeImg ,imgWidth,imgHeight,currentSt.Pos.x,nakiHeightPos, currentSt.Size.y,currentSt.Size.x, 5,5,SAMPLE_GETPOS);
				printf("NAKIHAI:HERE Extract Hai No.%d (%d %d ) : (%d  %d)\n",currentNum,currentSt.Pos.x,currentSt.Pos.y,currentSt.Size.x,currentSt.Size.y);
				HiSt.feature2=NAKIFLAG;
			}else{
				printf("UNKNOWN ERROR\n");
			}
		}else{
			//初回は既にやってるのでカット
			HiSt=currentSt;
		}
		
		if(HiSt.feature1>CORRALATION_THRESHOLD){
			currentSt=HiSt;
			//printf("TESTNAKIHAI_extract:%d =%d\n",currentNum,HiSt.feature2);
			HiArray[currentNum]=HiSt;
			//currentSt.Pos.x+=currentSt.Size.x;
			currentSt.Pos.x+=HiSt.Size.x;
			if(HiSt.feature2==NAKIFLAG){
				currentSt.Size.x=HiSt.Size.y;
				currentSt.Size.y=HiSt.Size.x;
				currentSt.Pos.y=HiSt.Pos.y+HiSt.Size.y-HiSt.Size.x;
			}
		}else{
			printf("judge:break HAI DETECT END currentNum=%d\n",currentNum);
				break;
		}
		
		currentNum++;
		
		totalLen+=HiSt.Size.x;
		
		//printf("HAI TOTAL LENGTH END %d > %d \n",currentSt.Pos.x,haiSet.Size.x,haiSet.Pos.x+haiSet.Size.x);
		if(currentSt.Pos.x>haiSet.Pos.x+haiSet.Size.x-10){
			printf("judge:break HAI TOTAL LENGTH END %d > %d \n",currentSt.Pos.x,haiSet.Pos.x+haiSet.Size.x);
			break;
		}
		
		
		
		if(flag==MODE_TEHAI){
			if(currentNum>=MAX_TEHAI){
				break;
			}
		}else if(flag==MODE_NAKI){
			if(currentNum>=MAX_NAKI){
				break;
			}
		}else if(flag==MODE_DORA){
			if(currentNum>=MAX_DORA){
				break;
			}
		}else{
			printf("Error\n");
		}
	
		

	}


	printf("SUCCESS!valid_count=%d;",currentNum);

	for(int i=0;i<currentNum;i++){

		if(HiArray[i].feature1>100){
			//printf("here%d\n",i);
			ChangeView(judgeImg ,imgWidth,imgHeight,HiArray[i].Pos.x,HiArray[i].Pos.y, HiArray[i].Size.x,HiArray[i].Size.y,1);
		}
	}
	u_wtfile(judgeImg,"judgeImgResult.raw",imgWidth*imgHeight);
	
	
	printf("prgram end\n");
	
	
	return currentNum;
}
