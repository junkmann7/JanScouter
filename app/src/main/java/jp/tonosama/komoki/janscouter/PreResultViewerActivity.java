package jp.tonosama.komoki.janscouter;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

import jp.tonosama.komoki.util.AdUtil;
import jp.tonosama.komoki.util.DevLog;

public class PreResultViewerActivity extends Activity {

    private static final String TAG = PreResultViewerActivity.class.getSimpleName();

    private AdView adView;

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    private Button mRonBtn;
    private Button mTsuBtn;

    private boolean[] condition;
    private int[]     haiNum;
    private byte[]    tehai;
    private byte[]    dora;
    private byte[]    naki;
    private boolean[] nakiflag;
    private int[]     result;
    private boolean[] yakulist;

    private int   mScreenWidth;
    private int   mScreenHeight;
    private float mPixelDensity;

    public PreResultViewerActivity(){
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
        mRonBtn = (Button)findViewById(R.id.ron_btn);
        mTsuBtn = (Button)findViewById(R.id.tsu_btn);
    }

    private void setButtonAction()
    {
        mRonBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startDetectedResultViewer(false);
            }
        });
        mTsuBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startDetectedResultViewer(true);
            }
        });
    }

    public void startDetectedResultViewer(boolean isTsumo)
    {
        condition[0] = isTsumo;
        Intent mIntent = new Intent(getApplicationContext(), DetectedResultViewerActivity.class);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_CONDITION, condition);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_HAINUM,    haiNum);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_TEHAI,     tehai);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_DORA,      dora);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKI,      naki);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKIFLAG,  nakiflag);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_RESULT,    result);
        mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_YAKULIST,  yakulist);
        finish();
        startActivity(mIntent);
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.pre_result_setting);
        getScreenSize();

        // get bundle
        Bundle bundle = getIntent().getExtras();
        if( bundle!=null ){
            condition = bundle.getBooleanArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_CONDITION);
            haiNum    = bundle.getIntArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_HAINUM);
            tehai     = bundle.getByteArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_TEHAI);
            dora      = bundle.getByteArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_DORA);
            naki      = bundle.getByteArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKI);
            nakiflag  = bundle.getBooleanArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKIFLAG);
            result    = bundle.getIntArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_RESULT);
            yakulist  = bundle.getBooleanArray(DetectedResultViewerActivity.KEY_RESULT_VIEWER_YAKULIST);
        }else{
            finish();
        }

        findAllViewById();
        setButtonAction();

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

    @Override
    protected void onPause() {
        super.onPause();
    }
}
