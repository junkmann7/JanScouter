package jp.tonosama.komoki.objectInfo;

import java.util.ArrayList;

import jp.tonosama.komoki.janscouter.R;
import jp.tonosama.komoki.util.DevLog;

import android.graphics.Point;
import android.util.Log;

public class HiInfo {

    private static final String TAG = HiInfo.class.getSimpleName();

    private int mHiTotalNum;
    private int mDetectResult = -1;
    private ArrayList<Point> mHiPoint;
    private ArrayList<Point> mHiSize;

    private byte[] mHiArray;
    public static final int HI_WIDTH  = 64;
    public static final int HI_HEIGHT = 96;
    public static final int TOTAL_HI_NUM = 34;

    public static final String SAMPLE_IMAGE_FILE_NAME = "hai_sample.raw";
    public static final String SAMPLE_IMAGE_FILE_PATH = "/JanScouter/debug";

    public static final int SCAN_HI_NUM = 36;

    public HiInfo() {
        DevLog.i(TAG, "Instantiated new " + this.getClass());
        mHiPoint = new ArrayList<Point>();
        mHiSize  = new ArrayList<Point>();
        mHiArray = new byte[HI_WIDTH*HI_HEIGHT*TOTAL_HI_NUM];
    }
    public void setGuideInfo(){

    }

    public void setHiInfo(int result, int[] hi_point_x, int[] hi_point_y, int[] hi_width, int[] hi_height, int[] edge_img)
    {
        mDetectResult = result;
        if( hi_point_x == null || hi_point_y == null || hi_width == null || hi_height == null ){
            DevLog.d(TAG, "hi data is invalid! can't set HiInfo !!");
            return;
        }
        mHiTotalNum = hi_point_x.length;
        mHiPoint.clear();
        mHiSize.clear();
        for(int i=0;i<mHiTotalNum;i++)
        {
            DevLog.d(TAG, "i="+i+"; (" + hi_point_x[i] + "," + hi_point_y[i] + ")" + "(" + hi_width[i] + "," + hi_height[i] + ")");
            Point hi_point = new Point(hi_point_x[i],hi_point_y[i]);
            Point hi_size  = new Point(hi_width[i],  hi_height[i]);
            mHiPoint.add(hi_point);
            mHiSize.add(hi_size);
        }
    }

    public int getDetectResult(){
        return mDetectResult;
    }
    public void setHiArray( byte[] hi_array ){
        mHiArray = hi_array.clone();
    }
    public byte[] getHiArray(){
        return mHiArray;
    }
    public String getDetectResultMsg(){
        String resultMsg = "";
        resultMsg = "Found " + String.valueOf(mDetectResult) + " !";
        return resultMsg;
    }

    public int getHiTotalNum(){
        return mHiTotalNum;
    }

    public ArrayList<Point> getHiPoint(){
        return mHiPoint;
    }
    public ArrayList<Point> getHiSize(){
        return mHiSize;
    }

    public static final String[] DEFINED_HAI_NAMES = {
        "一筒","二筒","三筒","四筒","五筒","六筒","七筒","八筒","九筒",
        "一萬","二萬","三萬","四萬","五萬","六萬","七萬","八萬","九萬",
        "一索","二索","三索","四索","五索","六索","七索","八索","九索",
        "東",  "南",  "西",  "北",  "白",  "発",  "中",
        "赤筒","赤萬","赤索"
    };

    public static final String[] DEFINED_SAMPLE_HAI_NAMES = {
        "一萬","二萬","三萬","四萬","五萬","六萬","七萬","八萬","九萬",
        "一索","二索","三索","四索","五索","六索","七索","八索","九索",
        "一筒","二筒","三筒","四筒","五筒","六筒","七筒","八筒","九筒",
        "東",  "南",  "西",  "北",  "白",  "発",  "中",
        "赤筒","赤萬","赤索"
    };

    public static final int[] DEFINED_HAI_RESOURCES = {
        R.drawable.m1, R.drawable.m2, R.drawable.m3, R.drawable.m4, R.drawable.m5, R.drawable.m6, R.drawable.m7, R.drawable.m8,R.drawable.m9,
        R.drawable.s1, R.drawable.s2, R.drawable.s3, R.drawable.s4, R.drawable.s5, R.drawable.s6, R.drawable.s7, R.drawable.s8,R.drawable.s9,
        R.drawable.p1, R.drawable.p2, R.drawable.p3, R.drawable.p4, R.drawable.p5, R.drawable.p6, R.drawable.p7, R.drawable.p8,R.drawable.p9,
        R.drawable.ton,R.drawable.nan,R.drawable.sha,R.drawable.pei,R.drawable.hak,R.drawable.hat,R.drawable.chu,
        R.drawable.p5a,R.drawable.m5a,R.drawable.s5a,R.drawable.ng,
    };

    public static final int DEFINED_HAI_BACK_RESOURCE = R.drawable.bak;

    public static final String[] DEBUG_HAI_TYPE_NAME = {
        "手牌：","鳴き牌：","ドラ牌："
    };

    public static final String DEBUG_CONDITION_TITLE = "あがり条件：";
    public static final String[] DEBUG_CONDITION_NAME = {
        "ツモ", "立直", "一発", "嶺上", "海底", "Ｗ立直", "オープン",
        "場風 - 東",
        "自風 - 東","自風 - 南","自風 - 西","自風 - 南",
    };

    public static final String[] YAKU_NAME_LIST = {
        "立直",     "ダブル立直", "一発",     "ツモ",           "断幺九",   "平和",          "一盃口",   "飜牌(場風)",   "飜牌(自風)", "飜牌(白)",
        "飜牌(発)", "飜牌(中)",   "嶺上開花", "オープン立直",   "海底撈月", "河底撈魚",      "三色同順", "一気通貫",     "全帯幺",     "七対子",
        "対々和",   "三暗刻",     "混老頭",   "三色同刻",       "三槓子",   "小三元",        "混一色",   "純全帯幺",     "二盃口",     "清一色",
        "天和",     "地和",       "国士無双", "国士無双13面待", "九連宝燈", "九連宝燈9面待", "四暗刻",   "四暗刻単騎待", "大四喜",     "小四喜",
        "大三元",   "字一色",     "清老頭",   "緑一色",         "四槓子",   "ドラ",          "裏ドラ",   "赤ドラ"
    };

    public static final int[] YAKU_VALUE_LIST = {
         1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
         2, 2, 2, 2, 2, 2, 3, 3, 3, 6,
        14,14,14,14,14,14,14,14,14,14,
        14,14,14,14,14, 1, 1, 1
    };

    public static final int[] YAKU_VALUE_LIST_NAKI = {
         1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
         2, 2, 2, 2, 2, 2, 2, 2, 3, 5,
        14,14,14,14,14,14,14,14,14,14,
        14,14,14,14,14, 1, 1, 1
    };

    public static int[] computeScoreOya(int fu, int han, int honba)
    {
        int[] score = {0,0,0};

        int[][] scores = {
            {     0,    0, 1500, 2000, 2400, 2900, 3400, 3900, 4400, 4800, 5300 }, // 1翻
            {  2000, 2400, 2900, 3900, 4800, 5800, 6800, 7700, 8700, 9600,10600 }, // 2翻
            {  3900, 4800, 5800, 7700, 9600,11600,12000,12000,12000,12000,12000 }, // 3翻
            {  7700, 9600,11600,12000,12000,12000,12000,12000,12000,12000,12000 }, // 4翻
            { 12000,12000,12000,12000,12000,12000,12000,12000,12000,12000,12000 }, // 5翻
            { 18000,18000,18000,18000,18000,18000,18000,18000,18000,18000,18000 }, // 6翻
            { 18000,18000,18000,18000,18000,18000,18000,18000,18000,18000,18000 }, // 7翻
            { 24000,24000,24000,24000,24000,24000,24000,24000,24000,24000,24000 }, // 8翻
            { 24000,24000,24000,24000,24000,24000,24000,24000,24000,24000,24000 }, // 9翻
            { 24000,24000,24000,24000,24000,24000,24000,24000,24000,24000,24000 }, //10翻
            { 36000,36000,36000,36000,36000,36000,36000,36000,36000,36000,36000 }, //11翻
            { 36000,36000,36000,36000,36000,36000,36000,36000,36000,36000,36000 }, //12翻
            { 48000,48000,48000,48000,48000,48000,48000,48000,48000,48000,48000 }, //13翻
        };

        int i = 0;
        int j = 0;
        if( han < 1 ) return score;
        if( han < 14 ) i = han-1;
        if( han > 13 ) i = 12;
        if( fu <= 20 ) j = 0;
        else if( fu <  30 ) j = 1;
        else if( fu <  40 ) j = 2;
        else if( fu <  50 ) j = 3;
        else if( fu <  60 ) j = 4;
        else if( fu <  70 ) j = 5;
        else if( fu <  80 ) j = 6;
        else if( fu <  90 ) j = 7;
        else if( fu < 100 ) j = 8;
        else if( fu < 110 ) j = 9;
        else j = 10;

        score[0] = scores[i][j] + honba * 300;
        score[1] = ((score[0]/3)%100 > 0) ? ((score[0]/3)/100+1) * 100 : ((score[0]/3)/100) * 100 ;
        score[2] = ((score[0]/3)%100 > 0) ? ((score[0]/3)/100+1) * 100 : ((score[0]/3)/100) * 100 ;

        return score;
    }

    public static int[] computeScoreKo(int fu, int han, int honba)
    {
        int[] score = {0,0,0};

        int[][] scores = {
            {     0,    0, 1000, 1300, 1600, 2000, 2300, 2600, 2900, 3200, 3600 }, // 1翻
            {  1300, 1600, 2000, 2600, 3200, 3900, 4500, 5200, 5800, 6400, 7100 }, // 2翻
            {  2600, 3200, 3900, 5200, 6400, 7700, 8000, 8000, 8000, 8000, 8000 }, // 3翻
            {  5200, 6400, 7700, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000 }, // 4翻
            {  8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000 }, // 5翻
            { 12000,12000,12000,12000,12000,12000,12000,12000,12000,12000,12000 }, // 6翻
            { 12000,12000,12000,12000,12000,12000,12000,12000,12000,12000,12000 }, // 7翻
            { 16000,16000,16000,16000,16000,16000,16000,16000,16000,16000,16000 }, // 8翻
            { 16000,16000,16000,16000,16000,16000,16000,16000,16000,16000,16000 }, // 9翻
            { 16000,16000,16000,16000,16000,16000,16000,16000,16000,16000,16000 }, //10翻
            { 24000,24000,24000,24000,24000,24000,24000,24000,24000,24000,24000 }, //11翻
            { 24000,24000,24000,24000,24000,24000,24000,24000,24000,24000,24000 }, //12翻
            { 32000,32000,32000,32000,32000,32000,32000,32000,32000,32000,32000 }, //13翻
        };

        int i = 0;
        int j = 0;
        if( han < 1 ) return score;
        if( han < 14 ) i = han-1;
        if( han > 13 ) i = 12;
        if( fu <= 20 ) j = 0;
        else if( fu <  30 ) j = 1;
        else if( fu <  40 ) j = 2;
        else if( fu <  50 ) j = 3;
        else if( fu <  60 ) j = 4;
        else if( fu <  70 ) j = 5;
        else if( fu <  80 ) j = 6;
        else if( fu <  90 ) j = 7;
        else if( fu < 100 ) j = 8;
        else if( fu < 110 ) j = 9;
        else j = 10;

        score[0] = scores[i][j] + honba * 300;
        score[1] = ((score[0]/4)%100 > 0) ? ((score[0]/4)/100+1) * 200 : ((score[0]/4)/100) * 200 ;
        score[2] = ((score[0]/4)%100 > 0) ? ((score[0]/4)/100+1) * 100 : ((score[0]/4)/100) * 100 ;

        return score;
    }
}
