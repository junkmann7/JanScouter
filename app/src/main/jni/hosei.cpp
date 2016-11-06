#include "akasaka.h"
#include "ogura.h"

//同じ牌が6個あったら計算しない
int checkOverWrong(jbyte *arrTehai,int tehaiNum,jbyte *arrNakihai,int nakiNum){
	int checkHai[100];
	for(int k=0;k<100;k++){
		checkHai[k]=0;
	}
	int ret=EASYCHECK_PASS;
	int i=0;
	int count=0;
	int scope=0;
	for(int i=0;i<tehaiNum;i++){
		printf("hosei:arrTehai[i]=%d\n",arrTehai[i]);
		scope=arrTehai[i];
		checkHai[scope]=checkHai[scope]+1;
		
	}
	for(int i=0;i<nakiNum;i++){
		printf("hosei:arrNaki[i]=%d\n",arrTehai[i]);
		scope=arrNakihai[i];
		checkHai[scope]=checkHai[scope]+1;
		
	}
	
	for(int k=0;k<34;k++){
		printf("hosei:checkHai %d",checkHai[k]);
		if(checkHai[k]>=6){
			ret=EASYCHECK_FAIL;
			printf("hosei:EASYCHECK_ERROR!!!%d = %d\n",k,checkHai[k]);
			break;
			
		}
	}
	printf("hosei:EASYCHECK_PASS!\n");
	
	return ret;

}
int getGroup(int targetHai){
	if(targetHai>=0 && targetHai<9){
		return FIND_WANZU;
	}else if(targetHai>=9 && targetHai<18){
		return FIND_SOUZU;
	}else if(targetHai>=18 && targetHai<27){
		return FIND_PINZU;
	}else if(targetHai>=27 && targetHai<34){
		return FIND_JIHAI;
	}else{
		printf("hosei:Error cannot categolize hai group :targetHai=%d\n",targetHai);
	}
	return FIND_JIHAI;
}	
int judgeSameGroup(int targetHai,jbyte *arrTehai){
	int leftHai=0;
	int rightHai=0;
	if(targetHai==0 || targetHai > 34){
		printf("hosei:Error fatal error ,maybe null exception=%d\n",targetHai);
		return DIFFGROUP;
	}
	leftHai=getGroup(arrTehai[targetHai-1]);
	rightHai=getGroup(arrTehai[targetHai+1]);
	
	if(leftHai==rightHai){
		if(arrTehai[targetHai-1] == arrTehai[targetHai+1]){
			//両隣が同じ牌
			return SAMEHAI;
		}else{
			return SAMEGROUP;
		}
	}else{
		return DIFFGROUP;
	}
	
}

int judgeFukeisan(int targetHaiNo,int HoseiHai,int nakiFlag,jboolean *arrCondition, jbyte *arrTehai,jbyte *arrDora, jbyte *arrNakihai, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist){
	int originalHai=0;
	int ret=0;
	
	if(nakiFlag==HOSEI_TEHAI){
		originalHai=arrTehai[targetHaiNo];
		arrTehai[targetHaiNo]=HoseiHai;
	}else{
		originalHai=arrNakihai[targetHaiNo];
		arrNakihai[targetHaiNo]=HoseiHai;
	}
	
	printf("hosei:fukeisan_main_ettry\n");
	ret=fukeisan_main(arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
	if(ret==1){
		//ハイケイサンできた場合
		printf("hosei:*SUCCESS!\n");
		return 1;
	}
	//違った場合は戻す
	if(nakiFlag==HOSEI_TEHAI){
		arrTehai[targetHaiNo]=originalHai;
	}else{
		arrNakihai[targetHaiNo]=originalHai;
	}
	return 0;
}
int judgeFukeisanEx(int targetHaiNo,int StartHai,int EndHai,int nakiFlag,int hoseiMode,jboolean *arrCondition, jbyte *arrTehai,jbyte *arrDora, jbyte *arrNakihai, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist){
	int ret=0;
	int thisStartHai=0;
	int thisEndHai=0;
	int StartHaiType=getGroup(StartHai);
	int tempHaiType=0;
	int targetHaiObj=0;
	
	char str1[20];
	char str2[20];
	char str3[20];
	printf("hosei:targetHaiNo=%d,StartHai=%d,EndHai=%d,nakiFlag=%d,hoseiMode=%d",targetHaiNo,StartHai,EndHai,nakiFlag,hoseiMode);
	if(hoseiMode!=HOSEI_MODE_JIHAI && hoseiMode!=HOSEI_MODE_ALL){
		//手配
		if(nakiFlag==HOSEI_TEHAI){

			printf("hosei:tehaiMode with  Not jihai And Not All");
			//同じ牌か同じ種類の牌
			if(hoseiMode==HOSEI_MODE_SAMEHAI || hoseiMode==HOSEI_MODE_SAMEGR){
				printf("hosei:sameHai or sameGroup");
				thisStartHai=arrTehai[StartHai];
				thisEndHai=arrTehai[EndHai];
			}
			//左から補正
			else if(hoseiMode==HOSEI_MODE_DIFFGR_LEFT){
				printf("hosei:Diff_from left");
				thisStartHai=arrTehai[StartHai];
				thisEndHai=thisStartHai;
				tempHaiType=getGroup(thisStartHai);
				for(int i=0;i<3;i++){
					if(tempHaiType!=getGroup(arrTehai[StartHai]+i)){
						break;
					}
					thisEndHai=arrTehai[StartHai]+i;
				}
			//右から補正
			}else if(hoseiMode==HOSEI_MODE_DIFFGR_RIGHT){
				printf("hosei:Diff_from right");
				thisEndHai=arrTehai[EndHai];
				thisStartHai=thisEndHai;
				tempHaiType=getGroup(thisEndHai);
				for(int i=0;i<3;i++){
					if(tempHaiType!=getGroup(arrTehai[EndHai]-i)){
						break;
					}
					thisStartHai=arrTehai[EndHai]-i;
				}
			
			}else{
				printf("hosei: mode error\n");
			}
		
		//ナキハイ
		}else{
			printf("hosei:naki_mode");
		
			printf("hosei:tehaiMode with  Not jihai And Not All");
			//同じ牌か同じ種類の牌
			if(hoseiMode==HOSEI_MODE_SAMEHAI || hoseiMode==HOSEI_MODE_SAMEGR){
				printf("hosei:sameHai or sameGroup");
				thisStartHai=arrNakihai[StartHai];
				thisEndHai=arrNakihai[EndHai];
			}
			//左から補正
			else if(hoseiMode==HOSEI_MODE_DIFFGR_LEFT){
				printf("hosei:Diff_from left");
				thisStartHai=arrNakihai[StartHai];
				thisEndHai=thisStartHai;
				tempHaiType=getGroup(thisStartHai);
				for(int i=0;i<3;i++){
					if(tempHaiType!=getGroup(arrNakihai[StartHai]+i)){
						break;
					}
					thisEndHai=arrNakihai[StartHai]+i;
				}
			//右から補正
			}else if(hoseiMode==HOSEI_MODE_DIFFGR_RIGHT){
				printf("hosei:Diff_from right");
				thisEndHai=arrNakihai[EndHai];
				thisStartHai=thisEndHai;
				tempHaiType=getGroup(thisEndHai);
				for(int i=0;i<3;i++){
					if(tempHaiType!=getGroup(arrNakihai[EndHai]-i)){
						break;
					}
					thisStartHai=arrNakihai[EndHai]-i;
				}
			
			}else{
				printf("hosei: mode error\n");
			}
		
		}
	}else{
		//字牌もしくは上がり牌
		thisStartHai=StartHai;
		thisEndHai=EndHai;
	}
	if(nakiFlag==HOSEI_TEHAI){
		printf("hosei:TargetHai %s between %s and  %s (%d:%d->%d)",getHaiName(arrTehai[targetHaiNo],str3),getHaiName(thisStartHai,str1),getHaiName(thisEndHai,str2),arrTehai[targetHaiNo],thisStartHai,thisEndHai);
	}else{
		printf("hosei:TargetHai %s between %s and  %s (%d:%d->%d)",getHaiName(arrNakihai[targetHaiNo],str3),getHaiName(thisStartHai,str1),getHaiName(thisEndHai,str2),arrNakihai[targetHaiNo],thisStartHai,thisEndHai);

	}
	int ErrCount=0;
	for(int i=thisStartHai;i<=thisEndHai;i++){
		
		ret=judgeFukeisan(targetHaiNo,i,nakiFlag,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
		if(ret==1){
			if(nakiFlag==HOSEI_TEHAI){
				printf("hosei:***Success!!tehai changed hai %s(%d) -> %s(%d) \n",getHaiName(arrTehai[targetHaiNo],str1),targetHaiNo,getHaiName(i,str2),i);
			}else{
				printf("hosei:***Success!!naki changed hai %s(%d) -> %s(%d) \n",getHaiName(arrNakihai[targetHaiNo],str1),targetHaiNo,getHaiName(arrNakihai[i],str2),i);
				
			}
			return 1;
		}
		
		ErrCount++;
		if(ErrCount>100){
			printf("hosei:fatal Error\n");
		}
	}
	printf("hosei:******judgeFukeisanEx_End");
	
	return 0;
	
}
//牌選出
int correctionMain(jboolean *arrCondition, jbyte *arrTehai, int tehaiNum,jbyte *arrDora, jbyte *arrNakihai,int nakiNum, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist,int flag){


	int retFukeisan=0;
	int judgeGroup=0;
	int tempGroup=0;
	int retJudgeHose=0;;
	int HoseiVolume=0;
	int tempHaiFormer=0;
	int tempHaiLatter=0;
	int tempHaiTarget;
	char str[20];

	if(flag==HOSEI_NAKI){
		printf("hosei:Naki mode\n");
		HoseiVolume=nakiNum;
	}else{
		HoseiVolume=tehaiNum;
		printf("hosei:Tehai mode\n");
	}
	for(int i=0;i<HoseiVolume;i++){
		printf("hosei:No.%d",i);
		//鳴き牌とテハイを判別
		if(flag==HOSEI_TEHAI){
			//テハイ
			if(i<=0){
				tempHaiFormer=0;
			}else{
				tempHaiFormer=arrTehai[i-1];
			}
			tempHaiTarget=arrTehai[i];
			tempHaiLatter=arrTehai[i+1];
		}else{
			//ナキハイ
			if(i<=0){
				tempHaiFormer=0;
			}else{
				tempHaiFormer=arrNakihai[i-1];
			}
			tempHaiTarget=arrNakihai[i];
			tempHaiLatter=arrNakihai[i+1];
			
		}
			
		if(i!=0 && i<HoseiVolume-1){
			printf("hosei:****correcting it at not first and last"); 
			if(flag==HOSEI_TEHAI){
				judgeGroup=judgeSameGroup(i,arrTehai);
			}else{
				judgeGroup=judgeSameGroup(i,arrNakihai);
			}
			//同じ牌
			if(judgeGroup==SAMEHAI){
				printf("hosei:correcting samehai");
				retFukeisan=judgeFukeisanEx(i,i-1,i-1,flag,HOSEI_MODE_SAMEHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
				if(retFukeisan==1){
					printf("hosei:sameHai:end changed hai %d -> %d \n",arrTehai[i],arrTehai[i-1]);
					break;
				}
			//同じ種類
				}else if(judgeGroup==SAMEGROUP){
				//数牌
				printf("hosei:correcting sameGourp");
				if(getGroup(tempHaiFormer)!=FIND_JIHAI){
					//両隣の間をすべて調べる。ちょっと冗長
					retFukeisan=judgeFukeisanEx(i,i-1,i+1,flag,HOSEI_MODE_SAMEGR,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}else{
					//字牌
					retFukeisan=judgeFukeisanEx(i,TONN,TYUNN,flag,HOSEI_MODE_JIHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}
			
			//両隣が違う種類	
			}else {
				printf("hosei:correcting differentGroup");
				
				//左側から補正
				if(getGroup(tempHaiFormer)!=FIND_JIHAI){
					//
					retFukeisan=judgeFukeisanEx(i,i-1,i-1,flag,HOSEI_MODE_DIFFGR_LEFT,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;

				}else{
					//字牌
					retFukeisan=judgeFukeisanEx(i,TONN,TYUNN,flag,HOSEI_MODE_JIHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}
				//右側から補正
				if(getGroup(tempHaiLatter)!=FIND_JIHAI){
					//数牌
					retFukeisan=judgeFukeisanEx(i,i+1,i+1,flag,HOSEI_MODE_DIFFGR_RIGHT,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;

				}else{
					//字牌
					retFukeisan=judgeFukeisanEx(i,TONN,TYUNN,flag,HOSEI_MODE_JIHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}
				
			}
		}else{
			//初牌
			if(i==0){
			//ジハイで無い
				printf("hosei:***correcting first Hai");
				//右から補正
				if(getGroup(tempHaiFormer)!=FIND_JIHAI){
					//数牌
					retFukeisan=judgeFukeisanEx(i,i+1,i+1,flag,HOSEI_MODE_DIFFGR_RIGHT,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}else{
					//字牌
					retFukeisan=judgeFukeisanEx(i,TONN,TYUNN,flag,HOSEI_MODE_JIHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
					if(retFukeisan==1) break;
				}	
			}
			//上がり牌
			else {
				printf("hosei:***correcting last Hai");
				if(flag==HOSEI_TEHAI){
					//上がり牌
					retFukeisan=judgeFukeisanEx(i,WANZU_1,TYUNN,flag,HOSEI_MODE_ALL,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
				}else{
					//左から補正（鳴き牌）
					//左側から補正
					if(getGroup(tempHaiFormer)!=FIND_JIHAI){
						//
						retFukeisan=judgeFukeisanEx(i,i-1,i-1,flag,HOSEI_MODE_DIFFGR_LEFT,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
						if(retFukeisan==1) break;

					}else{
						//字牌
						retFukeisan=judgeFukeisanEx(i,TONN,TYUNN,flag,HOSEI_MODE_JIHAI,arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
						if(retFukeisan==1) break;
					}
				}
				if(retFukeisan==1) break;
			}
		}
		printf("hosei:-----");
	}
		
	
	return retFukeisan;
}




//補正のentryPoint
int correctionAct(jboolean *arrCondition, jbyte *arrTehai, jbyte *arrDora, jbyte *arrNakihai, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist){
	int ret=0;
	int tehaiNum=0;
	int nakiNum=0;
	int i=0;

	while(arrTehai[i]!=HAI_END){
		tehaiNum++;
		i++;
	}
	i=0;
	while(arrNakihai[i]!=HAI_END){
		nakiNum++;
		i++;
	}
	ret=checkOverWrong(arrTehai,tehaiNum,arrNakihai,nakiNum);
	printf("hosei:EntryPointCorrecting\n");
	if(ret==EASYCHECK_FAIL){
		printf("hosei:Fail Scan\n");
		//全然スキャンできなかった
		for(int i=0;i<tehaiNum;i++){
			arrTehai[i]=TYUNN;
		}
		for(int i=0;i<nakiNum;i++){
			arrNakihai[i]=TYUNN;
		}
		ret=-1;
	}else{
		//一発目
		ret=fukeisan_main(arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, honba, arrResult, arrYakulist);
		printf("hosei:first_fukeisan_result=%d",ret);
		if(ret==-1){
			//補正
			//テハイ
			ret=correctionMain(arrCondition, arrTehai,tehaiNum, arrDora, arrNakihai,nakiNum, arrNakiflag, honba, arrResult, arrYakulist,HOSEI_TEHAI);
			if(ret!=1 && nakiNum>0){				
				//ナキハイ
				ret=correctionMain(arrCondition, arrTehai,tehaiNum, arrDora, arrNakihai,nakiNum, arrNakiflag, honba, arrResult, arrYakulist,HOSEI_NAKI);
			}
		}
	}
	
	return ret;
}