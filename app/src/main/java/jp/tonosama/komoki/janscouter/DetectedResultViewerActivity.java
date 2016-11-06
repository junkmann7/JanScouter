package jp.tonosama.komoki.janscouter;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

import java.util.ArrayList;

import jp.tonosama.komoki.janscouterUtil.JanScouterUtil;
import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.AdUtil;
import jp.tonosama.komoki.util.DevLog;

public class DetectedResultViewerActivity extends Activity implements AnimationListener {

    private static final String TAG = DetectedResultViewerActivity.class.getSimpleName();

    private AdView adView;

    private LinearLayout mTehaiRow;
    private LinearLayout mDoraRow;
    private LinearLayout mNakiRow;
    private TextView     mAgariTxt;
    private TextView     mDebugTxt;
    private LinearLayout mPickArea;
    private LinearLayout mAgariResult;

    private ImageView[] tehai_iv;
    private ImageView[] naki_iv;
    private ImageView[] dora_iv;

    private int mSelectedTyp;
    private int mSelectedNum;
    private int mPickedHiNum;

    private Button[] mYakuBtn;
    private LinearLayout mTsumoBtn;

    private int      mHonbaNum = 0;
    private Button   mHonbaMinusBtn;
    private Button   mHonbaPlusBtn;
    private TextView mHonbaText;

    private int      mDoraNum = 0;
    private Button   mDoraMinusBtn;
    private Button   mDoraPlusBtn;
    private TextView mDoraText;

    private Button   mJikazeMinusBtn;
    private Button   mJikazePlusBtn;
    private TextView mJikazeText;

    private Button   mBakazeMinusBtn;
    private Button   mBakazePlusBtn;
    private TextView mBakazeText;

    private boolean[] condition;
    private int[]     haiNum;
    private byte[]    tehai;
    private byte[]    dora;
    private byte[]    naki;
    private boolean[] nakiflag;
    private int[]     result;
    private boolean[] yakulist;

    private int[] yaku_button_back = {
            R.drawable.yaku_button_00, R.drawable.yaku_button_01, R.drawable.yaku_button_02,
            R.drawable.yaku_button_03, R.drawable.yaku_button_04,
    };

    private int[] oyanoten;
    private int[] konoten;

    public static final String KEY_RESULT_VIEWER_CONDITION = "KEY_RESULT_VIEWER_CONDITION";
    public static final String KEY_RESULT_VIEWER_HAINUM    = "KEY_RESULT_VIEWER_HAINUM";
    public static final String KEY_RESULT_VIEWER_TEHAI     = "KEY_RESULT_VIEWER_TEHAI";
    public static final String KEY_RESULT_VIEWER_DORA      = "KEY_RESULT_VIEWER_DORA";
    public static final String KEY_RESULT_VIEWER_NAKI      = "KEY_RESULT_VIEWER_NAKI";
    public static final String KEY_RESULT_VIEWER_NAKIFLAG  = "KEY_RESULT_VIEWER_NAKIFLAG";
    public static final String KEY_RESULT_VIEWER_RESULT    = "KEY_RESULT_VIEWER_RESULT";
    public static final String KEY_RESULT_VIEWER_YAKULIST  = "KEY_RESULT_VIEWER_YAKULIST";

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    private int   mScreenWidth;
    private int   mScreenHeight;
    private float mPixelDensity;

    public DetectedResultViewerActivity() {
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }

    private void getScreenSize()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        mScreenWidth  = displayMetrics.widthPixels;
        mScreenHeight = displayMetrics.heightPixels;
        mPixelDensity = displayMetrics.density;
        DevLog.d(TAG, "mScreenWidth  : " + String.valueOf(mScreenWidth) );
        DevLog.d(TAG, "mScreenHeight : " + String.valueOf(mScreenHeight) );
        DevLog.d(TAG, "mPixelDensity : " + String.valueOf(mPixelDensity) );
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.result_viewer);

        getScreenSize();

        // get bundle
        Bundle bundle = getIntent().getExtras();
        if( bundle!=null ){
            condition = bundle.getBooleanArray(KEY_RESULT_VIEWER_CONDITION);
            haiNum    = bundle.getIntArray(KEY_RESULT_VIEWER_HAINUM);
            tehai     = bundle.getByteArray(KEY_RESULT_VIEWER_TEHAI);
            dora      = bundle.getByteArray(KEY_RESULT_VIEWER_DORA);
            naki      = bundle.getByteArray(KEY_RESULT_VIEWER_NAKI);
            nakiflag  = bundle.getBooleanArray(KEY_RESULT_VIEWER_NAKIFLAG);
            result    = bundle.getIntArray(KEY_RESULT_VIEWER_RESULT);
            yakulist  = bundle.getBooleanArray(KEY_RESULT_VIEWER_YAKULIST);
        }else{
            finish();
        }

        findAllViewById();
        setAllViewAction();

        reComputeResult( false, false );

        // create and set adView
        adView = new AdView(this);
        adView.setAdSize(AdSize.BANNER);
        adView.setAdUnitId(AdUtil.getAdUnitId());
        LinearLayout layout = (LinearLayout)findViewById(R.id.mainLayout);
        layout.addView(adView);
        AdRequest.Builder adRequestBuilder = new AdRequest.Builder();
        adView.loadAd(adRequestBuilder.build());
    }

    @Override
    public void onDestroy() {
        adView.destroy();
        super.onDestroy();
    }

    private void findAllViewById()
    {
        mTehaiRow = (LinearLayout)findViewById(R.id.result_tehai_row);
        mDoraRow  = (LinearLayout)findViewById(R.id.result_dora_row);
        mNakiRow  = (LinearLayout)findViewById(R.id.result_naki_row);
        mAgariTxt = (TextView)findViewById(R.id.result_agari_text);
        mDebugTxt = (TextView)findViewById(R.id.result_debug_text);
        mPickArea = (LinearLayout)findViewById(R.id.result_pickarea);

        mAgariResult = (LinearLayout)findViewById(R.id.agari_result_area);

        mYakuBtn = new Button[7];
        mYakuBtn[0] = (Button) findViewById(R.id.result_tsumo_btn);
        mYakuBtn[1] = (Button) findViewById(R.id.result_reach_btn);
        mYakuBtn[2] = (Button) findViewById(R.id.result_ippatsu_btn);
        mYakuBtn[3] = (Button) findViewById(R.id.result_rinshan_btn);
        mYakuBtn[4] = (Button) findViewById(R.id.result_haitei_btn);
        mYakuBtn[5] = (Button) findViewById(R.id.result_double_btn);
        mYakuBtn[6] = (Button) findViewById(R.id.result_open_btn);

        mTsumoBtn   = (LinearLayout) findViewById(R.id.result_tsumo_toggle_btn);

        for( int i=0;i<mYakuBtn.length;i++ ) {
            if( condition[i] ) mYakuBtn[i].setBackgroundResource(R.drawable.dball_enable);
            else               mYakuBtn[i].setBackgroundResource(R.drawable.dball_disable);

            int txtSize   = (int)((mScreenWidth)/48f/mPixelDensity);
            int btnWidth  = (int)((mScreenWidth-20f*mPixelDensity)/8f);
            int btnHeight = (int)((mScreenWidth-20f*mPixelDensity)/8f/1.5f);
            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(btnWidth,btnHeight);
            mYakuBtn[i].setLayoutParams(lp);
            mYakuBtn[i].setTextSize(txtSize);
            if(i==0) {
                mTsumoBtn.setLayoutParams(lp);
                ((TextView)findViewById(R.id.result_tsumo_txt)).setTextSize(TypedValue.COMPLEX_UNIT_DIP,txtSize);
                ((TextView)findViewById(R.id.result_tsumo_txt)).setPadding( 0, 0, (int)(btnWidth/(72f/12f)), 0 );
                ((TextView)findViewById(R.id.result_ron_txt)).setTextSize(TypedValue.COMPLEX_UNIT_DIP,txtSize);
                ((TextView)findViewById(R.id.result_ron_txt)).setPadding( (int)(btnWidth/(72f/12f)), 0, 0, 0 );
            }
        }

        int txtSize   = (int)((mScreenWidth)/36f/mPixelDensity);

        mHonbaPlusBtn  = (Button) findViewById(R.id.result_honba_plus);
        mHonbaMinusBtn = (Button) findViewById(R.id.result_honba_minus);
        mHonbaText     = (TextView) findViewById(R.id.result_honba_text);
        mHonbaPlusBtn.setText(String.format("＞"));
        mHonbaMinusBtn.setText(String.format("＜"));
        mHonbaText.setText(String.format("%d 本場", mHonbaNum));

        mHonbaPlusBtn.setTextSize(txtSize);
        mHonbaMinusBtn.setTextSize(txtSize);
        mHonbaText.setTextSize(txtSize);

        mDoraPlusBtn  = (Button) findViewById(R.id.result_dora_plus);
        mDoraMinusBtn = (Button) findViewById(R.id.result_dora_minus);
        mDoraText     = (TextView) findViewById(R.id.result_dora_text);
        mDoraPlusBtn.setText(String.format("＞"));
        mDoraMinusBtn.setText(String.format("＜"));
        mDoraText.setText(String.format("ドラ %d", mDoraNum));

        mDoraPlusBtn.setTextSize(txtSize);
        mDoraMinusBtn.setTextSize(txtSize);
        mDoraText.setTextSize(txtSize);

        mJikazePlusBtn  = (Button) findViewById(R.id.result_jikaze_plus);
        mJikazeMinusBtn = (Button) findViewById(R.id.result_jikaze_minus);
        mJikazeText     = (TextView) findViewById(R.id.result_jikaze_text);
        mJikazePlusBtn.setText(String.format("＞"));
        mJikazeMinusBtn.setText(String.format("＜"));
        String jikaze = "";
        if(condition[8])  jikaze = "東";
        if(condition[9])  jikaze = "南";
        if(condition[10]) jikaze = "西";
        if(condition[11]) jikaze = "北";
        mJikazeText.setText("自風 - " + jikaze);

        mJikazePlusBtn.setTextSize(txtSize);
        mJikazeMinusBtn.setTextSize(txtSize);
        mJikazeText.setTextSize(txtSize);

        mBakazePlusBtn  = (Button) findViewById(R.id.result_bakaze_plus);
        mBakazeMinusBtn = (Button) findViewById(R.id.result_bakaze_minus);
        mBakazeText     = (TextView) findViewById(R.id.result_bakaze_text);
        mBakazePlusBtn.setText(String.format("＞"));
        mBakazeMinusBtn.setText(String.format("＜"));
        String bakaze = "";
        if(condition[7]) bakaze = "東";
        else             bakaze = "南";
        mBakazeText.setText("場風 - " + bakaze);

        mBakazePlusBtn.setTextSize(txtSize);
        mBakazeMinusBtn.setTextSize(txtSize);
        mBakazeText.setTextSize(txtSize);

        txtSize   = (int)((mScreenWidth)/18f/mPixelDensity);
        mAgariTxt.setTextSize(txtSize);
        mDebugTxt.setTextSize(txtSize/2);
    }

    private void setAllViewAction()
    {
        mJikazePlusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                int num = 0;
                for( int i=0;i<4;i++ ) {
                    if( condition[8+i] ) {
                        condition[8+i] = false;
                        num = (i+1)%4;
                    }
                }
                condition[8+num] = true;
                String jikaze = "";
                if(condition[8])  jikaze = "東";
                if(condition[9])  jikaze = "南";
                if(condition[10]) jikaze = "西";
                if(condition[11]) jikaze = "北";
                mJikazeText.setText("自風 - " + jikaze);
                reComputeResult( true, false );
            }
        });
        mJikazeMinusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                int num = 0;
                for( int i=0;i<4;i++ ) {
                    if( condition[8+i] ) {
                        condition[8+i] = false;
                        num = (i+3)%4;
                    }
                }
                condition[8+num] = true;
                String jikaze = "";
                if(condition[8])  jikaze = "東";
                if(condition[9])  jikaze = "南";
                if(condition[10]) jikaze = "西";
                if(condition[11]) jikaze = "北";
                mJikazeText.setText("自風 - " + jikaze);
                reComputeResult( true, false );
            }
        });

        mBakazePlusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if( condition[7] ) {
                    condition[7] = false;
                    mBakazeText.setText("場風 - 南");
                }else{
                    condition[7] = true;
                    mBakazeText.setText("場風 - 東");
                }
                reComputeResult( true, false );
            }
        });
        mBakazeMinusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                if( condition[7] ) {
                    condition[7] = false;
                    mBakazeText.setText("場風 - 南");
                }else{
                    condition[7] = true;
                    mBakazeText.setText("場風 - 東");
                }
                reComputeResult( true, false );
            }
        });

        mHonbaPlusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                mHonbaNum++;
                if( mHonbaNum <  0 ) mHonbaNum =  0;
                if( mHonbaNum > 20 ) mHonbaNum = 20;
                haiNum[3] = mHonbaNum;
                mHonbaText.setText(String.format("%d 本場", mHonbaNum));
                reComputeResult( true, false );
            }
        });
        mHonbaMinusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                mHonbaNum--;
                if( mHonbaNum <  0 ) mHonbaNum =  0;
                if( mHonbaNum > 20 ) mHonbaNum = 20;
                haiNum[3] = mHonbaNum;
                mHonbaText.setText(String.format("%d 本場", mHonbaNum));
                reComputeResult( true, false );
            }
        });
        mDoraPlusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                mDoraNum++;
                if( mDoraNum <  0 ) mDoraNum =  0;
                if( mDoraNum > 20 ) mDoraNum = 20;
                mDoraText.setText(String.format("ドラ %d", mDoraNum));
                reComputeResult( true, false );
            }
        });
        mDoraMinusBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                mDoraNum--;
                if( mDoraNum <  0 ) mDoraNum =  0;
                if( mDoraNum > 20 ) mDoraNum = 20;
                mDoraText.setText(String.format("ドラ %d", mDoraNum));
                reComputeResult( true, false );
            }
        });

        for( int i=0;i<mYakuBtn.length;i++ ) {
            final int num = i;
            mYakuBtn[num].setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    // 鳴いてたら リーチ/一発/Ｗリーチ/オープン は押せない対応
                    if( num==1 || num==2 | num==5 || num==6 ) {
                        if( haiNum[1] == 0 ) {
                            condition[num] = !condition[num];
                            if(condition[num])    mYakuBtn[num].setBackgroundResource(R.drawable.dball_enable);
                            else                  mYakuBtn[num].setBackgroundResource(R.drawable.dball_disable);
                            reComputeResult( true, true );
                        }
                        // ツモ/ロン の切り替えだけ特別対応
                    } else if( num==0 ) {
                        condition[num] = !condition[num];
                        if(condition[num]) {
                            mYakuBtn[num].setBackgroundResource(R.drawable.dball_enable);
                            mYakuBtn[num].setText(getApplicationContext().getResources().getString(R.string.pre_result_setting_tsumo));
                        } else {
                            mYakuBtn[num].setBackgroundResource(R.drawable.dball_inactive);
                            mYakuBtn[num].setText(getApplicationContext().getResources().getString(R.string.pre_result_setting_ron));
                        }
                        reComputeResult( true, true );
                    } else {
                        condition[num] = !condition[num];
                        if(condition[num])    mYakuBtn[num].setBackgroundResource(R.drawable.dball_enable);
                        else                  mYakuBtn[num].setBackgroundResource(R.drawable.dball_disable);
                        reComputeResult( true, true );
                    }
                }
            });
        }

        mTsumoBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                condition[0] = !condition[0];
                mTsumoBtn.setVisibility(View.GONE);
                mYakuBtn[0].setVisibility(View.VISIBLE);
                mYakuBtn[0].setBackgroundResource(R.drawable.dball_enable);
                /*
                if(condition[0]) {
                    ((TextView)findViewById(R.id.result_tsumo_txt)).setTextColor(Color.BLACK);
                    ((TextView)findViewById(R.id.result_ron_txt)).setTextColor(Color.WHITE);
                    ((LinearLayout)findViewById(R.id.result_tsumo_toggle_btn)).setBackgroundResource(R.drawable.tsumo_toggle_on);
                } else {
                    ((TextView)findViewById(R.id.result_ron_txt)).setTextColor(Color.BLACK);
                    ((TextView)findViewById(R.id.result_tsumo_txt)).setTextColor(Color.WHITE);
                    ((LinearLayout)findViewById(R.id.result_tsumo_toggle_btn)).setBackgroundResource(R.drawable.tsumo_toggle_off);
                }*/
                reComputeResult( true, true );
            }
        });
    }

    private void reComputeResult( boolean is_recompute, boolean animflg )
    {
        DevLog.d(TAG, "== ComputeResult Start ======================");
        String cond_str = "";
        String tehi_str = "";
        String dora_str = "";
        String naki_str = "";
        for( int i=0;i<condition.length;i++ ) cond_str += condition[i] + ",";
        for( int i=0;i<haiNum[0];i++ )        tehi_str += tehai[i] + ",";
        for( int i=0;i<haiNum[2];i++ )        dora_str += dora[i] + ",";
        for( int i=0;i<haiNum[1];i++ )        naki_str += naki[i] + ",";
        DevLog.d(TAG,"条件: " + cond_str);
        DevLog.d(TAG,"手牌: " + tehi_str);
        DevLog.d(TAG,"ドラ: " + dora_str);
        DevLog.d(TAG,"鳴き: " + naki_str);
        DevLog.d(TAG,"本場: " + haiNum[3]);
        int point_result = JanScouterUtil.JanComputeResult(condition, tehai, dora, naki, nakiflag, haiNum, result, yakulist, is_recompute);
        if( !(result[1] == 0 && result[2] == 0) ) result[1] += mDoraNum;
        DevLog.d(TAG,"譜: " + result[0]);
        DevLog.d(TAG,"翻: " + result[1]);
        DevLog.d(TAG,"点: " + result[2]);
        oyanoten = HiInfo.computeScoreOya(result[0], result[1], haiNum[3]);
        DevLog.d(TAG,"親ロン: " + oyanoten[0]);
        DevLog.d(TAG,"親ツモ: " + oyanoten[1]);
        konoten = HiInfo.computeScoreKo(result[0], result[1], haiNum[3]);
        DevLog.d(TAG,"子ロン: " + konoten[0]);
        DevLog.d(TAG,"子ツモ(親): " + konoten[1]);
        DevLog.d(TAG,"子ツモ(子): " + konoten[2]);

        setDetectedResult( point_result );

        DevLog.d(TAG, "== ComputeResult End ========================");

        if( !animflg ) return;

        for( int i=0;i<7;i++ ) {
            if( !condition[i] ) return;
        }

        startShengLongAnim();
    }

    private void startShengLongAnim() {

        ImageView sh_iv = (ImageView)findViewById(R.id.img_shenron);
        sh_iv.setVisibility(View.VISIBLE);

        int mDuration = 2000;
        ScaleAnimation scale = new ScaleAnimation(0.5f,2f,0.5f,2f);
        scale.setDuration(mDuration);
        scale.setInterpolator(new DecelerateInterpolator());

        AlphaAnimation alpha = new AlphaAnimation(1f,0f);
        alpha.setDuration(mDuration);
        alpha.setInterpolator(new DecelerateInterpolator());

        // (fromX,toX,fromY,toY)
        TranslateAnimation trans = new TranslateAnimation(
                (mScreenWidth  - sh_iv.getWidth() *0.5f)/2f,
                (mScreenWidth  - sh_iv.getWidth() *2.0f)/2f,
                (mScreenHeight - sh_iv.getHeight()*0.5f)/2.5f,
                (mScreenHeight - sh_iv.getHeight()*2.0f)/2.5f );
        trans.setDuration(mDuration);
        trans.setInterpolator(new DecelerateInterpolator());

        AnimationSet scale_set = new AnimationSet(true);
        scale_set.addAnimation(scale);
        scale_set.addAnimation(alpha);
        scale_set.addAnimation(trans);
        scale_set.setAnimationListener(this);
        sh_iv.startAnimation(scale_set);
    }

    private void setDetectedResult( int ret )
    {
        mPickArea.removeAllViews();
        mTehaiRow.removeAllViews();
        mDoraRow.removeAllViews();
        mNakiRow.removeAllViews();
        mAgariResult.removeAllViews();

        // =================================================================
        // Pick するためのすべての牌の画像ボタン
        ImageView[] pick_iv = new ImageView[ 37 ];
        for( int i=0;i<37;i++ )
        {
            pick_iv[i] = new ImageButton(this);
            //pick_iv[i].setImageResource( HiInfo.DEFINED_HAI_RESOURCES[ i ] );
            pick_iv[i].setBackgroundResource( HiInfo.DEFINED_HAI_RESOURCES[ i ] );
            int haiWidth  = (int)((mScreenWidth-30f*mPixelDensity)/15f);
            int haiHeight = (int)((haiWidth*128/88));
            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(haiWidth,haiHeight);
            lp.setMargins(haiHeight/64,haiHeight/64,haiHeight/64,haiHeight/64);
            pick_iv[i].setLayoutParams(lp);
            final int num = i;
            pick_iv[i].setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    mPickedHiNum = num;
                    if(mSelectedTyp==0) tehai[ mSelectedNum ] = (byte) mPickedHiNum;
                    if(mSelectedTyp==1) naki[ mSelectedNum ]  = (byte) mPickedHiNum;
                    if(mSelectedTyp==2) dora[ mSelectedNum ]  = (byte) mPickedHiNum;
                    reComputeResult( true, false );
                }
            });
            mPickArea.addView(pick_iv[i]);
        }
        mPickArea.setVisibility(View.GONE);

        // =================================================================
        // 手牌の画像ボタン
        if( haiNum[0] > 0 ){
            tehai_iv = new ImageView[ haiNum[0] ];
            for( int i=0;i<haiNum[0];i++ )
            {
                tehai_iv[i] = new ImageButton(this);
                //tehai_iv[i].setImageResource( HiInfo.DEFINED_HAI_RESOURCES[ tehai[i] ] );
                tehai_iv[i].setBackgroundResource( HiInfo.DEFINED_HAI_RESOURCES[ tehai[i] ] );
                int haiWidth  = (int)((mScreenWidth-40f*mPixelDensity)/15f);
                int haiHeight = (int)((haiWidth*128/88));
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(haiWidth,haiHeight);
                if( i!=haiNum[0]-1 ) lp.setMargins(haiHeight/64,haiHeight/64,haiHeight/64,haiHeight/64);
                else                 lp.setMargins((int)(haiHeight/64f*20f),haiHeight/64,haiHeight/64,haiHeight/64);
                tehai_iv[i].setLayoutParams(lp);
                tehai_iv[i].setPadding(0,0,0,0);
                final int num = i;
                tehai_iv[i].setOnClickListener(new View.OnClickListener() {
                    public void onClick(View v) {
                        if( mPickArea.getVisibility() == View.VISIBLE ){
                            mPickArea.setVisibility(View.GONE);
                            if(mSelectedTyp==0) tehai_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==1) naki_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==2) dora_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                        }else{
                            mSelectedTyp = 0;
                            mSelectedNum = num;
                            mPickArea.setVisibility(View.VISIBLE);
                            tehai_iv[num].setImageResource(R.drawable.selected_overlay);
                            tehai_iv[num].setScaleType(ScaleType.FIT_XY);
                        }
                    }
                });
                mTehaiRow.addView( tehai_iv[i] );
            }
        }

        // =================================================================
        // ドラ牌の画像ボタン
        if( haiNum[2] > 0 ){
            dora_iv = new ImageView[ haiNum[2] ];
            for( int i=0;i<haiNum[2];i++ )
            {
                dora_iv[i] = new ImageButton(this);
                //dora_iv[i].setImageResource( HiInfo.DEFINED_HAI_RESOURCES[ dora[i] ] );
                dora_iv[i].setBackgroundResource( HiInfo.DEFINED_HAI_RESOURCES[ dora[i] ] );
                int haiWidth  = (int)((mScreenWidth-30f*mPixelDensity)/15f);
                int haiHeight = (int)((haiWidth*128/88));
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(haiWidth,haiHeight);
                lp.setMargins(haiHeight/64,haiHeight/64,haiHeight/64,haiHeight/64);
                dora_iv[i].setLayoutParams(lp);
                dora_iv[i].setPadding(0,0,0,0);
                final int num = i;
                dora_iv[i].setOnClickListener(new View.OnClickListener() {
                    public void onClick(View v) {
                        if( mPickArea.getVisibility() == View.VISIBLE ){
                            mPickArea.setVisibility(View.GONE);
                            if(mSelectedTyp==0) tehai_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==1) naki_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==2) dora_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                        }else{
                            mSelectedTyp = 2;
                            mSelectedNum = num;
                            mPickArea.setVisibility(View.VISIBLE);
                            dora_iv[num].setImageResource(R.drawable.selected_overlay);
                            dora_iv[num].setScaleType(ScaleType.FIT_XY);
                        }
                    }
                });
                mDoraRow.addView( dora_iv[i] );
            }
        }
        // =================================================================
        // 鳴き牌の画像ボタン
        if( haiNum[1] > 0 ){
            naki_iv = new ImageView[ haiNum[1] ];
            for( int i=0;i<haiNum[1];i++ )
            {
                naki_iv[i] = new ImageButton(this);
                int haiWidth  = (int)((mScreenWidth-30f*mPixelDensity)/15f);
                int haiHeight = (int)((haiWidth*128/88));
                int topMargin = 0;
                if( nakiflag[i] ){ // 鳴いた牌だったら
                    Bitmap haiBitmap = BitmapFactory.decodeResource(getResources(), HiInfo.DEFINED_HAI_RESOURCES[ naki[i] ] );
                    Matrix matrix = new Matrix();
                    matrix.postRotate(90f);
                    haiBitmap = Bitmap.createBitmap(haiBitmap, 0, 0, haiBitmap.getWidth(), haiBitmap.getHeight(), matrix, true);
                    //naki_iv[i].setBackgroundResource( HiInfo.DEFINED_NAKI_HAI_RESOURCES[ naki[i] ] );
                    naki_iv[i].setBackgroundDrawable( new BitmapDrawable(haiBitmap) );
                    int tmp   = haiWidth;
                    haiWidth  = haiHeight;
                    haiHeight = tmp;
                    topMargin = haiWidth - haiHeight;
                }else{
                    naki_iv[i].setBackgroundResource( HiInfo.DEFINED_HAI_RESOURCES[ naki[i] ] );
                }
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(haiWidth,haiHeight);
                lp.setMargins(haiHeight/64,haiHeight/64+topMargin,haiHeight/64,haiHeight/64);
                naki_iv[i].setLayoutParams(lp);
                naki_iv[i].setPadding(0,0,0,0);
                final int num = i;
                naki_iv[i].setOnClickListener(new View.OnClickListener() {
                    public void onClick(View v) {
                        if( mPickArea.getVisibility() == View.VISIBLE ){
                            mPickArea.setVisibility(View.GONE);
                            if(mSelectedTyp==0) tehai_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==1) naki_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                            if(mSelectedTyp==2) dora_iv[mSelectedNum].setImageResource(R.drawable.transparent);
                        }else{
                            mSelectedTyp = 1;
                            mSelectedNum = num;
                            mPickArea.setVisibility(View.VISIBLE);
                            naki_iv[num].setImageResource(R.drawable.selected_overlay);
                            naki_iv[num].setScaleType(ScaleType.FIT_XY);
                        }
                    }
                });
                mNakiRow.addView( naki_iv[i] );
            }
        }

        // =================================================================
        // 結果の表示 -> S
        String mResultStr = "";

        // チョンボだったら
        //if( result[1] ==0 && result[2] == 0 ) {
        if( ret == -2 ) {
            mResultStr += "チョンボ！";
            // 役満でなかったら
        } else if( result[1] < 13 && result[0] != -1 ) {
            mResultStr += result[0] + "譜";
            mResultStr += result[1] + "翻";
            // 役満だったら
        } else {
            mResultStr += "役満！";
        }

        // チョンボだったら
        //if( result[1] ==0 && result[2] == 0 ) {
        if( ret == -2 ) {
            if( condition[8] ) {
                mResultStr += "-12000" + "点";
            } else {
                mResultStr +=  "-8000" + "点";
            }
            // チョンボでなかったら
        } else {
            if( condition[8] ) {
                mResultStr += oyanoten[0] + "点\n（"     + oyanoten[1] + "点オール）";
            } else {
                mResultStr += konoten[0]  + "点\n（親: " + konoten[1]  + "点 , 子: " + konoten[2] + "点）";
            }
        }
        mAgariTxt.setText(mResultStr);

        // =================================================================
        // あがり役ブロックの表示 -> S
        // =====================================================================================================
        // ArrayList<String>  yakuNames  -> 役の名前：飜牌(発),一気通貫,混一色,混一色,ドラ,ドラ ･･･ みたいな並び
        // ArrayList<Integer> yakuValues -> 役の値　：1,1,2,2 ･･･ みたいな並び
        // =====================================================================================================
        ArrayList<String>  yakuNames = new ArrayList<String>();
        ArrayList<Integer> yakuValue = new ArrayList<Integer>();
        for( int i=0;i<yakulist.length;i++ ) {
            if( yakulist[i] ) {
                int yaku_value = 0;
                // 鳴いてる/鳴いてないで翻数が変わるので...
                if(haiNum[1]==0) yaku_value = HiInfo.YAKU_VALUE_LIST[i];
                else             yaku_value = HiInfo.YAKU_VALUE_LIST_NAKI[i];
                yakuValue.add(yaku_value);
                for( int j=0;j<yaku_value;j++ )  yakuNames.add(HiInfo.YAKU_NAME_LIST[i]);
            }
        }
        // ドラがあれば１つの役として追加
        // あがっていなければ追加しない
        if( !(result[1]==0&&result[2]==0) ) {
            if( mDoraNum>0 ) {
                yakuValue.add(mDoraNum);
                for( int j=0;j<mDoraNum;j++ ) yakuNames.add("ドラ");
            }
        }
        // --------------------------------------------------------------------------------
        // -- デバッグ用 -> S
        for( int i=0;i<yakuNames.size();i++ ) {
            DevLog.d(TAG, String.format( "役%02d: %s", i+1, yakuNames.get(i) ) );
        }
        // -- デバッグ用 -> E
        // --------------------------------------------------------------------------------
        LinearLayout[] rowLL = new LinearLayout[4];
        int yakuTxtSize = (int)((mScreenWidth)/32f/mPixelDensity);    // 役名の文字サイズ
        int yakuLength  = (int)((mScreenWidth-20f*mPixelDensity)/4f); // 役ボタンの横幅
        int btn_num = 0;       // 何個目のボタンか
        String yaku_name = ""; // 前のボタンの役の名前
        int yakuCount = 0;     // 何個目の役か
        for( int rowNum=0;rowNum<3;rowNum++ )
        {
            rowLL[rowNum] = new LinearLayout(this);

            for( int btnNum=0;btnNum<4;btnNum++ )
            {
                Button yakuTxt = new Button(this);
                yakuTxt.setTextColor(Color.BLACK);
                yakuTxt.setTextSize(yakuTxtSize);
                yakuTxt.setShadowLayer(3.0f, 2.0f, 3.2f, Color.LTGRAY);
                yakuTxt.setGravity(Gravity.CENTER);

                if( btn_num<yakuNames.size() )
                {
                    if(yaku_name != yakuNames.get(btn_num)) yakuCount++;
                    yaku_name = yakuNames.get(btn_num);
                    yakuTxt.setText( yakuNames.get(btn_num) );
                    yakuTxt.setBackgroundResource( yaku_button_back[yakuCount%5] );
                } else {
                    yakuTxt.setBackgroundResource( R.drawable.yaku_button_none );
                }
                rowLL[rowNum].addView(yakuTxt,yakuLength,MP);
                btn_num++;
            }

            mAgariResult.addView(rowLL[rowNum],WC,WC);
        }
        // あがり役ブロックの表示 -> E
        // =================================================================

        String mDebugStr = "";
        mDebugStr += HiInfo.DEBUG_HAI_TYPE_NAME[0] + haiNum[0] + "、";
        mDebugStr += HiInfo.DEBUG_HAI_TYPE_NAME[1] + haiNum[1] + "、";
        mDebugStr += HiInfo.DEBUG_HAI_TYPE_NAME[2] + haiNum[2] + "\n";
        mDebugStr += HiInfo.DEBUG_CONDITION_TITLE;
        for( int i=0;i<condition.length;i++ ) {
            String c_name = HiInfo.DEBUG_CONDITION_NAME[i];
            mDebugStr += condition[i] ? c_name + "(○)" : c_name + "(×)";
        }
        mDebugTxt.setText(mDebugStr);
        // 結果の表示 -> E
        // =================================================================
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.result_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.detect_again:
                Intent mIntent = new Intent(getApplicationContext(), ResultDetectorActivity.class);
                mIntent.putExtra(ResultDetectorActivity.CONDITION_KEY, condition);
                finish();
                startActivity(mIntent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    public void onAnimationEnd(Animation arg0) {
        ((ImageView)findViewById(R.id.img_shenron)).setVisibility(View.GONE);
    }

    @Override
    public void onAnimationRepeat(Animation animation) {
    }

    @Override
    public void onAnimationStart(Animation animation) {
    }
}
