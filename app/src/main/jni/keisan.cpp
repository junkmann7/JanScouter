/*
// keisan.cpp
// 概要： mjscore_main()へのラッパー関数
// 1.Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_JanComputeResult()からのデータを変換
// 2. mjscoreの形式に変換
// 3. mjsocre.RUNの形式で渡す
// 4. 結果を JanCompluteResultに渡す
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <jni.h>
#include "mjscore.h"
#include "common.h"
#include "ogura.h"
#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "mjscore", __VA_ARGS__)
#define printf(...) LOGV(__VA_ARGS__)

//デバッグモード
//#define OGURA_FUKEISAN_DEBUG_MODE


class FuStatus{
public:
	jbyte  hai; //牌の情報
	jbyte  status; // トイツ、シュンツなど
	bool is_agari; //あたり牌
	bool is_naki;  //鳴き牌
};

class FuData{
public:
	FuStatus hidata[18];
	jint fu_result;
	jint yaku_result;
	jint score_result;
};

jboolean *condition_data;
jbyte *tehai_data;
jbyte *dora_data;

FuData fudata, fudata_naki;

int set(jint honba, jint *arrResult, jboolean *arrYakulist);


int hai_tomjtehai(jbyte *tehai, bool is_nakihai){
	int i,j,k;
	jbyte tmp;
	jbyte agarihai;
}

void  hai_setstatus(int pos,int range, FuData *fudata, int param){
	int i;
	for(i = pos; i < pos + range; i++ ){
		fudata->hidata[i].status = param;
	}
}

void hai_setnaki(int pos,int range, FuData *fudata, jboolean *nakiflag){
	int i;
	for(i = pos; i < pos + range; i++ ){
		fudata->hidata[i].status = nakiflag[i];
	}
}

int hai_nakisort(int pos, FuData *fudata, jboolean *nakiflag ){
//ToDo:鳴いた牌のデータを追加
	int i,j,k;
	printf("hai_nakisort start  \n");
	//４枚の場合 ANKAN or MINAKNのどちらか。nakiflagにより決める。
	for(i = pos ; fudata->hidata[i].hai != HAI_END  && i  < 14; i++){
		printf("hai = %d\n", fudata->hidata[i].hai);

		if(fudata->hidata[i].hai == fudata->hidata[i+1].hai
			&& fudata->hidata[i].hai == fudata->hidata[i+2].hai
			&& fudata->hidata[i].hai == fudata->hidata[i+3].hai
		){
			k = 0;
			for(j = i ; j < i+4; j++){
				if(nakiflag[j]) k = 1;
			}
			if(k){
				printf("MINKAN Ditect!! pos = %d\n", i);
				fudata->hidata[i].status   = MJScore::MINKAN;  //MINKAN
				fudata->hidata[i+1].status = MJScore::MINKAN;
				fudata->hidata[i+2].status = MJScore::MINKAN;
				fudata->hidata[i+3].status = MJScore::MINKAN;

			} else	{
				printf("ANKAN Ditect!! pos = %d\n", i);
				fudata->hidata[i].status   = MJScore::ANKAN;
				fudata->hidata[i+1].status = MJScore::ANKAN;
				fudata->hidata[i+2].status = MJScore::ANKAN;
				fudata->hidata[i+3].status = MJScore::ANKAN;
			}

			fudata->hidata[i].is_naki = nakiflag[i];
			fudata->hidata[i+1].is_naki = nakiflag[i+1];
			fudata->hidata[i+2].is_naki = nakiflag[i+2];
			fudata->hidata[i+3].is_naki = nakiflag[i+3];
			i += 4;
			hai_nakisort(i, fudata, nakiflag);
			break;
		}
		else if(fudata->hidata[i].hai == fudata->hidata[i+1].hai && fudata->hidata[i].hai == fudata->hidata[i+2].hai){
			printf("MINKOU Ditect!! pos = %d\n", i);
			fudata->hidata[i].status   = MJScore::PON; //PON
			fudata->hidata[i+1].status = MJScore::PON;
			fudata->hidata[i+2].status = MJScore::PON;

			fudata->hidata[i].is_naki = nakiflag[i];
			fudata->hidata[i+1].is_naki = nakiflag[i+1];
			fudata->hidata[i+2].is_naki = nakiflag[i+2];

			i += 3;
			hai_nakisort(i, fudata, nakiflag);
			break;
		}
		else if((fudata->hidata[i].hai+1) == fudata->hidata[i+1].hai && (fudata->hidata[i].hai+2) == fudata->hidata[i+2].hai){
			if((fudata->hidata[i].hai >= PINZU_1 && fudata->hidata[i].hai <= PINZU_7)
			|| (fudata->hidata[i].hai >= WANZU_1 && fudata->hidata[i].hai <= WANZU_7)
			||(fudata->hidata[i].hai >= SOUZU_1 && fudata->hidata[i].hai <= SOUZU_7)){
				printf("SHUNTSU Ditect!! pos = %d\n", i);
				fudata->hidata[i].status   = MJScore::CHII;  //CHII
				fudata->hidata[i+1].status = MJScore::CHII;
				fudata->hidata[i+2].status = MJScore::CHII;

				fudata->hidata[i].is_naki = nakiflag[i];
				fudata->hidata[i+1].is_naki = nakiflag[i+1];
				fudata->hidata[i+2].is_naki = nakiflag[i+2];

				i +=3;
				hai_nakisort(i, fudata, nakiflag);
				break;
			}
		}
	}
	printf("LoopEnd = %d\n",i);
	return 0;
}

int hai_presort(jbyte *tehai, FuData *fudata, bool is_nakihai){
	int i,j,k;
	jbyte tmp;
	jbyte agarihai;
	printf("hai_presort start \n");
	for(i = 0; tehai[i] != HAI_END && i <17; i++){
		fudata->hidata[i].hai  = tehai[i];
		if(tehai[i] ==  WANZU_AKA)
			fudata->hidata[i].hai =WANZU_5;
		if(tehai[i] ==  PINZU_AKA)
			fudata->hidata[i].hai  =PINZU_5;
		if(tehai[i] ==  SOUZU_AKA)
			fudata->hidata[i].hai  =SOUZU_5;
	}

	fudata->hidata[i].hai = tehai[i];
	if(is_nakihai) return 0;

	//ToDo? もしかしたら面前のソートが入る可能性あり。
	return 0;

}

int fukeisan_main(
		jboolean *arrCondition,
		jbyte    *arrTehai,
		jbyte    *arrDora,
		jbyte    *arrNakihai,
		jboolean *arrNakiflag,
		jint      honba,
		jint     *arrResult,
		jboolean *arrYakulist )
	{
	int i,j,k;
	int compute_result;
	static const FuData zero = {0};
	fudata_naki = zero;
	fudata = zero;

	tehai_data = arrTehai;
	dora_data  = arrDora;
	condition_data = arrCondition;

	for(i = 0; tehai_data[i] != HAI_END && i < 15; i++){
		printf("set_test() tehai[%d] = %d\n",i, tehai_data[i]);
	}
	for(i = 0; dora_data[i] != HAI_END && i < 8; i++){
		printf("set_test() dora[%d] = %d \n",i, dora_data[i]);
	}

	hai_presort(arrNakihai, &fudata_naki, 1);
	hai_nakisort(0, &fudata_naki, arrNakiflag);

	compute_result = set(honba,arrResult,arrYakulist);

	printf("Fukeisan end\n");
	return compute_result;
}

int hi_change(int hai){

	switch(hai){
	case WANZU_1: return 1;
	case WANZU_2: return 2;
	case WANZU_3: return 3;
	case WANZU_4: return 4;
	case WANZU_5: return 5;
	case WANZU_6: return 6;
	case WANZU_7: return 7;
	case WANZU_8: return 8;
	case WANZU_9: return 9;
	case PINZU_1: return 11;
	case PINZU_2: return 12;
	case PINZU_3: return 13;
	case PINZU_4: return 14;
	case PINZU_5: return 15;
	case PINZU_6: return 16;
	case PINZU_7: return 17;
	case PINZU_8: return 18;
	case PINZU_9: return 19;
	case SOUZU_1: return 21;
	case SOUZU_2: return 22;
	case SOUZU_3: return 23;
	case SOUZU_4: return 24;
	case SOUZU_5: return 25;
	case SOUZU_6: return 26;
	case SOUZU_7: return 27;
	case SOUZU_8: return 28;
	case SOUZU_9: return 29;
	case TONN:    return 31;
	case NANN: return 32;
	case SHA: return 33;
	case PEI: return 34;
	case HAKU: return 35;
	case HATSU: return 36;
	case TYUNN: return 37;
	case WANZU_AKA: return 38;
	case PINZU_AKA: return 39;
	case SOUZU_AKA: return 40;
	default: break;
	}
	return -1;
}

int set(jint honba, jint *arrResult, jboolean *arrYakulist)
{
	int i,j;
	int compute_result;
	int tehai[41]; //手牌
	int fuuro[20]; //フーロ
	int dora[9];   //ドラ
	MJScore a;     //インスタンス

	/*          手牌等の情報入力         */
	/***************************************************/
	//初期化
	memset(tehai,0,sizeof(tehai));
	memset(fuuro,0,sizeof(fuuro));
	memset(dora,0,sizeof(dora));

	for(i = 0; tehai_data[i] != HAI_END && i < 15; i++){
		j = hi_change(tehai_data[i]);
		tehai[j] += 1;
		printf("set() tehai[%d] = %d",j, tehai[j]);
	}
	a.Set_Agarihai(hi_change(tehai_data[i-1]));//あがり牌

	//ドラ
	int count = 0;
	for(i = 0,j = 1; dora_data[i] != HAI_END && i < 8; i++,j++){
		dora[j] = hi_change(dora_data[i]);
		printf("set() dora[%d] = %d",j, dora[j]);
	}

	for(i = 0, j =0; fudata_naki.hidata[i].hai != HAI_END && j < 16 ;j+=4){
		switch(fudata_naki.hidata[i].status){
		case MJScore::PON:
			printf("set() PON %d \n", fudata_naki.hidata[i].hai);
			fuuro[j]=MJScore::PON;
			fuuro[j+1]=hi_change(fudata_naki.hidata[i].hai);
			i+=3;
			break;
		case MJScore::CHII:
			printf("set() CHII %d \n", fudata_naki.hidata[i].hai);
			fuuro[j]=MJScore::CHII;
			fuuro[j+1]=hi_change(fudata_naki.hidata[i].hai);
			i+=3;
			break;
		case MJScore::ANKAN:
			printf("set() ANKAN %d \n", fudata_naki.hidata[i].hai);
			fuuro[j]=MJScore::ANKAN;
			fuuro[j+1]=hi_change(fudata_naki.hidata[i].hai);
			i+=4;
			break;
		case MJScore::MINKAN:
			fuuro[j]=MJScore::ANKAN;
			fuuro[j+1]=hi_change(fudata_naki.hidata[i].hai);
			i+=4;
			break;
		default:
			;
		}
	}

	/***************************************************/
	//役情報
	if(condition_data[0])  a.Set_Tumoagari(true);   //ツモ
	if(condition_data[1])  a.Is_Riichi(true);       //リーチ
	if(condition_data[2])  a.Is_Ippatu(true);       //一発
	if(condition_data[3])  a.Is_Rinsyan(true);      //リンシャン
	if(condition_data[4])  a.Is_Haitei(true);       //ハイテイ
	if(condition_data[5])  a.Is_DoubleRiichi(true); //ダブルリーチ
	if(condition_data[6])  a.Is_Tyankan(true);      //チャンカン //->これをオープン立直代わりする
//	if(condition_data[X])  a.Is_Tenhou(true);       //天和
//	if(condition_data[X])  a.Is_Tiihou(true);       //地和
	if(condition_data[7])  a.Set_Bakaze(31);        //場風 - 東
	else                   a.Set_Bakaze(32);        //場風 - 南
	if(condition_data[8])  a.Set_Jikaze(31);        //自風 - 東
	if(condition_data[9])  a.Set_Jikaze(32);        //自風 - 南
	if(condition_data[10]) a.Set_Jikaze(33);        //自風 - 西
	if(condition_data[11]) a.Set_Jikaze(34);        //自風 - 北
//	if(condition_data[X])
	a.Avail_Akahai(true); //赤牌有り
//	if(condition_data[X])
	a.Avail_Kuitan(true); //クイタン有り
	//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
	//ルール入力
	a.Avail_ManganKiriage(true);    //満貫切り上げ有り
	a.Avail_Ba1500(false);          //場千五有り
	a.Avail_DoubleKokushi13(false); //国士13面待ちダブル役満有り
	a.Avail_DoubleTyuuren9(false);  //九連宝燈9面待ちダブル役満有り
	a.Avail_DoubleDaisuusii(false); //大四喜ダブル役満有り
	a.Avail_DoubleSuttan(false);    //四暗刻単騎ダブル役満有り
	//特殊役成立フラグ入力
	a.Is_NagashiMangan(false); //流し満貫
	//追加情報入力
	//必須情報入力
	a.Set_Tehai(tehai);        //手牌
	a.Set_Honba(honba);        //N本場
	a.Set_KyoutakuRIichi(0);   //供託リーチ
	a.Set_Fuuro(fuuro);        //フーロ牌入力
	a.Set_Dora(dora);          //ドラ牌入力

	a.Run(); //実行
	printf("Fukeisan errorstate = %d\n\n",a.Get_ErrorCode());
	if(a.Get_ErrorCode() > 0){
		memset(arrResult,0,sizeof(int)*3);
		memset(arrYakulist,0,sizeof(bool)*60);
		//akasaka Add 
		if(a.Get_ErrorCode()==a.YAKUNASI){
			return -2;
		}
		//akasaka End
		return -1;
	};
	//エラーコード取得
	//-1: 未計算
	// 0: 正常終了
	// 1: 「非フーロ牌+フーロ数*3」が14枚でないor手牌に0未満か5以上のデータがある
	// 2: あがり牌情報がないor手牌にあがり牌がない
	// 3: あがっていない
	// 4: 場風か自風情報がない
	// 5: 鳴きリーチしている
	// 6: 役がない

	jstring result[20];     //サイズは20以上
	a.Get_Yakuname(result); //参照渡し

	bool Yakutable[60];

	a.Get_Yakutable(Yakutable);	 //成立役のテーブル
	for(i = 0; i < 60; i++){
			arrYakulist[i] = Yakutable[i];
	}

	printf("Fu: %d,Fan: %d,TokutenKo: %d",a.Get_Fu(),a.Get_Fan(),a.Get_TokutenKo());

	//戻り値格納。
	arrResult[0] = (jint)a.Get_Fu();
	arrResult[1] = (jint)a.Get_Fan();
	arrResult[2] = (jint)a.Get_TokutenKo();

	//ToDo: 役の名前を返す処理を追加。
	return 1;
}
