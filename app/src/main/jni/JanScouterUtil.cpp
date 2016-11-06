#include <jni.h>
#include <android/log.h>
#include "akasaka.h"
#include "ogura.h"
#include "time.h"


extern "C" {
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    decodeYUV2BIN
	 * Signature: ([BII[I)I
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_decodeYUV2BIN
	(JNIEnv *env, jobject me, jbyteArray img, jint imgWidth, jint imgHeight, jintArray out)
	{
		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;
		int size = imgWidth * imgHeight;

		jbyte* arrImg = env->GetByteArrayElements(img, &b);
		jint*  arrOut = env->GetIntArrayElements(out,&b);

		for (j = 0; j < imgHeight; j++) {
			int pixPtr = j * imgWidth;
			int jDiv2 = j >> 1;
		for (i = 0; i < imgWidth; i++) {
			Y = arrImg[pixPtr];
			if (Y < 0) Y += 255;
			if ((i & 0x1) != 1) {
				int cOff = size + jDiv2 * imgWidth + (i >> 1) * 2;
				Cb = arrImg[cOff];
				if (Cb < 0) Cb += 127;
				else Cb -= 128;
				Cr = arrImg[cOff + 1];
				if (Cr < 0) Cr += 127;
				else Cr -= 128;
			}
			int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
			if (R < 0) R = 0;
			else if (R > 255) R = 255;
			int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
			if (G < 0) G = 0;
			else if (G > 255) G = 255;
			int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
			if (B < 0) B = 0;
			else if (B > 255) B = 255;
			int value = (B + G + R) / 3;
			if( value < 128 ) B = G = R = 0;
			else              B = G = R = 255;
			arrOut[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
		}
		}

		env->ReleaseByteArrayElements( img, arrImg, 0);
		env->ReleaseIntArrayElements(  out, arrOut, 0);
	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    decodeYUV2GRAY
	 * Signature: ([BII[I)I
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_decodeYUV2GRAY
	(JNIEnv *env, jobject me, jbyteArray img, jint imgWidth, jint imgHeight, jintArray out)
	{
		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;
		int size = imgWidth * imgHeight;

		jbyte* arrImg     = env->GetByteArrayElements(img, &b);
		jint*  arrOut     = env->GetIntArrayElements(out,&b);

		for (j = 0; j < imgHeight; j++) {
			int pixPtr = j * imgWidth;
			int jDiv2 = j >> 1;
		for (i = 0; i < imgWidth; i++) {
			Y = arrImg[pixPtr];
			if (Y < 0) Y += 255;
			if ((i & 0x1) != 1) {
				int cOff = size + jDiv2 * imgWidth + (i >> 1) * 2;
				Cb = arrImg[cOff];
				if (Cb < 0) Cb += 127;
				else Cb -= 128;
				Cr = arrImg[cOff + 1];
				if (Cr < 0) Cr += 127;
				else Cr -= 128;
			}
			int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
			if (R < 0) R = 0;
			else if (R > 255) R = 255;
			int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
			if (G < 0) G = 0;
			else if (G > 255) G = 255;
			int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
			if (B < 0) B = 0;
			else if (B > 255) B = 255;
			int value = (B + G + R) / 3;
			B = G = R = value;
			arrOut[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
		}
		}

		env->ReleaseByteArrayElements( img, arrImg, 0);
		env->ReleaseIntArrayElements(  out, arrOut, 0);
	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    decodeYUV
	 * Signature: ([BII[I)I
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_decodeYUV2RGB
	(JNIEnv *env, jobject me, jbyteArray img, jint imgWidth, jint imgHeight, jintArray out)
	{
		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;
		int size = imgWidth * imgHeight;

		jbyte* arrImg     = env->GetByteArrayElements(img, &b);
		jint*  arrOut     = env->GetIntArrayElements(out,&b);

		for (j = 0; j < imgHeight; j++) {
			int pixPtr = j * imgWidth;
			int jDiv2 = j >> 1;
		for (i = 0; i < imgWidth; i++) {
			Y = arrImg[pixPtr];
			if (Y < 0) Y += 255;
			if ((i & 0x1) != 1) {
				int cOff = size + jDiv2 * imgWidth + (i >> 1) * 2;
				Cb = arrImg[cOff];
				if (Cb < 0) Cb += 127;
				else Cb -= 128;
				Cr = arrImg[cOff + 1];
				if (Cr < 0) Cr += 127;
				else Cr -= 128;
			}
			int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
			if (R < 0) R = 0;
			else if (R > 255) R = 255;
			int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
			if (G < 0) G = 0;
			else if (G > 255) G = 255;
			int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
			if (B < 0) B = 0;
			else if (B > 255) B = 255;
			arrOut[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
		}
		}

		env->ReleaseByteArrayElements( img, arrImg, 0);
		env->ReleaseIntArrayElements(  out, arrOut, 0);
	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    public static native int JanSampleDetector(
        byte[]  img,              // カメラから取得したスルー画像。
        int     imgWidth,         // byte[] img の横サイズ。
        int     imgHeight,        // byte[] img の縦サイズ。
        int     guidePosX,        // ガイド枠の開始位置 X 座標
        int     guidePosY,        // ガイド枠の開始位置 Y 座標
        int     guideWidth,       // ガイド枠の横サイズ
        int     guideHeight,      // ガイド枠の縦サイズ
        int[]   pointX,           // 各牌のX座標。出力用。
        int[]   pointY,           // 各牌のY座標。出力用。
        int[]   tileWidth,        // 各牌の横幅。出力用。
        int[]   tileHeight,       // 各牌の縦幅。出力用。
        byte[]  hiArray,          // 各牌の2値画像。出力用。
        int[]   edgeImg,          // 解析画像。デバッグ用。
        boolean isDebugBtnPressed // デバッグボタン押下判定。デバッグ用。
        );
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_JanSampleDetector
	(JNIEnv *env, jobject me, jbyteArray img, jint imgWidth, jint imgHeight, jint guidePosX, jint guidePosY, jint guideWidth, jint guideHeight, jintArray pointX, jintArray pointY, jintArray tileWidth, jintArray tileHeight, jbyteArray hiArray, jintArray edgeImg, jboolean isDebugBtnPressed)
	{
		if( isDebugBtnPressed ) return -999;

		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;
		int size = imgWidth * imgHeight;

		// Java 側から渡された配列を使用する
		// 実際に配列の要素にアクセスしたければ、配列への参照に変換する。変換後は普通に要素へアクセス出来る。
		jbyte* arrImg        = env->GetByteArrayElements(img, &b);
		jint*  arrPointX     = env->GetIntArrayElements(pointX,&b);
		jint*  arrPointY     = env->GetIntArrayElements(pointY,&b);
		jint*  arrTileWidth  = env->GetIntArrayElements(tileWidth,&b);
		jint*  arrTileHeight = env->GetIntArrayElements(tileHeight,&b);
		jbyte* arrHiArray    = env->GetByteArrayElements(hiArray,&b);
		jint*  arrEdgeImg    = env->GetIntArrayElements(edgeImg,&b);

		unsigned char *orgImg;
		unsigned char *binaryImg;
		unsigned char *outImg;

		orgImg    = new unsigned char [guideWidth*guideHeight];
		binaryImg = new unsigned char [guideWidth*guideHeight];
		outImg    = new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*34];

		printf("******   SAMPLE HAI DETECTION START *********\n");
		// 配列に対して操作を行う -> S
		int count = 0;
		for (j = 0; j < imgHeight; j++) {
			int pixPtr = j * imgWidth;
			int jDiv2  = j >> 1;
		for (i = 0; i < imgWidth; i++) {
			Y = arrImg[pixPtr];
			if (Y < 0) Y += 255;
			if ((i & 0x1) != 1) {
				int cOff = size + jDiv2 * imgWidth + (i >> 1) * 2;
				Cb = arrImg[cOff];
				if (Cb < 0) Cb += 127;
				else Cb -= 128;
				Cr = arrImg[cOff + 1];
				if (Cr < 0) Cr += 127;
				else Cr -= 128;
			}
			int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
			if (R < 0) R = 0;
			else if (R > 255) R = 255;
			int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
			if (G < 0) G = 0;
			else if (G > 255) G = 255;
			int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
			if (B < 0) B = 0;
			else if (B > 255) B = 255;

			if( j < guidePosY || (guidePosY+guideHeight) <= j || i < guidePosX || (guidePosX+guideWidth) <= i ){
			}else{
				//orgImg[count]     = (R+G+B)/3;
				orgImg[count]     = B;
				if(R>170 && G > 170 && B > 170){
					arrEdgeImg[count] = 255;
				}else{
					arrEdgeImg[count] = 1;
				}
				count++;
			}

			pixPtr++;
		}

		}

		// 配列に対して操作を行う -> E
		//LOGV("test");
		//printf("AAA");

		HiStatus HiArray[36];
		int ret = 0;

		//検出用に画像変換
		preImageProcess(guideWidth,guideHeight,orgImg,binaryImg);

		ret = extractSampleHai(guideWidth,guideHeight,binaryImg,HiArray);
		printf("ret=%d",ret);

		//マッチング用に画像変換
		printf("preImageProcessForMathing start");
		preImageProcessForMathing(guideWidth,guideHeight,orgImg,binaryImg);
		printf("preImageProcessForMathing end");

		//u_wtfileEx(commonImg,"/data/data/jp.tonosama.komoki.janscouter/sampleImage.raw",guideWidth*guideHeight);
		//preImageProcessForMathing(guideWidth,guideHeight,orgImg,binaryImg);
		if(ret>0){
			for(int i=0;i<36;i++){
				arrPointX[i]     = HiArray[i].Pos.x;
				arrPointY[i]     = HiArray[i].Pos.y;
				arrTileWidth[i]  = HiArray[i].Size.x;
				arrTileHeight[i] = HiArray[i].Size.y;
				//printf("i=%d %d %d %d %d",i,HiArray[i].Pos.x,HiArray[i].Pos.y,HiArray[i].Size.x,HiArray[i].Size.y);
			}
			printf("***FOUND COUNT =%d ***\n ",ret);
			arrangeHai(guideWidth,guideHeight,binaryImg,34,outImg,HiArray);
		}
		//preImageProcess(guideWidth,guideHeight,orgImg,binaryImg);
		for(i=0;i<guideWidth*guideHeight;i++){
			arrEdgeImg[i] = 0xff000000 + (binaryImg[i] << 16) + (binaryImg[i] << 8) + binaryImg[i];
		}
		for(i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*34;i++){
			arrHiArray[i] = (jbyte)outImg[i];
		}

		//JNI は、配列のコピーを作って操作するので、元の配列オブジェクトが Java の GC によって回収されても、Native 側では残ってしまう。
		//そのため利用しなくなった配列を解放する必要がある。
		//第３引数は、配列を操作した結果を Java 側に書き戻すかどうかのモードを指定する。0 は、書き戻して array を解放する。
		printf("release array elements start");
		env->ReleaseByteArrayElements( img,        arrImg,        0);
		env->ReleaseIntArrayElements(  pointX,     arrPointX,     0);
		env->ReleaseIntArrayElements(  pointY,     arrPointY,     0);
		env->ReleaseIntArrayElements(  tileWidth,  arrTileWidth,  0);
		env->ReleaseIntArrayElements(  tileHeight, arrTileHeight, 0);
		env->ReleaseByteArrayElements( hiArray,    arrHiArray,    0);
		env->ReleaseIntArrayElements(  edgeImg,    arrEdgeImg,    0);
		printf("release array elements end");
		delete [] outImg;
		delete [] orgImg;
		delete [] binaryImg;

		printf("******   SAMPLE HAI DETECTION END *********\n");

		return (jint)ret;
	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    public static native int JanResultDetector(
        byte[]    img,              // カメラから取得したスルー画像。
        int[]     imgSize,          // byte[] img の横サイズ、縦サイズ、ガイド枠の開始 X 座標、Y 座標、ガイド枠の横サイズ、縦サイズが入る。入力用。
        int[]     pointX,           // 各牌のX座標。出力用。
        int[]     pointY,           // 各牌のY座標。出力用。
        int[]     tileWidth,        // 各牌の横幅。出力用。
        int[]     tileHeight,       // 各牌の縦幅。出力用。
        byte[]    hiArray,          // 各牌の2値画像。出力用。
        boolean[] nakiflag,         // 鳴き牌フラグ。
        int[]     haiNum,           // 手牌、鳴き牌、ドラ牌の数が入る。出力用。
        byte[]    tehaiImg,         // 手牌の画像。  出力用。
        byte[]    doraImg,          // ドラ牌の画像。出力用。
        byte[]    nakiImg,          // 鳴き牌の画像。出力用。
        int[]     edgeImg,          // 解析画像。デバッグ用。
        boolean   isDebugBtnPressed // デバッグボタン押下判定。デバッグ用。
        );
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_JanResultDetector
	(JNIEnv *env, jobject me,
		jbyteArray img,      jintArray imgSize,
		jintArray pointX,    jintArray pointY,
		jintArray tileWidth, jintArray tileHeight, jbyteArray hiArray,
		jbooleanArray nakiflag,
		jintArray  haiNum,
		jbyteArray tehaiImg, jbyteArray doraImg,   jbyteArray nakiImg,
		jintArray edgeImg,   jboolean   isDebugBtnPressed )
	{
		if( isDebugBtnPressed ) return -999;

		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;

		printf("******   AGARI HAI DETECTION START *********\n");

		// Java 側から渡された配列を使用する
		// 実際に配列の要素にアクセスしたければ、配列への参照に変換する。変換後は普通に要素へアクセス出来る。
		jbyte*    arrImg        = env->GetByteArrayElements(    img,        &b);
		jint*     arrImgSize    = env->GetIntArrayElements(     imgSize,    &b);
		jint*     arrPointX     = env->GetIntArrayElements(     pointX,     &b);
		jint*     arrPointY     = env->GetIntArrayElements(     pointY,     &b);
		jint*     arrTileWidth  = env->GetIntArrayElements(     tileWidth,  &b);
		jint*     arrTileHeight = env->GetIntArrayElements(     tileHeight, &b);
		jbyte*    arrHiArray    = env->GetByteArrayElements(    hiArray,    &b);
		jboolean* arrNakiflag   = env->GetBooleanArrayElements( nakiflag,   &b);
		jint*     arrHaiNum     = env->GetIntArrayElements(     haiNum,     &b);
		jbyte*    arrTehaiImg   = env->GetByteArrayElements(    tehaiImg,   &b);
		jbyte*    arrDoraImg    = env->GetByteArrayElements(    doraImg,    &b);
		jbyte*    arrNakiImg    = env->GetByteArrayElements(    nakiImg,    &b);
		jint*     arrEdgeImg    = env->GetIntArrayElements(     edgeImg,    &b);

		int imgWidth    = arrImgSize[0];
		int imgHeight   = arrImgSize[1];
		int guidePosX   = arrImgSize[2];
		int guidePosY   = arrImgSize[3];
		int guideWidth  = arrImgSize[4];
		int guideHeight = arrImgSize[5];
		int size = imgWidth * imgHeight;

		unsigned char *orgImg;
		unsigned char *binaryImg;
		unsigned char *tehaiHai;
		//unsigned char *doraHai;
		unsigned char *nakiHai;



		orgImg    = new unsigned char [guideWidth*guideHeight];
		binaryImg = new unsigned char [guideWidth*guideHeight];

		tehaiHai = new unsigned char [CONST_HAI_WIDTH*CONST_HAI_HEIGHT*MAX_TEHAI];
		//doraHai= new unsigned char [CONST_HAI_WIDTH*CONST_HAI_HEIGHT*MAX_DORA];
		nakiHai= new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*MAX_NAKI];

		// =====================================================================================================================================================
		// 配列操作ここから → S

		printf("******   START AGARI MAIN ********\n");

		int count = 0;
		for (j = 0; j < imgHeight; j++) {
			int pixPtr = j * imgWidth;
			int jDiv2  = j >> 1;
			for (i = 0; i < imgWidth; i++) {
				Y = arrImg[pixPtr];
				if (Y < 0) Y += 255;
				if ((i & 0x1) != 1) {
					int cOff = size + jDiv2 * imgWidth + (i >> 1) * 2;
					Cb = arrImg[cOff];
					if (Cb < 0) Cb += 127;
					else Cb -= 128;
					Cr = arrImg[cOff + 1];
					if (Cr < 0) Cr += 127;
					else Cr -= 128;
				}
				int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
				if (R < 0) R = 0;
				else if (R > 255) R = 255;
				int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
				if (G < 0) G = 0;
				else if (G > 255) G = 255;
				int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
				if (B < 0) B = 0;
				else if (B > 255) B = 255;

				if( j < guidePosY || (guidePosY+guideHeight) <= j || i < guidePosX || (guidePosX+guideWidth) <= i ){
				}else{
					//orgImg[count]     = (R+G+B)/3;
					orgImg[count]     = B;
					arrEdgeImg[count] = 0xff000000 + (B << 16) + (G << 8) + R;;
					count++;
				}
				pixPtr++;
			}

		}

		// 配列操作ここまで → E
		// =====================================================================================================================================================
		//********************************
		//上がり牌用
		//*****************************************

		/*
		int doraPosY=3;
		int tehaiPosY=guideHeight*2/5-10;
		int nakiPosY=guideHeight*4/5-10;
		int guideWakuY=guideHeight/4+10;
		*/
		int guideWakuY=guideHeight*8/27;
		int doraPosY= guideHeight/6 - guideWakuY/2;
		int tehaiPosY=guideHeight*3/6 - guideWakuY/2;
		int nakiPosY=guideHeight*5/6 -	guideWakuY/2;
		class AgariHai agariHaiArray;
		int ret=0;



		printf("doraY=%d tehaiY=%d nakiY=%d,guideY=%d\n",doraPosY,tehaiPosY,nakiPosY,guideWakuY);

		ret= agariMain(guideWidth,guideHeight,orgImg,agariHaiArray,tehaiPosY,nakiPosY,guideWakuY,tehaiHai,nakiHai);

		printf("Agari result is %d\n",ret);

		agariHaiArray.showHaiStatus();



		printf("ResultFound is dora=%d tehai=%d naki=%d\n",agariHaiArray.doraNum,agariHaiArray.tehaiNum,agariHaiArray.nakiNum);
		count=0;
		if(ret>0){
			/*for(int i=0;i<agariHaiArray.doraNum;i++){
				arrPointX[count]     = agariHaiArray.DoraArray[i].Pos.x;
				arrPointY[count]     = agariHaiArray.DoraArray[i].Pos.y+doraPosY;
				arrTileWidth[count]  = agariHaiArray.DoraArray[i].Size.x;
				arrTileHeight[count] = agariHaiArray.DoraArray[i].Size.y;
				count++;
			}
			*/
			for(int i=0;i<agariHaiArray.tehaiNum;i++){
				arrPointX[count]     = agariHaiArray.TehaiArray[i].Pos.x;
				arrPointY[count]     = agariHaiArray.TehaiArray[i].Pos.y+tehaiPosY;
				arrTileWidth[count]  = agariHaiArray.TehaiArray[i].Size.x;
				arrTileHeight[count] = agariHaiArray.TehaiArray[i].Size.y;
				count++;
			}
			for(int i=0;i<agariHaiArray.nakiNum;i++){
				arrPointX[count]     = agariHaiArray.NakiArray[i].Pos.x;
				arrPointY[count]     = agariHaiArray.NakiArray[i].Pos.y+nakiPosY;
				arrTileWidth[count]  = agariHaiArray.NakiArray[i].Size.x;
				arrTileHeight[count] = agariHaiArray.NakiArray[i].Size.y;
				arrNakiflag[i]=agariHaiArray.NakiArray[i].feature2;
				//printf("TESTNAKIHAI:%d =%d\n",i,agariHaiArray.NakiArray[i].feature2);
				count++;
			}


		}
		count=0;
		/*for(int i=0;i<agariHaiArray.doraNum;i++){
			printf("SouuterUtil_DORA:pos (%d %d) size (%d %d) \n",arrPointX[count],arrPointY[count],arrTileWidth[count],arrTileHeight[count]);
			count++;
		}*/
		for(int i=0;i<agariHaiArray.tehaiNum;i++){
			printf("SouuterUtil_TEHAI:pos (%d %d) size (%d %d) \n",arrPointX[count],arrPointY[count],arrTileWidth[count],arrTileHeight[count]);
			count++;
		}
		for(int i=0;i<agariHaiArray.nakiNum;i++){
			printf("SouuterUtil_NAKI:pos (%d %d) size (%d %d) \n",arrPointX[count],arrPointY[count],arrTileWidth[count],arrTileHeight[count]);
			count++;
		}



		arrHaiNum[0]=agariHaiArray.tehaiNum;
		arrHaiNum[1]=agariHaiArray.nakiNum;
		//arrHaiNum[2]=agariHaiArray.doraNum;


		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*agariHaiArray.tehaiNum;i++){
			arrTehaiImg[i]=tehaiHai[i];

		}
		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*agariHaiArray.nakiNum;i++){
			arrNakiImg[i]=nakiHai[i];

		}
		/*for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*agariHaiArray.doraNum;i++){
			arrDoraImg[i]=doraHai[i];

		}*/




		delete [] orgImg;
		delete [] binaryImg;
		//delete [] doraHai;
		delete [] tehaiHai;
		delete [] nakiHai;
		//JNI は、配列のコピーを作って操作するので、元の配列オブジェクトが Java の GC によって回収されても、Native 側では残ってしまう。
		//そのため利用しなくなった配列を解放する必要がある。
		//第３引数は、配列を操作した結果を Java 側に書き戻すかどうかのモードを指定する。0 は、書き戻して array を解放する。
		env->ReleaseByteArrayElements(    img,        arrImg,        0);
		env->ReleaseIntArrayElements(     imgSize,    arrImgSize,    1);
		env->ReleaseIntArrayElements(     pointX,     arrPointX,     0);
		env->ReleaseIntArrayElements(     pointY,     arrPointY,     0);
		env->ReleaseIntArrayElements(     tileWidth,  arrTileWidth,  0);
		env->ReleaseIntArrayElements(     tileHeight, arrTileHeight, 0);
		env->ReleaseByteArrayElements(    hiArray,    arrHiArray,    0);
		/*
		env->ReleaseByteArrayElements(    sampleImg,  arrSampleImg,  0);
		env->ReleaseByteArrayElements(    tehai,      arrTehai,      0);
		env->ReleaseByteArrayElements(    dora,       arrDora,       0);
		env->ReleaseByteArrayElements(    nakihai,    arrNakihai,    0);
		*/
		env->ReleaseBooleanArrayElements( nakiflag,   arrNakiflag,   0);
		env->ReleaseIntArrayElements(     haiNum,     arrHaiNum,     0);
		env->ReleaseByteArrayElements(    tehaiImg,   arrTehaiImg,   0);
		env->ReleaseByteArrayElements(    doraImg,    arrDoraImg,    0);
		env->ReleaseByteArrayElements(    nakiImg,    arrNakiImg,    0);
		env->ReleaseIntArrayElements(     edgeImg,    arrEdgeImg,    0);

		printf("******   AGARI HAI DETECTION END *********\n");
		return (jint)ret;

	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    public static native int JanResultMatching(
        byte[]    sampleImg, // サンプル用の牌画像。入力用。
        byte[]    tehai,     // 解析した手牌の情報。出力用。31432239・・・ みたいな感じで値が入ってて、それぞれ「ピンズの１」とか「ソーズの５」とかを意味してる。
        byte[]    dora,      // ドラの情報。出力用。
        byte[]    nakihai,   // 解析した鳴き牌の情報。出力用。「東東東」とか「ピンズ345」とかの値が入る。
        boolean[] nakiflag,  // 鳴き牌フラグ。
        int[]     haiNum,    // 手牌、鳴き牌、ドラ牌の数が入る。入力用。
                             // haiNum[0] = 手牌の数 haiNum[1] = 鳴き牌の数  haiNum[2] = ドラ牌の数
        byte[]    tehaiImg,  // 手牌の画像。  入力用。
        byte[]    doraImg,   // ドラ牌の画像。入力用。
        byte[]    nakiImg,   // 鳴き牌の画像。入力用。
        int[]     edgeImg    // 解析画像。デバッグ用。
        );
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_JanResultMatching
	(JNIEnv *env, jobject me,
		jbyteArray sampleImg,
		jbyteArray tehai,    jbyteArray dora,      jbyteArray nakihai,   jbooleanArray nakiflag,
		jintArray  haiNum,
		jbyteArray tehaiImg, jbyteArray doraImg,   jbyteArray nakiImg,
		jintArray edgeImg )
	{
		jboolean b;
		int i,j;
		int Y, Cr = 0, Cb = 0;

		printf("******   MATCHING START *********\n");

		// Java 側から渡された配列を使用する
		// 実際に配列の要素にアクセスしたければ、配列への参照に変換する。変換後は普通に要素へアクセス出来る。
		jbyte*    arrSampleImg  = env->GetByteArrayElements(    sampleImg,  &b);
		jbyte*    arrTehai      = env->GetByteArrayElements(    tehai,      &b);
		jbyte*    arrDora       = env->GetByteArrayElements(    dora,       &b);
		jbyte*    arrNakihai    = env->GetByteArrayElements(    nakihai,    &b);
		jboolean* arrNakiflag   = env->GetBooleanArrayElements( nakiflag,   &b);
		jint*     arrHaiNum     = env->GetIntArrayElements(     haiNum,     &b);
		jbyte*    arrTehaiImg   = env->GetByteArrayElements(    tehaiImg,   &b);
		jbyte*    arrDoraImg    = env->GetByteArrayElements(    doraImg,    &b);
		jbyte*    arrNakiImg    = env->GetByteArrayElements(    nakiImg,    &b);
		jint*     arrEdgeImg    = env->GetIntArrayElements(     edgeImg,    &b);

		unsigned char * nativeSampleImg;
		unsigned char * nativeTehaiImg;
		//unsigned char * nativeDoraImg;
		unsigned char * nativeNakiImg;
		char str[100];

		int tehaiNum=arrHaiNum[0];
		int nakiNum=arrHaiNum[1];
		//int doraNum=arrHaiNum[2];


		//int resultDora[MAX_DORA+1];
		int resultTehai[MAX_TEHAI+1];
		int resultNaki[MAX_NAKI+1];
		int nakiFlagArray[MAX_NAKI+1];

		//printf("MAT:tehaiNum=%d nakiNum=%d doraNum=%d\n",tehaiNum,nakiNum,doraNum);
		 nativeSampleImg = new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*34];
		 nativeTehaiImg = new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*tehaiNum];
		// nativeDoraImg  = new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*doraNum];
		 nativeNakiImg = new unsigned char[CONST_HAI_WIDTH*CONST_HAI_HEIGHT*nakiNum];


		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*34;i++){
			nativeSampleImg[i]=arrSampleImg[i];

		}
		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*tehaiNum;i++){
			nativeTehaiImg[i]=arrTehaiImg[i];

		}
		/*for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*doraNum;i++){
			nativeDoraImg[i]=arrDoraImg[i];

		}*/
		for(int i=0;i<CONST_HAI_WIDTH*CONST_HAI_HEIGHT*nakiNum;i++){
			nativeNakiImg[i]=arrNakiImg[i];

		}

		//u_wtfileEx(nativeSampleImg, "/data/data/jp.tonosama.komoki.janscouter/sample.raw", CONST_HAI_WIDTH*CONST_HAI_HEIGHT);
		//u_wtfileEx(nativeTehaiImg, "/data/data/jp.tonosama.komoki.janscouter/tehai.raw", CONST_HAI_WIDTH*CONST_HAI_HEIGHT);


		easyMatching(nativeTehaiImg,tehaiNum,nativeSampleImg,resultTehai,nakiFlagArray);
		//testHosei(resultTehai);

		for(int i=0;i<nakiNum;i++){
			nakiFlagArray[i]=arrNakiflag[i];
			//printf("TESTNAKIHAI:%d =%d\n",i,arrNakiflag[i]);
		}
		easyMatching(nativeNakiImg,nakiNum,nativeSampleImg,resultNaki,nakiFlagArray);
		//testHosei(resultNaki);



		/*	arrDora[0]=WANZU_1;
			arrDora[1]=WANZU_2;
			arrDora[2]=HAI_END;
		*/


		for(int i=0;i<tehaiNum;i++){
			arrTehai[i]=resultTehai[i];
		}
		for(int i=0;i<nakiNum;i++){
			arrNakihai[i]=resultNaki[i];
		}






		delete [] nativeSampleImg;
		delete [] nativeTehaiImg;
		//delete [] nativeDoraImg;
		//delete [] nativeNakiImg;

		printf("******   MATCHING END *********\n");

		//JNI は、配列のコピーを作って操作するので、元の配列オブジェクトが Java の GC によって回収されても、Native 側では残ってしまう。
		//そのため利用しなくなった配列を解放する必要がある。
		//第３引数は、配列を操作した結果を Java 側に書き戻すかどうかのモードを指定する。0 は、書き戻して array を解放する。
		env->ReleaseByteArrayElements(    sampleImg,  arrSampleImg,  0);
		env->ReleaseByteArrayElements(    tehai,      arrTehai,      0);
		env->ReleaseByteArrayElements(    dora,       arrDora,       0);
		env->ReleaseByteArrayElements(    nakihai,    arrNakihai,    0);
		env->ReleaseBooleanArrayElements( nakiflag,   arrNakiflag,   0);
		env->ReleaseIntArrayElements(     haiNum,     arrHaiNum,     0);
		env->ReleaseByteArrayElements(    tehaiImg,   arrTehaiImg,   0);
		env->ReleaseByteArrayElements(    doraImg,    arrDoraImg,    0);
		env->ReleaseByteArrayElements(    nakiImg,    arrNakiImg,    0);
		env->ReleaseIntArrayElements(     edgeImg,    arrEdgeImg,    0);

		return 0;
	}
	/*
	 * Class:     jp_tonosama_komoki_janscouterUtil_JanScouterUtil
	 * Method:    public static native int JanComputeResult(
        boolean[] condition,   // ツモorロン、リーチ、一発、リンシャンカイホウ、チャンカン かどうか。
        byte[]    tehai,       // 解析した手牌の情報。31432239・・・ みたいな感じで値が入ってて、それぞれ「ピンズの１」とか「ソーズの５」とかを意味してる。
        byte[]    dora,        // ドラの情報。
        byte[]    nakihai,     // 解析した鳴き牌の情報。「東東東」とか「ピンズ345」とかの値が入る。
        boolean[] nakiflag,    // 鳴き牌かどうかの情報。
        int[]     haiNum,      // 手牌、鳴き牌、ドラ牌の数が入る。入力用。
        int[]     result,      // フ、ハン、点数の値。出力用。
        boolean[] yakulist     // あがり役名のリスト。出力用。
        );
	 */
	JNIEXPORT jint JNICALL Java_jp_tonosama_komoki_janscouterUtil_JanScouterUtil_JanComputeResult
	(JNIEnv *env, jobject me, jbooleanArray condition, jbyteArray tehai, jbyteArray dora, jbyteArray nakihai, jbooleanArray nakiflag, jintArray haiNum, jintArray result, jbooleanArray yakulist, jboolean isReCompute)
	{
		int compute_result = -1;
		//int nanikaFlag=0;
		// Java 側から渡された配列を使用する
		// 実際に配列の要素にアクセスしたければ、配列への参照に変換する。変換後は普通に要素へアクセス出来る。
		jboolean b;
		jboolean* arrCondition = env->GetBooleanArrayElements( condition, &b);
		jbyte*    arrTehai     = env->GetByteArrayElements(    tehai,     &b);
		jbyte*    arrDora      = env->GetByteArrayElements(    dora,      &b);
		jbyte*    arrNakihai   = env->GetByteArrayElements(    nakihai,   &b);
		jboolean* arrNakiflag  = env->GetBooleanArrayElements( nakiflag,  &b);
		jint*     arrHaiNum    = env->GetIntArrayElements(     haiNum,    &b);
		jint*     arrResult    = env->GetIntArrayElements(     result,    &b);
		jboolean* arrYakulist  = env->GetBooleanArrayElements( yakulist,  &b);
		clock_t start,end;
		start=clock();
		// =====================================================================
		// 計算ここから -> S
		arrTehai[ arrHaiNum[0] ]   = HAI_END;
		arrDora[ arrHaiNum[2] ]    = HAI_END;
		arrNakihai[ arrHaiNum[1] ] = HAI_END;

		#if 0

		//Debug Start
		arrTehai[0]=0;
		arrTehai[1]=1;
		arrTehai[2]=2;

		arrTehai[3]=3;
		arrTehai[4]=4;
		arrTehai[5]=5;

		arrTehai[6]=33;
		arrTehai[7]=33;
		arrTehai[8]=33;

		arrTehai[9]=12;
		arrTehai[10]=12;
		arrTehai[11]=12;

		arrTehai[12]=15;
		arrTehai[13]=18;
		arrTehai[14]=HAI_END;
		//Debug End
		#endif

		#if 0
		arrHaiNum[0]=8;
		//Debug Start
		arrTehai[0]=0;
		arrTehai[1]=1;
		arrTehai[2]=2;

		arrTehai[3]=3;
		arrTehai[4]=4;
		arrTehai[5]=5;

		arrTehai[6]=33;
		arrTehai[7]=33;

		arrTehai[8]=HAI_END;

		arrHaiNum[1]=6;

		arrNakihai[0]=12;
		arrNakihai[1]=13;
		arrNakihai[2]=14;

		arrNakihai[3]=16;
		arrNakihai[4]=16;
		arrNakihai[5]=16;

		arrNakihai[6]=HAI_END;
		#endif
		//Debug End
		printf("hosei:TehaiNum=%d NakiNum=%d",arrHaiNum[0],arrHaiNum[1]);
		if(isReCompute){
			compute_result =fukeisan_main(arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, arrHaiNum[3], arrResult, arrYakulist);
		}else{
			compute_result =correctionAct(arrCondition, arrTehai, arrDora, arrNakihai, arrNakiflag, arrHaiNum[3], arrResult, arrYakulist);
		}

		if(compute_result==-2){
			printf("hosei:Yakunashi\n");
		}

		// 計算ここまで -> E
		// =====================================================================
		end=clock();
		printf("hosei:HAI KEISAN %.3f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
		//JNI は、配列のコピーを作って操作するので、元の配列オブジェクトが Java の GC によって回収されても、Native 側では残ってしまう。
		//そのため利用しなくなった配列を解放する必要がある。
		//第３引数は、配列を操作した結果を Java 側に書き戻すかどうかのモードを指定する。0 は、書き戻して array を解放する。
		env->ReleaseBooleanArrayElements( condition, arrCondition, 1);
		env->ReleaseByteArrayElements(    tehai,     arrTehai,     0);
		env->ReleaseByteArrayElements(    dora,      arrDora,      0);
		env->ReleaseByteArrayElements(    nakihai,   arrNakihai,   0);
		env->ReleaseBooleanArrayElements( nakiflag,  arrNakiflag,  1);
		env->ReleaseIntArrayElements(     haiNum,    arrHaiNum,    0);
		env->ReleaseIntArrayElements(     result,    arrResult,    0);
		env->ReleaseBooleanArrayElements( yakulist,  arrYakulist,  0);
		return compute_result;
	}
}
