package jp.tonosama.komoki.janscouter;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.google.android.gms.ads.AdView;

import jp.tonosama.komoki.janscouterUtil.JanScouterPreference;
import jp.tonosama.komoki.util.AdUtil;
import jp.tonosama.komoki.util.DevLog;

public class JanScouterActivity extends Activity {

    private static final String TAG = JanScouterActivity.class.getSimpleName();

    private AdView adView;

    private Button mStartBtn;
    private Button mSampleConfirmBtn;
    private Button mUsageBtn;
    private Button mSettingBtn;
    private LinearLayout   mTitleArea;
    private RelativeLayout mStartSouterArea;
    private ImageView mStartScouterDroid;
    private CheckBox mIsDebugMode;

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    private int   mScreenWidth;
    private int   mScreenHeight;
    private float mPixelDensity;

    public JanScouterActivity(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }

    private void getScreenSize()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        mScreenWidth  = displayMetrics.widthPixels;
        mScreenHeight = displayMetrics.heightPixels;;
        mPixelDensity = displayMetrics.density;
        if( mScreenWidth > mScreenHeight ) {
            int tmp = mScreenWidth;
            mScreenWidth  = mScreenHeight;
            mScreenHeight = tmp;
        }
        DevLog.d(TAG, "mScreenWidth  : " + String.valueOf(mScreenWidth) );
        DevLog.d(TAG, "mScreenHeight : " + String.valueOf(mScreenHeight) );
        DevLog.d(TAG, "mPixelDensity : " + String.valueOf(mPixelDensity) );
    }

    private void findAllViewById()
    {
        mStartBtn         = (Button) findViewById(R.id.startBtn);
        mSampleConfirmBtn = (Button) findViewById(R.id.sampleConfirmBtn);
        mUsageBtn         = (Button) findViewById(R.id.usageBtn);
        mSettingBtn       = (Button) findViewById(R.id.settingBtn);
        mTitleArea        = (LinearLayout) findViewById(R.id.title_area);
        mStartSouterArea    = (RelativeLayout) findViewById(R.id.start_scouter_area);
        mStartScouterDroid  = (ImageView) findViewById(R.id.startScouterDroid);

        boolean isVertical = true;
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        if(displayMetrics.widthPixels>displayMetrics.heightPixels){
            isVertical = false;
        }else{
            isVertical = true;
        }

        // =======================================
        // レイアウト定義 -> S
        // =======================================
        int titleHeight     =  WC;
        int checkBoxbHeight =  WC;
        int bottomHeight    = (int)( mScreenWidth /  5f );
        int startBtnHeight  = (int)( mScreenWidth * (9f/10f) );

        int btnTextSize     = (int)( mScreenWidth  /  25f / mPixelDensity );
        int startTextSize   = (int)( mScreenWidth  /   8f / mPixelDensity );
        int bottomPadding   = (int)( mScreenHeight / 108f );

        int titleTextSize   = (int)((mScreenWidth)/7f/mPixelDensity);
        int titleMargin     = (int)((mScreenHeight)/40f);

        if( !isVertical ) {
            titleTextSize *= 0.85f;
            titleMargin    = (int)((mScreenWidth)/80f);
            bottomHeight  *= 0.85f;
        }

        LinearLayout.LayoutParams cblp = new LinearLayout.LayoutParams(MP,checkBoxbHeight);
        cblp.setMargins((int)((mScreenWidth)/27f),0,0,0);
        mIsDebugMode = (CheckBox) findViewById(R.id.result_setting_debug_btn);
        mIsDebugMode.setLayoutParams(cblp);
        mIsDebugMode.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);

        mIsDebugMode.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
        mStartBtn.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
        mSampleConfirmBtn.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
        mUsageBtn.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
        mSettingBtn.setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);

        TextView tv1 = ((TextView)findViewById(R.id.main_title_1));
        TextView tv2 = ((TextView)findViewById(R.id.main_title_2));

        LinearLayout.LayoutParams tlp = new LinearLayout.LayoutParams(WC,WC);
        tlp.setMargins(0,titleMargin,0,titleMargin);
        tv1.setLayoutParams(tlp);
        tv2.setLayoutParams(tlp);
        tv1.setTextSize(TypedValue.COMPLEX_UNIT_DIP,titleTextSize);
        tv2.setTextSize(TypedValue.COMPLEX_UNIT_DIP,titleTextSize);

        LinearLayout.LayoutParams titlelp = new LinearLayout.LayoutParams(MP,titleHeight);
        mTitleArea.setLayoutParams(titlelp);

        TextView tv3 = ((TextView)findViewById(R.id.startScouterBtnTxt));
        tv3.setTextSize(TypedValue.COMPLEX_UNIT_DIP,startTextSize);

        int startBottomMargin;
        if(isVertical) startBottomMargin = (int)((mScreenWidth)/10f);
        else           startBottomMargin = (int)((mScreenWidth)/128f);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(WC,WC);
        lp.setMargins(0, 0, 0, startBottomMargin);
        lp.addRule(RelativeLayout.CENTER_HORIZONTAL);
        lp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        tv3.setLayoutParams(lp);

        LinearLayout.LayoutParams llp2 = new LinearLayout.LayoutParams(MP,bottomHeight);
        ((LinearLayout)findViewById(R.id.main_bottom_area)).setLayoutParams(llp2);
        ((LinearLayout)findViewById(R.id.main_bottom_area)).setPadding(bottomPadding, bottomPadding, bottomPadding, bottomPadding);

        RelativeLayout.LayoutParams sttplm = new RelativeLayout.LayoutParams(startBtnHeight,startBtnHeight);
        sttplm.addRule(RelativeLayout.CENTER_IN_PARENT);
        mStartSouterArea.setLayoutParams(sttplm);
    }

    private void setButtonAction()
    {
        mStartBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.d(TAG, "start CameraPreview");
                Intent mIntent = new Intent(JanScouterActivity.this, SampleDetectorActivity.class);
                startActivity(mIntent);
            }
        });
        mSampleConfirmBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent mIntent = new Intent(JanScouterActivity.this, SampleConfirmActivity.class);
                startActivity(mIntent);
            }
        });
        mStartScouterDroid.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                boolean isTsumo   = false;
                boolean isReach   = false;
                boolean isIppatsu = false;
                boolean isRinshan = false;
                boolean isHaitei  = false;
                boolean isDReach  = false;
                boolean isOpenR   = false;
                boolean isTonba   = true;
                boolean isKazeTon = true;
                boolean isKazeNan = false;
                boolean isKazeSha = false;
                boolean isKazePee = false;
                boolean[] condition = { isTsumo,isReach,isIppatsu,isRinshan,isHaitei,isDReach,isOpenR,isTonba,isKazeTon,isKazeNan,isKazeSha,isKazePee };

                if(mIsDebugMode.isChecked() && getIsOnlyAnalyze())
                {
                    Intent mIntent = new Intent(getApplicationContext(), DetectedResultViewerActivity.class);
                    int[]     haiNum   = { 11,4,0,0 }; // tehai,nakihai,dora,honba
                    byte[]    tehai    = { 21,22,23,25,26,32,32,32,27,27,24 };
                    byte[]    dora     = { 0 };
                    byte[]    nakihai  = { 31,31,31,31,99 };
                    boolean[] nakiflag = { false,true,false,false,false };
                    int[]     result   = { 0,0,0 };
                    boolean[] yakulist = new boolean[60];
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_CONDITION, condition);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_HAINUM,    haiNum);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_TEHAI,     tehai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_DORA,      dora);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKI,      nakihai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKIFLAG,  nakiflag);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_RESULT,    result);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_YAKULIST,  yakulist);
                    startActivity(mIntent);
                    return;
                }

                if(mIsDebugMode.isChecked())
                {
                    Intent mIntent = new Intent(getApplicationContext(), DetectedResultViewerActivity.class);
                    int[]     haiNum   = { 11,3,0,0 }; // tehai,nakihai,dora,honba
                    byte[]    tehai    = { 21,22,23,25,26,32,32,32,27,27,24 };
                    byte[]    dora     = { 0 };
                    byte[]    nakihai  = { 0,0,0,0 };
                    boolean[] nakiflag = { false,true,false };
                    int[]     result   = { 0,0,0 };
                    boolean[] yakulist = new boolean[60];
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_CONDITION, condition);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_HAINUM,    haiNum);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_TEHAI,     tehai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_DORA,      dora);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKI,      nakihai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKIFLAG,  nakiflag);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_RESULT,    result);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_YAKULIST,  yakulist);
                    startActivity(mIntent);
                    return;
                }

                Intent mIntent = new Intent(JanScouterActivity.this, ResultDetectorActivity.class);
                mIntent.putExtra(ResultDetectorActivity.CONDITION_KEY, condition);
                startActivity(mIntent);
            }
        });
        mUsageBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent mIntent = new Intent(JanScouterActivity.this,UsageActivity.class);
                startActivity(mIntent);
            }
        });
        mSettingBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent mIntent = new Intent(JanScouterActivity.this,JanScouterPreference.class);
                startActivity(mIntent);
            }
        });
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.main);
        getScreenSize();

        findAllViewById();
        setButtonAction();
        getAllPreference();

        // create and set adView
        adView = new AdView(this);
        adView.setAdSize(com.google.android.gms.ads.AdSize.BANNER);
        adView.setAdUnitId(AdUtil.getAdUnitId());
        LinearLayout layout = (LinearLayout)findViewById(R.id.mainLayout);
        layout.addView(adView);
        com.google.android.gms.ads.AdRequest.Builder adRequestBuilder = new com.google.android.gms.ads.AdRequest.Builder();
        adView.loadAd(adRequestBuilder.build());
    }

    @Override
    public void onDestroy() {
        adView.destroy();
        super.onDestroy();
    }


    @Override
    protected void onPause() {
        super.onPause();
    }

    // ==============================================================================
    // プリファレンス関係 -> S
    //
    public void getAllPreference() {
        getScouterSize();
        getIsEnableSound();
        getIsOnlyAnalyze();
        getIsOnlyPreview();
        getIsSaveInSDcard();
    }
    public int getScouterSize() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return Integer.parseInt(pref.getString("SCOUTER_SIZE", "5"));
    }
    public boolean getIsEnableSound() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("IS_ENABLE_SOUND", false);
    }
    public boolean getIsOnlyAnalyze() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("IS_ONLY_ANALYZE", false);
    }
    public boolean getIsOnlyPreview() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("IS_ONLY_PREVIEW", false);
    }
    public boolean getIsSaveInSDcard() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("SAVE_IN_SDCARD", false);
    }
    //
    // プリファレンス関係 -> E
    // ==============================================================================
}
