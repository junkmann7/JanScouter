#include "akasaka.h"


class CorrelationResult{
public:
	int sampleNo;//サンプル用ナム
	int Judge;//見つかったかどうか
	int currentHaiType;//現在の調べてるハイの種類
	int currentHaiNo;//現在調べてるハイの番号
	int oldHaiType;//前回牌種類
	int oldHaiNo;//前回no
	int maxCorrelatedHaiType; //最大
	int maxCorrelatedHaiNo;
	int maxCorrelation;
	int flagSouzu[9];//チェック済みかどうか 
	int flagPinzu[9];
	int flagWanzu[9];
	int flagJihai[9];
	int CheckedFlagSouzu;
	int CheckedFlagWanzu;
	int CheckedFlagPinzu;
	int CheckedFlagJihai;
	
	CorrelationResult(){
		Judge=CORRALATION_NOTFOUND;
		
		sampleNo=255;
		maxCorrelatedHaiType=255;
		maxCorrelatedHaiNo=255;
		maxCorrelation=255;
		CheckedFlagSouzu=NOT_ALREADY_CHECKED;
		CheckedFlagWanzu=NOT_ALREADY_CHECKED;
		CheckedFlagPinzu=NOT_ALREADY_CHECKED;
		CheckedFlagJihai=NOT_ALREADY_CHECKED;
		
		for(int i=0;i<9;i++){
			flagSouzu[i]=NOT_ALREADY_CHECKED;
			flagPinzu[i]=NOT_ALREADY_CHECKED;
			flagWanzu[i]=NOT_ALREADY_CHECKED;
			flagJihai[i]=NOT_ALREADY_CHECKED;
		}
	}
	void initialize(){
		
		sampleNo=255;
		maxCorrelatedHaiType=255;
		maxCorrelatedHaiNo=255;
		maxCorrelation=255;
		oldHaiType=FIND_WANZU;
		currentHaiType=0;
		
	}
	void setMaxCorrelation(){
		currentHaiType=maxCorrelatedHaiType;
		currentHaiNo=maxCorrelatedHaiNo;
		
	}
	void setDefinedCorrelation(){
		//FIND_PINZU,FIND_WANZU,FIND_SOUZU,FIND_JIHAI
		//前回一致の牌と今回一致の牌が異なっていた場合はその種類はalready checkedとする
		if(oldHaiType!=currentHaiType){
			printf("change haiType\n");
			if(oldHaiType==FIND_PINZU){
				CheckedFlagPinzu=ALREADY_CHECKED;
			}else if(oldHaiType==FIND_WANZU ){
				CheckedFlagWanzu=ALREADY_CHECKED;
			}else if(oldHaiType==FIND_SOUZU ){
				CheckedFlagSouzu=ALREADY_CHECKED;
			}else if(oldHaiType==FIND_JIHAI ){
				CheckedFlagJihai=ALREADY_CHECKED;
			}else{
				printf("setDefined correlatoin Error \n");
			}
		}
			
		//それより前の牌をチェック済みとする
		if(currentHaiType==FIND_PINZU){
			for(int i=0;i<currentHaiNo;i++){
				flagPinzu[i]=ALREADY_CHECKED;
			}
		}else if(currentHaiType==FIND_WANZU){
			for(int i=0;i<currentHaiNo;i++){
				flagWanzu[i]=ALREADY_CHECKED;
			}
		}else if(currentHaiType==FIND_SOUZU){
			for(int i=0;i<currentHaiNo;i++){
				flagSouzu[i]=ALREADY_CHECKED;
			}
		}else if(currentHaiType==FIND_JIHAI){
			for(int i=0;i<currentHaiNo;i++){
				flagJihai[i]=ALREADY_CHECKED;
			}
		}else{
			printf("error undefined erorr\n");
		}
		
		//フラグのカイホウ
		if(CheckedFlagSouzu==TEMPORALILY_CHECKED){
			CheckedFlagSouzu=NOT_ALREADY_CHECKED;
		}
		if(CheckedFlagWanzu==TEMPORALILY_CHECKED){
			CheckedFlagWanzu=NOT_ALREADY_CHECKED;
		}
		if(CheckedFlagPinzu==TEMPORALILY_CHECKED){
			CheckedFlagPinzu=NOT_ALREADY_CHECKED;
		}
		if(CheckedFlagJihai==TEMPORALILY_CHECKED){
			CheckedFlagJihai=NOT_ALREADY_CHECKED;
		}
		//全会一致の牌をセット
		oldHaiType=currentHaiType;
		oldHaiNo=currentHaiNo;
			
	}
	
	void showParameter(){
		printf("---------parameter show --------------\n");
		printf("currentHaiType =%d currentHaiNo=%d maxHaytype=%d maxNo=%d maxCorr=%d oldHaiType=%d,oldhaiNo=%d\n", currentHaiType,currentHaiNo,maxCorrelatedHaiType,maxCorrelatedHaiNo,maxCorrelation,oldHaiType,currentHaiNo);
		printf("Checked wanzu =%d souzu=%d pinzu=%d,jihai=%d\n",CheckedFlagWanzu,CheckedFlagSouzu,CheckedFlagPinzu,CheckedFlagJihai);
		printf("Wanzu :");
		for(int i=0;i<9;i++){
			printf("%d ",flagWanzu[i]);
		}
		printf("\n");
		printf("Souzu :");
		for(int i=0;i<9;i++){
			printf("%d ",flagSouzu[i]);
		}
		printf("\n");
		printf("Pinzu :");
		for(int i=0;i<9;i++){
			printf("%d ",flagPinzu[i]);
		}
		printf("\n");
		printf("Jiahi :");
		for(int i=0;i<9;i++){
			printf("%d ",flagJihai[i]);
		}
		printf("\n");
		
	}
	
	int findingHai(){
	//ワンズチェック
	if(CheckedFlagWanzu==NOT_ALREADY_CHECKED && CheckedFlagWanzu!=TEMPORALILY_CHECKED){
		printf("wanzu\n");
		for(int i=0;i<9;i++){
			if(flagWanzu[i]==NOT_ALREADY_CHECKED){
				currentHaiType=FIND_WANZU;
				currentHaiNo=i;
				break;
			}
		}
		CheckedFlagWanzu=TEMPORALILY_CHECKED;
	//ソーズチェック
	}else if(CheckedFlagSouzu==NOT_ALREADY_CHECKED && CheckedFlagSouzu!=TEMPORALILY_CHECKED){
		printf("wanzu\n");
		for(int i=0;i<9;i++){
			if(flagSouzu[i]==NOT_ALREADY_CHECKED){
				currentHaiType=FIND_SOUZU;
				currentHaiNo=i;
				break;
			}
		}
		CheckedFlagSouzu=TEMPORALILY_CHECKED;
	
	//ピンズチェック
	}else if(CheckedFlagPinzu==NOT_ALREADY_CHECKED && CheckedFlagPinzu!=TEMPORALILY_CHECKED){
		printf("pinzu\n");

		for(int i=0;i<9;i++){
			if(flagPinzu[i]==NOT_ALREADY_CHECKED){
				currentHaiType=FIND_PINZU;
				currentHaiNo=i;
				break;
			}
		}
		CheckedFlagPinzu=TEMPORALILY_CHECKED;
	//ジハイチェック
	}else if(CheckedFlagJihai==NOT_ALREADY_CHECKED && CheckedFlagJihai != TEMPORALILY_CHECKED){
			printf("jihai\n");

		for(int i=0;i<7;i++){
			if(flagJihai[i]==NOT_ALREADY_CHECKED){
				currentHaiType=FIND_JIHAI;
				currentHaiNo=i;
				break;
			}
		}
		CheckedFlagJihai=TEMPORALILY_CHECKED;
	}else{
		printf("not found Hai\n");
		return ALL_RESERCHED;
	}
		printf("HaiType =%d HaiNo=%d\n",currentHaiType,currentHaiNo);
		return NOT_ALL_RESERCHED;
		
}
};

int calcSumOfSquaredDifference(unsigned char *X , unsigned char *Y,int width,int height){
	float retSSD=0.0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			retSSD+=abs(X[j+i*width]-Y[j+i*width]);
		}
	}
	return (int)(retSSD/width/height);
			
}
//X is tareget Img , Y is sample Img
int calcSumOfSquaredDifference2(unsigned char *X , unsigned char *Y,int width,int height){
	float retSSD=0.0;
	int count=0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(X[j+i*width]>16){
				retSSD+=abs(X[j+i*width]-Y[j+i*width]);
				count++;
			}
		}
	}
	//return (int)(retSSD/width/height);
	return (int)(retSSD/count);		
}
int calcSumOfSquaredDifferenceEx(unsigned char *X , unsigned char *Y,int width,int height){
	float retSSD=0.0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(X[j+i*width]>128 && Y[j+i*width]>128){
				retSSD+=255;
			}
		}
	}
	return (int)(retSSD/width/height);
			
}
int calcSumOfSquaredDifferenceImg(unsigned char *X , unsigned char *Y,int width,int height,unsigned char *out){
	float retSSD=0.0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(X[j+i*width]==Y[j+i*width]){
				out[j+i*width]=255;
			}else{
				out[j+i*width]=0;
			}
		}
	}
	return (int)(retSSD/width/height);
			
}

void preImageProcessForMathing(int imgWidth,int imgHeight,unsigned char *inImg, unsigned char *outImg){
	unsigned char *edgeImg;
	unsigned char *binaryImg;
	unsigned char *gausuImg;
	unsigned char *negativeImg;
	unsigned char *edgeImg2;
	
	printf("Extracting...  %d %d\n",imgWidth,imgHeight);
	
	binaryImg =new unsigned char [imgWidth * imgHeight];
	gausuImg = new unsigned char[imgWidth *imgHeight];
	negativeImg = new unsigned char [imgWidth *imgHeight];
	edgeImg2=new unsigned char [imgWidth * imgHeight];

	#if 0
	//オリジナル
	u_wtfile(inImg,"inImg.raw",imgWidth*imgHeight);
	
	//ガウシアン
	filterGaussian(inImg,gausuImg,imgWidth,imgHeight,PREWITT);
	u_wtfile(gausuImg,"gausu.raw",imgWidth*imgHeight);
	
		//反転
	filterNegative(gausuImg,negativeImg,imgWidth,imgHeight);
	u_wtfile(negativeImg,"nagative.raw",imgWidth*imgHeight);

	//エッジ	
	filterEdge(negativeImg,edgeImg2,imgWidth,imgHeight,PREWITT);
	u_wtfile(edgeImg2,"edge.raw",imgWidth*imgHeight);
	
	//２値化
	binarizeImage(edgeImg2,outImg,imgWidth,imgHeight,64);
	u_wtfile(outImg,"binary.raw",imgWidth*imgHeight);

	#endif
	//ガウシアン
	filterGaussian(inImg,outImg,imgWidth,imgHeight,PREWITT);

	delete [] binaryImg;
	delete [] gausuImg;
	delete [] negativeImg;
	delete [] edgeImg2;

}

int getElement(unsigned char *in,int pos_x,int pos_y,int sz_x, int sz_y){
	if(pos_x >= 0 && pos_x < sz_x && pos_y >= 0 && pos_y < sz_y){
		return in[pos_x+pos_y*sz_x];
	}else{
		return 0;
	}
}

static int exludeWidth=1;
static int lineWidth=4;
void differentialEdgeX(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float threshold,int width ){
	float point_pre=0;
	float point_aft=0;
	for(int i=exludeWidth;i<sz_y-exludeWidth;i++){
		for(int j=sz_x-exludeWidth;j>sz_x/2-exludeWidth;j--){
			point_pre=0.1;//0除算
			point_aft=0.1;
		
			point_pre+=(float)getElement(in,j,i,sz_x,sz_y);
			point_aft+=(float)getElement(in,j-width,i,sz_x,sz_y);
			
			if(point_aft/point_pre > threshold || point_pre/point_aft >threshold){
				for(int k=0;k<lineWidth;k++){
					out[j-k+i*sz_x]=255;
				}
				break;
			}
		}
		
	}
	

	for(int i=exludeWidth;i<sz_y-exludeWidth;i++){
		for(int j=exludeWidth;j<sz_x/2-exludeWidth;j++){
			point_pre=0.1;//0除算
			point_aft=0.1;
			
			
			point_pre+=(float)getElement(in,j,i,sz_x,sz_y);
			point_aft+=(float)getElement(in,j+width,i,sz_x,sz_y);
			
			if(point_aft/point_pre >threshold ||  point_pre/point_aft >threshold){
				for(int k=0;k<lineWidth;k++){
					out[j+k+i*sz_x]=255;
				}
				break;
			}
		}
		
	}


}
void differentialEdgeY(unsigned char *in,unsigned char *out,int sz_x, int sz_y,float threshold,int width){
	//printf("**here\n");
	float point_pre=0;
	float point_aft=0;
	
	for(int j=exludeWidth;j<sz_x-exludeWidth;j++){
		for(int i=sz_y-exludeWidth;i>=sz_y/2-exludeWidth;i--){
			point_pre=0.1;//0除算
			point_aft=0.1;
		
			point_pre+=(float)getElement(in,j,i,sz_x,sz_y);
			point_aft+=(float)getElement(in,j,i-width,sz_x,sz_y);
			if(point_aft/point_pre >threshold ||  point_pre/point_aft >threshold){
				for(int k=0;k<lineWidth;k++){
					out[j+(i-k)*sz_x]=255;
				}
				break;
			}
			
			//printf("%d %d\n",j,i);
		}
		
	}

	for(int j=exludeWidth;j<sz_x-exludeWidth;j++){
		for(int i=exludeWidth;i<sz_y/2-exludeWidth;i++){
			point_pre=0.1;//0除算
			point_aft=0.1;
			
			point_pre+=(float)getElement(in,j,i,sz_x,sz_y);
			point_aft+=(float)getElement(in,j,i+width,sz_x,sz_y);
			if(point_aft/point_pre >threshold ||  point_pre/point_aft >threshold){
				for(int k=0;k<lineWidth;k++){
					out[j+(i+k)*sz_x]=255;
				}
				break;
			}
		}
		
		
	}
	
}
void onlyOutsideEdgeFilter(unsigned char *in, unsigned char *out,int imgWidth,int imgHeight){
	
	unsigned char *edgeHorizonImg;
	unsigned char *edgeVerticalImg;
	
	edgeHorizonImg = new unsigned char [imgWidth*imgHeight];
	edgeVerticalImg = new unsigned char [imgWidth*imgHeight];

	for(int i=0;i<imgWidth*imgHeight;i++){
		out[i]=0;
		edgeHorizonImg[i]=0;
		edgeVerticalImg[i]=0;
	}
	
	//水平方向エッジ
	differentialEdgeX(in,edgeHorizonImg,imgWidth,imgHeight,1.1,2);
	//u_wtfile(edgeHorizonImg,"orgEdgeX.raw",imgWidth*imgHeight);
	
	//垂直方向エッジ
	differentialEdgeY(in,edgeVerticalImg,imgWidth,imgHeight,1.1,2);
	//u_wtfile(edgeVerticalImg,"orgEdgeY.raw",imgWidth*imgHeight);
	
	//合体
	for(int i=0;i<imgWidth*imgHeight;i++){
		if(edgeHorizonImg[i] || edgeVerticalImg[i]){
			out[i]=255;
		}else{
			out[i]=0;
		}
	}
	
	delete [] edgeHorizonImg;
	delete [] edgeVerticalImg;
	
}
void prewittFilter(unsigned char *inImg, unsigned char *outImg,int imgWidth,int imgHeight){
	unsigned char *edgeImg2;
	unsigned char *negativeImg;

	
	
	//printf("Extracting...  %d %d\n",imgWidth,imgHeight);
	negativeImg = new unsigned char [imgWidth *imgHeight];
	edgeImg2=new unsigned char [imgWidth * imgHeight];

	//反転
	filterNegative(inImg,negativeImg,imgWidth,imgHeight);
	u_wtfile(negativeImg,"nagative.raw",imgWidth*imgHeight);

	//エッジ
	//filterEdge(negativeImg,edgeImg2,imgWidth,imgHeight,PREWITT);
	u_wtfile(edgeImg2,"edge.raw",imgWidth*imgHeight);
	filterEdge(negativeImg,outImg,imgWidth,imgHeight,PREWITT);

	//２値化
	/* 一時的
	binarizeImage(edgeImg2,outImg,imgWidth,imgHeight,64);
	u_wtfile(outImg,"binary.raw",imgWidth*imgHeight);
	*/

	delete [] negativeImg;
	delete [] edgeImg2;

}

void matchingFilter(unsigned char *in, unsigned char *out,int imgWidth,int imgHeight,int matchingFilterType){
	
	if(matchingFilterType==OUTSIDE_METHOD){
		onlyOutsideEdgeFilter(in,out,imgWidth,imgHeight);
	}else if(matchingFilterType==PREWITT_METHOD){
		prewittFilter(in,out,imgWidth,imgHeight);
	}else{
		printf("FILTER ERROR\n");
	}
	
}
	
char *getHaiType(int haiType,char *Name){
	switch(haiType){
	case FIND_PINZU:
		sprintf(Name,"PINZU");
		break;
	case FIND_WANZU:
		sprintf(Name,"WANZU");
		break;
	case FIND_SOUZU:
		sprintf(Name,"SOUZU");
		break;
	case FIND_JIHAI:
		sprintf(Name,"JIHAI");
		break;
	default:
		printf("get hai error\n");
		break;
	}
	return Name;
}
		
char *getHaiName(int haiNo, char *Name){
	switch(haiNo){
	case 0:
		sprintf(Name,"WANZU_1");
		break;
	case 1:
		sprintf(Name,"WANZU_2");
		break;
	case 2:
		sprintf(Name,"WANZU_3");
		break;
	case 3:
		sprintf(Name,"WANZU_4");
		break;
	case 4:
		sprintf(Name,"WANZU_5");
		break;
	case 5:
		sprintf(Name,"WANZU_6");
		break;
	case 6:
		sprintf(Name,"WANZU_7");
		break;
	case 7:
		sprintf(Name,"WANZU_8");
		break;
	case 8:
		sprintf(Name,"WANZU_9");
		break;
	case 9:
		sprintf(Name,"SOUZU_1");
		break;
	case 10:
		sprintf(Name,"SOUZU_2");
		break;
	case 11:
		sprintf(Name,"SOUZU_3");
		break;
	case 12:
		sprintf(Name,"SOUZU_4");
		break;
	case 13:
		sprintf(Name,"SOUZU_5");
		break;
	case 14:
		sprintf(Name,"SOUZU_6");
		break;
	case 15:
		sprintf(Name,"SOUZU_7");
		break;
	case 16:
		sprintf(Name,"SOUZU_8");
		break;
	case 17:
		sprintf(Name,"SOUZU_9");
		break;
	case 18:
		sprintf(Name,"PINZU_1");
		break;
	case 19:
		sprintf(Name,"PINZU_2");
		break;
	case 20:
		sprintf(Name,"PINZU_3");
		break;
	case 21:
		sprintf(Name,"PINZU_4");
		break;
	case 22:
		sprintf(Name,"PINZU_5");
		break;
	case 23:
		sprintf(Name,"PINZU_6");
		break;
	case 24:
		sprintf(Name,"PINZU_7");
		break;
	case 25:
		sprintf(Name,"PINZU_8");
		break;
	case 26:
		sprintf(Name,"PINZU_9");
		break;
	case 27:
		sprintf(Name,"TONN");
		break;
	case 28:
		sprintf(Name,"NANN");
		break;
	case 29:
		sprintf(Name,"SHA");
		break;
	case 30:
		sprintf(Name,"PEI");
		break;
	case 31:
		sprintf(Name,"HAKU");
		break;
	case 32:
		sprintf(Name,"HATU");
		break;
	case 33:
		sprintf(Name,"TYUN");
		break;
	default:
		sprintf(Name,"ERROR=%d",haiNo);
		break;
	}
	
	return Name;

		
}
//サンプルデータから取得
void getHaiImgFromArray(int haiNo,unsigned char *sampleImg,unsigned char *haiImg){
	int haiSize=CONST_HAI_WIDTH*CONST_HAI_HEIGHT;
	int startPos=CONST_HAI_WIDTH*CONST_HAI_HEIGHT*haiNo;
	int count=0;
	for(int i=startPos;i<startPos+haiSize;i++){
		haiImg[count++]=sampleImg[i];
	}
}
//classタイプから配列順に変換
int convertClassToSampleType(int haiType,int No){
	switch(haiType){
	case FIND_PINZU:
		return No;
		break;
	case FIND_WANZU:
		return WANZU_START +No;
		break;
	case FIND_SOUZU:
		return SOUZU_START+No;
		break;
	case FIND_JIHAI:
		return JIHAI_START+No;
		break;
	}
	printf("Error conversion class to sample\n");
	return 0;
}
//配列並びからclassタイプに変換
void convertSampleTypeToClass(int &haiType,int &No,int sampleNum){
	printf("convertSampleTypeToClass=%d\n",sampleNum);
	if(sampleNum<9){
		haiType=FIND_WANZU;
		No=sampleNum-WANZU_START;
	}else if(sampleNum<18){
		haiType=FIND_SOUZU;
		No=sampleNum-SOUZU_START;
	}else if(sampleNum<27){
		haiType=FIND_PINZU;
		No=sampleNum;
	}else if(sampleNum<34){
		haiType=FIND_JIHAI;
		No=sampleNum-JIHAI_START;
	}else{
		printf("Error convert sample to class\n");
	}
}


void comparedByCategolized(unsigned char *targetHai,int startPos,int EndPos,unsigned char *sampleImg,class CorrelationResult *currentSt){
	//float minCorrationValue=255.0;
	//int minCorrationHaiNo=0;
	char str[1024];
	int ret=0;
	int width=0;
	int height=0;
	width=CONST_HAI_WIDTH;
	height=CONST_HAI_HEIGHT;
	
	unsigned char sampleHai[CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	for(int i=startPos;i<EndPos;i++){
		currentSt->sampleNo=i;
		printf("i=%d currentSt->sampleNo=%d\n",i,currentSt->sampleNo);
	
		getHaiImgFromArray(currentSt->sampleNo, sampleImg,sampleHai);
		if(currentSt->currentHaiType==FIND_WANZU){
			width/=2;
		}
//		ret=calcSumOfSquaredDifference(targetHai,sampleHai,width,height);
		ret=calcSumOfSquaredDifference2(targetHai,sampleHai,width,height);
		printf("***corralation =%d\n",ret);
		if(ret<=10.0){

			currentSt->maxCorrelation=ret;
			currentSt->sampleNo=i;
			currentSt->Judge=CORRALTION_FOUND;
			convertSampleTypeToClass(currentSt->maxCorrelatedHaiType,currentSt->maxCorrelatedHaiNo,currentSt->sampleNo);
			printf("Found %s with high corralation %d %d\n",getHaiName(currentSt->sampleNo,str),currentSt->maxCorrelatedHaiType,currentSt->maxCorrelatedHaiNo);
			return ;
		}else if(ret < currentSt->maxCorrelation){
			currentSt->maxCorrelation=ret;
			currentSt->sampleNo=i;
		}
	}
	printf("The target is not found in this type\n");

	convertSampleTypeToClass(currentSt->maxCorrelatedHaiType,currentSt->maxCorrelatedHaiNo,currentSt->sampleNo);
	return ;
}
		
void mathingExByCategolized(unsigned char *targetHai,int type, int No,unsigned char *sampleImg,class CorrelationResult *currentSt){
	class CorrelationResult corrRet;
	//unsigned char *targetHai;
	int startPos=0;
	int endPos=0;
	if(type==FIND_PINZU){
		startPos=PINZU_START+No;
		endPos=PINZU_START+9;
		printf("FINDING.... PINZU from %d to %d \n",startPos,endPos);
		comparedByCategolized(targetHai,startPos,endPos , sampleImg,currentSt);
		
	}else if(type==FIND_WANZU){

		startPos=WANZU_START+No;
		endPos=WANZU_START+9;
		printf("FINDING.... WANNZU from %d to %d \n",startPos,endPos);
		comparedByCategolized(targetHai, startPos,endPos , sampleImg,currentSt);			
		
	}else if(type==FIND_SOUZU){
		startPos=SOUZU_START+No;
		endPos=SOUZU_START+9;
		printf("FINDING.... SOUZU from %d to %d \n",startPos,endPos);
		comparedByCategolized(targetHai,startPos,endPos , sampleImg,currentSt);			
		
	}else if(type==FIND_JIHAI){
;
		startPos=JIHAI_START+No;
		endPos=JIHAI_START+7;
		printf("FINDING.... JIHAI from %d to %d \n",startPos,endPos);
		comparedByCategolized(targetHai, startPos, endPos, sampleImg,currentSt);			
		
	}else{
		printf("mathing categolized error\n");
	}
	
	return ;
}




void mathingEx(unsigned char *agariImg,int targetNum,unsigned char *sampleImg,int resultArray[]){
	//はじめの牌を全検索で見つける
	//その牌の種類を調べる
	//その牌ではない場合は、マッチング対象のもを探す
	class CorrelationResult currentSt;
	//class *ptCurrentSt=currentSt;
	unsigned char *targetHai;
	char Name[1024];
	int ret=0;
	targetHai= new unsigned char [CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	
	for(int i=0;i<3;i++){
		//ターゲットを選択
		getHaiImgFromArray(i, agariImg,targetHai);
		//相関用パラメータの初期化
		currentSt.initialize();
		currentSt.showParameter();
		
		while(1){
			//検索すべき牌を決定
			ret=currentSt.findingHai();
				
			if(ret!=ALL_RESERCHED){
				//その牌の種類を検索する
				mathingExByCategolized(targetHai,currentSt.currentHaiType,currentSt.currentHaiNo,sampleImg,&currentSt);
			}else{
				//今までの最大相関をセット
				currentSt.currentHaiType=currentSt.maxCorrelatedHaiType;
				currentSt.currentHaiNo=currentSt.maxCorrelatedHaiNo;
				currentSt.Judge=CORRALTION_FOUND;
			}
			
			if(currentSt.Judge==CORRALTION_FOUND){
				currentSt.setMaxCorrelation();
				//見つかったので、その種類のそれまでをALREADYとする
				currentSt.setDefinedCorrelation();
				break;
			}else{
				//見つからなかったので、他のものを調べるためにこれまでの相関最大値とそのハイの種類、ハイの数をセット
				currentSt.setMaxCorrelation();
			}
		}
		currentSt.showParameter();
		printf("RESULT %d %d\n",currentSt.maxCorrelatedHaiType,currentSt.maxCorrelatedHaiNo);
		printf("Result is %s_%d\n",getHaiType(currentSt.maxCorrelatedHaiType,Name),currentSt.maxCorrelatedHaiNo+1);
		resultArray[i]=convertClassToSampleType(currentSt.maxCorrelatedHaiType,currentSt.maxCorrelatedHaiNo);
		
	}
	delete [] targetHai;
}
void shiftImage(unsigned char *in,unsigned char *out,int width,int height,int shift_x,int shift_y){
	int mod_x;
	int mod_y;
	
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			mod_x=j+shift_x;
			mod_y=i+shift_y;
			if(mod_x >=0 && mod_y>=0 && mod_x< width && mod_y <height){
				out[j+i*width]=in[mod_x+mod_y*width];
			}else{
				out[j+i*width]=0;
			}
		}
	}
}
int judgeHaku(unsigned char *Hai,int width,int height){
	int count=0;
	for(int i=0;i<width*height;i++){
		if(Hai[i]>64){
			count++;
		}
	}
	printf("count %d\n",count);
	int threshold=width*height*0.07;
	if(count<threshold){
		printf("found count HAKU < %d \n",threshold);
		return judgedHaku;
	}
	return judgedOther;

}


float compareEx(unsigned char *targetHai, unsigned char *sampleHai, int width, int height,int distanceX,int distanceY ){
	unsigned char *shiftHai;
	shiftHai = new unsigned char [width*height];
	float maxCorr=255.0;
	float ret=0.0;
	int max_x;int max_y;
	//移動量０は計算しないとかにすれば？
	for(int y=-distanceY ;y<distanceY+1 ; y+=3){
		for(int x=-distanceX;x<distanceX+1;x+=3){
			shiftImage(targetHai,shiftHai,width,height,x,y);
			ret=calcSumOfSquaredDifference(shiftHai,sampleHai,CONST_HAI_WIDTH,CONST_HAI_HEIGHT);
			//printf("x=%d,y=%d,ret=%d\n",x,y,ret);
			if(maxCorr>ret){
				maxCorr=ret;
				max_x=x;
				max_y=y;
			}
		}
	}
	//printf("max_x=%d, max_y =%d\n",max_x,max_y);
	delete [] shiftHai;
	return maxCorr;
}
void rotImg180(unsigned char *in,unsigned char *out){
	
	int count=0;
	for(int y=CONST_HAI_HEIGHT-1;y>=0;y--){
		for(int x=CONST_HAI_WIDTH-1;x>=0;x--){
				out[count++]=in[x+y*CONST_HAI_WIDTH];
		}
	}
}

void MatchingFilterGroup(unsigned char *orgImg,unsigned char modImg[][CONST_HAI_WIDTH*CONST_HAI_HEIGHT],int num){
	
	unsigned char targetHai[CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	
	for(int i=0;i<num;i++){
		//printf("ok?? %d num=%d",i,num);
		//ターゲットの牌取得
		getHaiImgFromArray(i, orgImg,targetHai);
		//ターゲットの画像処理
		matchingFilter(targetHai,modImg[i],CONST_HAI_WIDTH,CONST_HAI_HEIGHT,PREWITT_METHOD);
	}
	//printf("here2");
}
int judgeSpecialHai(unsigned char *Hai,int width,int height){

	int yArray[height];
	int yArray2[height];
	
	for(int i=0;i<height;i++){
		yArray[i]=0;
		yArray2[i]=0;
	}
	
	int k=0;
	for(int i=0;i<height;i+=3){
		for(int j=width/7;j<width;j++){
			if(Hai[j+i*width]>20){
				yArray[k]=j;
				break;
			}
			
		}
		k++;
	}
	k=0;
	for(int i=0;i<height;i+=3){
		for(int j=(width*6/7);j>=0;j--){
			if(Hai[j+i*width]>20){
				yArray2[k]=j;
				break;
			}
		}
		k++;
	}
	k=0;
	printf("native debug:start");
	int diffdistance=0;
	int slimCount=0;
	int tyunnSwitch=0;
	int tyunnTriger=0;
	int zeroCount=0;
	for(int i=0;i<(height);i+=3){
		diffdistance=yArray2[k]-yArray[k];
		//printf("native debug:y=%d FromLeft=%d FromRight=%d diff=%d",i,yArray[k],yArray2[k],diffdistance);
		if(i> 40 && diffdistance<20 && diffdistance > 5 && yArray2[k] < 48  && yArray[k]>16){
			slimCount++;
		}
		if(yArray2[k]==0 && yArray[k]==0){
			zeroCount++;
		}
		//チュンスイッチオン
		if(i>9 && tyunnSwitch!=2){
			if(diffdistance>30 ){
				tyunnSwitch=1;
			}
		}
		//チュンスイッチオフ
		if(i>9 && i< 70 && tyunnSwitch>0){
			if(diffdistance<30){
				tyunnSwitch=2;
			}
		}
		
		if(tyunnSwitch==1){
			tyunnTriger++;
		}
		k++;
	}
	
	printf("debug zerCount=%d slimCount=%d tyunnTriget=%d",zeroCount,slimCount,tyunnTriger);
	
	if(slimCount > 8 && tyunnTriger <15){
		printf("debug judge Tyunn\n");
		return judgedTyun;
	}else if(zeroCount>15){
		printf("debug judge haku\n");
		return judgedHaku;
	}
	return judgedOther;

}
void easyMatching(unsigned char *agariImg,int targetNum,unsigned char *sampleImg,int resultArray[],int nakiFlagArray[]){
	
	unsigned char *targetHai;
	unsigned char *sampleHai;
	unsigned char *targetHaiForMat;
	unsigned char *sampleHaiForMat;
	int count=0;

	unsigned char nakiRotHai[CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	targetHai= new unsigned char [CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	sampleHai= new unsigned char [CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	
	unsigned char targetHaiFormatArray[targetNum][CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	unsigned char sampleHaiForMatArray[34][CONST_HAI_WIDTH*CONST_HAI_HEIGHT];
	
	char str[1024];
	float ret=0;
	float maxCorre=255;
	int maxNo=0;
	float resultArrayStrong[100]={0};
	printf("---easyMatching---\n");
	
	//テハイ＋ナキハイ
	MatchingFilterGroup(agariImg,targetHaiFormatArray,targetNum);
	//サンプルハイ
	MatchingFilterGroup(sampleImg,sampleHaiForMatArray,34);
	
	char strTmp[1024];
	for(int i=0;i<targetNum;i++){
		sprintf(strTmp,"/data/data/jp.tonosama.komoki.janscouter/debugedTyun%d.raw",i);
		//printf(strTmp);
		u_wtfileEx(targetHaiFormatArray[i],strTmp,CONST_HAI_WIDTH*CONST_HAI_HEIGHT);
	}
	
	printf("start matching");
	for(int targetNo=0;targetNo<targetNum;targetNo++){
		
		maxCorre=1000;
		maxNo=0;
		printf("debug targetNo=%d",targetNo);
		ret=judgeSpecialHai(targetHaiFormatArray[targetNo],CONST_HAI_WIDTH,CONST_HAI_HEIGHT);
		
		if(ret==judgedHaku){
			maxNo=31;
		}else if(ret==judgedTyun){
			maxNo=33;
		}else{
	
			for(int sampleNo=0;sampleNo<34;sampleNo++){
				
				//白以外
				if(sampleNo!=31){
					//鳴き牌用に画像生成
					if(nakiFlagArray[targetNo]==NAKIFLAG){
						rotImg180(sampleHaiForMatArray[sampleNo],nakiRotHai);
					}
					for(int k=0;k<2;k++){
						if(k==0){
							ret=compareEx(sampleHaiForMatArray[sampleNo],targetHaiFormatArray[targetNo],CONST_HAI_WIDTH,CONST_HAI_HEIGHT,9,9);
							//鳴き牌でない場合はbreakさせる
							if(nakiFlagArray[targetNo]!=NAKIFLAG){
								k=2;
							}
						}else{
							printf("nakihai found\n");
							ret=compareEx(nakiRotHai,targetHaiFormatArray[targetNo],CONST_HAI_WIDTH,CONST_HAI_HEIGHT,9,9);
						}
						printf("The correlation of %s(%d) is %f \n",getHaiName(sampleNo,str),sampleNo, ret);
						if(maxCorre>ret){
							maxCorre=ret;
							maxNo=sampleNo;
						}
					}
				}
			}
		}
		
		printf("mat:The result is  %s (signal=%f) \n",getHaiName(maxNo,str),maxCorre);
		resultArray[targetNo]=maxNo;
		resultArrayStrong[targetNo]=maxCorre;
		//break;
	}
	for(int i=0;i<targetNum;i++){
		printf("TheResult of %d is  %s(%d) =%f\n",i,getHaiName(resultArray[i],str),resultArray[i],resultArrayStrong[i]);
	}

	delete [] targetHai;
	delete [] sampleHai;
}
int juntuTest(int &hai1,int &hai2,int &hai3){
	if(hai1+2==hai3 && hai2+1==hai3){
		printf("CORRECT JUNTU\n");
	}else if(hai1+1 == hai2){
		hai3=hai1+2;
	}else if(hai1+2 == hai3){
		hai2=hai1+1;
	}else if(hai2+1 == hai3){
		hai1=hai2-1;
	}else{
		printf("mentuError\n");
	}
}
void showHaiSet(int testHai[]){
	char str[100];
	for(int i=0;i<14;i++){
		printf("HOSEI:No.%d  %s\n",i,getHaiName(testHai[i],str));
	}
}
int testHosei(int testHai[]){
	char str[100];
	char str2[100];
	char str3[100];
	int mentu1[4][3];
	int jantou[2];
	int oldHai=0;
	int tempHai[14]={0};
	int count=0;
	int haiSet[3]={0};
	int Jantou[2]={0};
	int janFlag=0;
	showHaiSet(testHai);
	//頭を外す
	printf("juntu search\n");
	for(int i=0;i<13;i++){
		if(testHai[i]==testHai[i+1]){
			Jantou[0]=i;
			Jantou[1]=i+1;
			janFlag=1;
			break;
		}
	
	}
	
	if(janFlag==0){
		printf("not found Juntou\n");
		return 0;
	}
	printf("JANTOU = %s(%d)   %s(%d)\n",getHaiName(testHai[Jantou[0]],str),Jantou[0],getHaiName(testHai[Jantou[1]],str2),Jantou[1]);
	//ジュンツを探す
	count=0;
	for(int i=0;i<14;i++){
		if(i!=Jantou[0] && i!=Jantou[1]){
			haiSet[count++]=i;
			if(count==3){
				printf("SET_MENTU:%s %s %s \n",getHaiName(testHai[i-2],str),getHaiName(testHai[i-1],str2),getHaiName(testHai[i],str3));
				juntuTest(testHai[haiSet[0]],testHai[haiSet[1]],testHai[haiSet[2]]);
				count=0;
			}
			
		}
	}
	showHaiSet(testHai);
	return 1;
}
void writeAgariHai(unsigned char *img,int num,const char *TAG){
	unsigned char *tempHai;
	int count=0;
	char str[1024];
	int mSize=CONST_HAI_WIDTH*CONST_HAI_HEIGHT;
	tempHai = new unsigned char [mSize];
	
	for(int i=0;i<num;i++){
		count=0;
		for(int j=i*mSize;j<i*mSize+mSize;j++){
			tempHai[count++]=img[j];
		}
		//printf("count=%d\n",count);
		sprintf(str,"hai/%s_%d.raw",TAG,i);
		u_wtfile(tempHai, str, mSize);
	}
		
	delete [] tempHai;
	return ;
}


int agariMain(int imgWidth,int imgHeight,unsigned char *preProcessImg, class AgariHai &agariHaiArray,int tehaiPosY,int nakiPosY,int guideWakuY,unsigned char* tehaiHai,unsigned char* nakiHai ){
	long int countNaki=0;
	long int countTehai=0;
	//long int countDora=0;
	int haiHeight;
	
	unsigned char *tehaiImg;
	//unsigned char *doraImg;
	unsigned char *nakiImg;
	unsigned char *processedTehaiImg;
	//unsigned char *processedDoraImg;
	unsigned char *processedNakiImg;
	
			
	tehaiImg=  new unsigned char [imgWidth * guideWakuY];
	//doraImg=  new unsigned char [imgWidth * guideWakuY];
	nakiImg= new unsigned char [imgWidth * guideWakuY];
	processedTehaiImg=  new unsigned char [imgWidth * guideWakuY];
	//processedDoraImg=  new unsigned char [imgWidth * guideWakuY];
	processedNakiImg= new unsigned char [imgWidth * guideWakuY];
	
	
	int resultTehai[MAX_TEHAI]={0};
	//int resultDora[MAX_DORA]={0};
	int reusltNaki[MAX_NAKI]={0};
	//mergedHai= new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*(MAX_NAKI+MAX_DORA+MAX_NAKI)];
	
	//printf("agari main... %d %d  doraY=%d tehaiY=%d nakiY=%d  WAKU_%d\n",imgWidth,imgHeight,doraPosY,tehaiPosY,nakiPosY,guideWakuY);
	printf("agari main... %d %d  tehaiY=%d nakiY=%d  WAKU_%d\n",imgWidth,imgHeight,tehaiPosY,nakiPosY,guideWakuY);
	
	for(int i=0;i<imgHeight;i++){
		for(int j=0;j<imgWidth;j++){
			/*if(i>=doraPosY && i< doraPosY+guideWakuY){
				doraImg[countDora++]=preProcessImg[j+i*imgWidth];
			}*/
			if(i>=tehaiPosY && i<tehaiPosY+guideWakuY){
				tehaiImg[countTehai++]=preProcessImg[j+i*imgWidth];
			}
			if(i>=nakiPosY && i<nakiPosY+guideWakuY){
			
				nakiImg[countNaki++]=preProcessImg[j+i*imgWidth];
			}
		}
	}
	
	//牌検出用画像処理
	preImageProcess(imgWidth,guideWakuY,tehaiImg,processedTehaiImg);
	preImageProcess(imgWidth,guideWakuY,nakiImg,processedNakiImg);
	
	
	//牌検出
	agariHaiArray.tehaiNum=extractAgariHai(imgWidth,guideWakuY,processedTehaiImg,agariHaiArray.TehaiArray,MODE_TEHAI,haiHeight);

	printf("FOUND TEHAIHAI %d\n",agariHaiArray.tehaiNum);
	
	

	agariHaiArray.nakiNum=extractAgariHai(imgWidth,guideWakuY,processedNakiImg,agariHaiArray.NakiArray,MODE_NAKI,haiHeight);
	printf("FOUND NAKI %d\n",agariHaiArray.nakiNum);

	#if 1
	u_wtfileEx(processedTehaiImg,"/data/data/jp.tonosama.komoki.janscouter/processedTehai.raw",imgWidth*guideWakuY);
	//u_wtfileEx(processedDoraImg,"/data/data/jp.tonosama.komoki.janscouter/processedDora.raw",imgWidth*guideWakuY);
	u_wtfileEx(processedNakiImg,"/data/data/jp.tonosama.komoki.janscouter/processedNaki.raw",imgWidth*guideWakuY);
	#endif
	
	//マッチング用
	preImageProcessForMathing(imgWidth,guideWakuY,tehaiImg,processedTehaiImg);
	preImageProcessForMathing(imgWidth,guideWakuY,nakiImg,processedNakiImg);
	
	//整理する
	arrangeHai(imgWidth,guideWakuY,processedTehaiImg,agariHaiArray.tehaiNum,tehaiHai,agariHaiArray.TehaiArray);
	//arrangeHai(imgWidth,guideWakuY,processedDoraImg,agariHaiArray.doraNum,doraHai,agariHaiArray.DoraArray);
	arrangeHai(imgWidth,guideWakuY,processedNakiImg,agariHaiArray.nakiNum,nakiHai,agariHaiArray.NakiArray);

	
	u_wtfile(tehaiHai,"tehaihai1st.raw",CONST_HAI_WIDTH*CONST_HAI_HEIGHT);
	u_wtfile(nakiHai,"naki1st.raw",CONST_HAI_WIDTH*CONST_HAI_HEIGHT);
	//u_wtfile(doraHai,"dora1st.raw",CONST_HAI_WIDTH*CONST_HAI_HEIGHT);
	
	#ifndef NATIVE_DEBUG
	writeAgariHai(tehaiHai,agariHaiArray.tehaiNum,"A_TEHAI");
	//writeAgariHai(doraHai,agariHaiArray.doraNum,"A_DORA");
	writeAgariHai(nakiHai,agariHaiArray.nakiNum,"A_NAKI");
	//writeAgariHai(sampleHai,34,"SAMPLE");
	#endif
	

	//delete [] doraImg;
	delete [] tehaiImg;
	delete [] nakiImg;
	
	delete [] processedTehaiImg;
	//delete [] processedDoraImg;
	delete [] processedNakiImg;
	



	return agariHaiArray.tehaiNum+agariHaiArray.doraNum+agariHaiArray.nakiNum;
	
}
