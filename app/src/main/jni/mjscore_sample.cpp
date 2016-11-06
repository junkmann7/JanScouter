#include "mjscore.h"

int
tehai[41],//手牌
fuuro[20],//フーロ
dora[9];//ドラ

MJScore a;//インスタンス

void set();

int mjscore_main()
{
	printf("starttest1\n");
	set();
	printf("starttest2\n");
	a.Run();//実行
	printf("starttest3\n");


//	cout<<"エラーコード＝"<<a.Get_ErrorCode()<<endl<<endl;
	printf("starttest3\n");

	printf("starttest4=%d\n\n",a.Get_ErrorCode());
	//エラーコード取得
	//-1:未計算
	//0:正常終了
	//1:「非フーロ牌+フーロ数*3」が14枚でないor手牌に0未満か5以上のデータがある
	//2:あがり牌情報がないor手牌にあがり牌がない
	//3:あがっていない
	//4:場風か自風情報がない
	//5:鳴きリーチしている
	//6:役がない

	//結果
	//役名の配列取得
	jstring result[20];//サイズは20以上
	a.Get_Yakuname(result);//参照渡し
	//表示
	for(int i=0;result[i] != (jstring)"";i++){
//		cout<<result[i];
		printf("starttest5 %s",result[i]);
		if(a.Get_Fan()<MJScore::YAKUMAN){
//			cout<<"飜";
			printf("\n");

		}
		printf("\n");
//		cout<<endl;
	}
//	cout<<endl;
	printf("\n");

	//結果取得
#if 0
	cout<<"親ツモ＝"<<a.Get_OyaTumo()<<endl;//親ツモ
	cout<<"親ロン＝"<<a.Get_OyaRon()<<endl;//親ロン
	cout<<"子ツモ親＝"<<a.Get_KoTumoOya()<<endl;//子ツモ親
	cout<<"子ツモ子＝"<<a.Get_KoTumoKo()<<endl;//子ツモ子
	cout<<"子ロン＝"<<a.Get_KoRon()<<endl;//子ロン
	cout<<"符＝"<<a.Get_Fu()<<endl;//符 -1で符が関係無い役
	cout<<"役数＝"<<a.Get_Fan()<<endl;//役数
	cout<<"ドラ数(赤除く)"<<a.Get_Dorasuu()<<endl;//ドラ数(赤除く)
	cout<<"裏ドラ数"<<a.Get_Uradorasuu()<<endl;//裏ドラ数
	cout<<"赤ドラ数"<<a.Get_Akadorasuu()<<endl;//赤ドラ数
	cout<<"最終的に親が貰う点数＝"<<a.Get_TokutenOya()<<endl;//最終的に親が貰う点数
	cout<<"最終的に子が貰う点数＝"<<a.Get_TokutenKo()<<endl;//最終的に子が貰う点数
#endif
	if(a.Get_Ismangan())
//		cout<<"満貫"<<endl;//満貫かどうか
		printf("\n");

	bool Yakutable[60];
	a.Get_Yakutable(Yakutable);	 //成立役のテーブルS

}

void set()
{

	/*          手牌等の情報入力         */
	/***************************************************/
	//初期化
	memset(tehai,0,sizeof(tehai));
	memset(fuuro,0,sizeof(fuuro));
	memset(dora,0,sizeof(dora));

	//ドラ
	dora[1]=13,dora[2]=0,dora[3]=0,dora[4]=0;
	dora[5]=11,dora[6]=0,dora[7]=0,dora[8]=0;

	//手牌
	tehai[1]=0,tehai[2]=0,tehai[3]=0,tehai[4]=0,tehai[5]=0,
	tehai[6]=1,tehai[7]=1,tehai[8]=1,tehai[9]=0,

	tehai[11]=0,tehai[12]=1,tehai[13]=1,tehai[14]=1,tehai[15]=2,
	tehai[16]=0,tehai[17]=0,tehai[18]=0,tehai[19]=0,

	tehai[21]=0,tehai[22]=0,tehai[23]=0,tehai[24]=0,tehai[25]=0,
	tehai[26]=1,tehai[27]=1,tehai[28]=1,tehai[29]=0,

	tehai[31]=0,tehai[32]=0,tehai[33]=0,tehai[34]=0,tehai[35]=0,
	tehai[36]=0,tehai[37]=0,

	tehai[38]=0,tehai[39]=1,tehai[40]=0;

	//フーロ牌
	fuuro[0]=MJScore::ANKAN;
	fuuro[1]=16;
	/*
	fuuro[4]=MJScore::CHII;
	fuuro[5]=21;

	fuuro[8]=MJScore::KAKAN;
	fuuro[9]=9;

	fuuro[12]=MJScore::PON;
	fuuro[13]=7;
	*/
	/***************************************************/

	//ルール入力
	a.Avail_Akahai(false);//赤牌有り
	a.Avail_Kuitan(true);//クイタン有り
	a.Avail_ManganKiriage(true);//満貫切り上げ有り
	a.Avail_Ba1500(false);//場千五有り
	a.Avail_DoubleKokushi13(false);//国士13面待ちダブル役満有り
	a.Avail_DoubleTyuuren9(false);//九連宝燈9面待ちダブル役満有り
	a.Avail_DoubleDaisuusii(false);//大四喜ダブル役満有り
	a.Avail_DoubleSuttan(false);//四暗刻単騎ダブル役満有り

	//特殊役成立フラグ入力
	a.Is_Riichi(false);//リーチ
	a.Is_Tenhou(false);//天和
	a.Is_Tiihou(false);//地和
	a.Is_DoubleRiichi(false);//ダブルリーチ
	a.Is_Ippatu(false);//一発
	a.Is_Tyankan(false);//チャンカン
	a.Is_Rinsyan(false);//リンシャン
	a.Is_NagashiMangan(false);//流し満貫
	a.Is_Haitei(false);//ハイテイ

	//必須情報入力
	a.Set_Tehai(tehai);//手牌
	a.Set_Agarihai(15);//あがり牌
	a.Set_Bakaze(31);//場風
	a.Set_Jikaze(32);//自風

	//追加情報入力
	a.Set_Tumoagari(false);//ツモあがりかどうか
	a.Set_Honba(0);//N本場
	a.Set_KyoutakuRIichi(0);//供託リーチ
	a.Set_Fuuro(fuuro);//フーロ牌入力
	a.Set_Dora(dora);//ドラ牌入力

}
