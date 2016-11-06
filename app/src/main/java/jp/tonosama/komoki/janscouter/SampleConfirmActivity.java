package jp.tonosama.komoki.janscouter;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.AdUtil;
import jp.tonosama.komoki.util.DevLog;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

public class SampleConfirmActivity extends Activity {

    private static final String TAG = SampleConfirmActivity.class.getSimpleName();

    private AdView adView;

    private int   mScreenWidth;
    @SuppressWarnings("unused")
    private int   mScreenHeight;
    @SuppressWarnings("unused")
    private float mPixelDensity;

    private LinearLayout mContentArea;
    private Button       mShowDetailBtn;
    private ImageView[] image;

    private byte[] mSampleImg;

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    public SampleConfirmActivity(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }

    private void findAllViewById()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        mScreenWidth  = displayMetrics.widthPixels;
        mScreenHeight = displayMetrics.heightPixels;;
        mPixelDensity = displayMetrics.density;

        mContentArea   = (LinearLayout)findViewById(R.id.all_sample_area);
        mShowDetailBtn = (Button)findViewById(R.id.show_detail_sample_btn);
    }

    private void setAllViewAction()
    {
        mShowDetailBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent mIntent = new Intent(SampleConfirmActivity.this, SampleDetailConfirmActivity.class);
                mIntent.putExtra("SAMPLE_DETAIL_NO_KEY",0);
                startActivity(mIntent);
                //SampleConfirmActivity.this.finish();
            }
        });
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.sample_all_viewer);

        findAllViewById();
        setAllViewAction();

        // ============================================================================================
        // get sample hai image -> S
        int length = HiInfo.HI_WIDTH * HiInfo.HI_HEIGHT * HiInfo.TOTAL_HI_NUM ;
        String filename  = HiInfo.SAMPLE_IMAGE_FILE_NAME;

        if( getIsSaveInSDcard() ) {
            // ============================================================================================
            // sdcard に保存するパターン
            // デバッグのため現状はこっちで
            String directory = Environment.getExternalStorageDirectory().toString() + HiInfo.SAMPLE_IMAGE_FILE_PATH;
            FileInputStream in = null;
            File file = new File(directory, filename);
            try {
                in = new FileInputStream(file);
                mSampleImg = new byte[ length ];
                try {
                    in.read(mSampleImg);
                } catch (IOException e) {
                    e.printStackTrace();
                    //Toast.makeText(getApplicationContext(), "IOException", Toast.LENGTH_SHORT).show();
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                //Toast.makeText(getApplicationContext(), "FileNotFoundException", Toast.LENGTH_SHORT).show();
            }
        } else {
            // ============================================================================================
            // /data/data/～ に保存するパターン
            // とりあえずは保留
            BufferedInputStream in = null;
            try {
                FileInputStream file = openFileInput(filename);
                in = new BufferedInputStream(file);
                mSampleImg = new byte[ length ];
                in.read(mSampleImg, 0, length);
                file.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                //Toast.makeText(getApplicationContext(), "FileNotFoundException", Toast.LENGTH_SHORT).show();
            } catch(IOException e) {
                e.printStackTrace();
                //Toast.makeText(getApplicationContext(), "IOException", Toast.LENGTH_SHORT).show();
            }
        }
        // get sample hai image -> E
        // ============================================================================================

        if( mSampleImg==null ) {
            mSampleImg = new byte[ length ];
            for( int i=0;i<length;i++ ) {
                mSampleImg[i] = 0;
            }
            Toast.makeText(getApplicationContext(), "サンプルデータが見つかりませんでした。\n操作ガイドを確認してください。", Toast.LENGTH_LONG).show();
        }

        int[][] mHiArray = new int[HiInfo.TOTAL_HI_NUM][HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT];

        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ ){
            for( int j=0;j<HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT;j++ )
            {
                int value = mSampleImg[i*HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT + j] & 0xff;
                mHiArray[i][j] = Color.rgb(value, value, value);
            }
        }

        int imageWidth  = (int)((mScreenWidth-50f) / 9f);
        int imageHeight = (int)(imageWidth * 1.4f);

        ScrollView   wrapScrollView     = new ScrollView(this);
        LinearLayout resultLinearLayout = new LinearLayout(this);
        resultLinearLayout.setOrientation(LinearLayout.VERTICAL);
        //resultLinearLayout.setBackgroundColor(Color.LTGRAY);
        resultLinearLayout.setPadding(5, 5, 0, 0);

        Bitmap[]    hiBmp = new Bitmap[HiInfo.TOTAL_HI_NUM];
        image = new ImageView[HiInfo.TOTAL_HI_NUM * 2 + 2];
        LinearLayout[] eachRowLLayout = new LinearLayout[8];

        for( int i=0;i<8;i++ )
        {
            eachRowLLayout[i] = new LinearLayout(this);
            eachRowLLayout[i].setOrientation(LinearLayout.HORIZONTAL);
            eachRowLLayout[i].setPadding(0, 0, 0, 0);
        }

        TextView myHaiTitle = new TextView(this);
        myHaiTitle.setText("あなたの牌");
        myHaiTitle.setTextColor(Color.DKGRAY);
        resultLinearLayout.addView(myHaiTitle, MP, WC);

        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ )
        {
            hiBmp[i] = Bitmap.createBitmap(mHiArray[i], HiInfo.HI_WIDTH, HiInfo.HI_HEIGHT, Config.RGB_565);
            image[i] = new ImageView(this);
            image[i].setImageBitmap(hiBmp[i]);
            image[i].setScaleType(ImageView.ScaleType.FIT_XY);
            image[i].setPadding(5,5,5,5);
            // =====================================================
            // 牌をタップした際の処理 -> S
            final int num = i;
            image[i].setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    Intent mIntent = new Intent(SampleConfirmActivity.this, SampleDetailConfirmActivity.class);
                    mIntent.putExtra("SAMPLE_DETAIL_NO_KEY",num);
                    startActivity(mIntent);
                    //SampleConfirmActivity.this.finish();
                }
            });
            image[i].setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    if( event.getAction() == MotionEvent.ACTION_DOWN ) {
                        image[num].setBackgroundResource(R.drawable.selected_overlay_blue);
                    } else if ( event.getAction() == MotionEvent.ACTION_UP ) {
                        image[num].setBackgroundResource(R.drawable.transparent);
                    } else if ( event.getAction() == MotionEvent.ACTION_CANCEL ) {
                        image[num].setBackgroundResource(R.drawable.transparent);
                    }
                    return false;
                }
            });
            // 牌をタップした際の処理 -> E
            // =====================================================
            eachRowLLayout[i/9].addView(image[i], imageWidth, imageHeight);
            if(i%9==8 || i==HiInfo.TOTAL_HI_NUM-1) resultLinearLayout.addView(eachRowLLayout[i/9], MP, WC);
        }

        TextView presetHaiTitle = new TextView(this);
        presetHaiTitle.setText("正しい牌の並び");
        presetHaiTitle.setTextColor(Color.DKGRAY);
        resultLinearLayout.addView(presetHaiTitle, MP, WC);

        for( int i=0;i<36;i++ )
        {
            image[34+i] = new ImageView(this);
            if( i < HiInfo.TOTAL_HI_NUM )
                image[34+i].setImageResource(HiInfo.DEFINED_HAI_RESOURCES[i]);
            else
                image[34+i].setImageResource(HiInfo.DEFINED_HAI_BACK_RESOURCE);
            image[34+i].setScaleType(ImageView.ScaleType.FIT_XY);
            image[34+i].setPadding(5,5,5,5);
            // =====================================================
            // 牌をタップした際の処理 -> S
            final int num = i;
            image[34+i].setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    Intent mIntent = new Intent(SampleConfirmActivity.this, SampleDetailConfirmActivity.class);
                    mIntent.putExtra("SAMPLE_DETAIL_NO_KEY",num);
                    startActivity(mIntent);
                    //SampleConfirmActivity.this.finish();
                }
            });
            image[34+i].setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    if( event.getAction() == MotionEvent.ACTION_DOWN ) {
                        image[34+num].setBackgroundResource(R.drawable.selected_overlay_blue);
                    } else if ( event.getAction() == MotionEvent.ACTION_UP ) {
                        image[34+num].setBackgroundResource(R.drawable.transparent);
                    } else if ( event.getAction() == MotionEvent.ACTION_CANCEL ) {
                        image[34+num].setBackgroundResource(R.drawable.transparent);
                    }
                    return false;
                }
            });
            // 牌をタップした際の処理 -> E
            // =====================================================
            eachRowLLayout[4+i/9].addView(image[34+i], imageWidth, imageHeight);
            if(i%9==8 || i==35) resultLinearLayout.addView(eachRowLLayout[4+i/9], MP, WC);
        }

        wrapScrollView.addView(resultLinearLayout, MP, WC);
        mContentArea.addView(wrapScrollView);
        //addContentView(wrapScrollView, new LinearLayout.LayoutParams(MP, WC));

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

    public boolean getIsSaveInSDcard() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("SAVE_IN_SDCARD", false);
    }
}
