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
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

@SuppressWarnings("unused")
public class SampleDetailConfirmActivity extends Activity {

    private static final String TAG = SampleDetailConfirmActivity.class.getSimpleName();

    private AdView adView;

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    private int   mScreenWidth;
    private int   mScreenHeight;
    private float mPixelDensity;

    private MyPagerAdapter mPagerAdapter;
    private ViewPager      mViewPager;

    private Bitmap[]    haiBmp;
    private ImageView[] pagerIV;
    private TextView    pagetTV;
    private TextView    hnameTV;
    private Button      mShowAllBtn;

    private byte[] mSampleImg;

    private int mCurrentPos = 0;

    public SampleDetailConfirmActivity(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }

    private void getScreenSize()
    {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        mScreenWidth  = displayMetrics.widthPixels;
        mScreenHeight = displayMetrics.heightPixels;;
        mPixelDensity = displayMetrics.density;
        DevLog.d(TAG, "mScreenWidth  : " + String.valueOf(mScreenWidth) );
        DevLog.d(TAG, "mScreenHeight : " + String.valueOf(mScreenHeight) );
        DevLog.d(TAG, "mPixelDensity : " + String.valueOf(mPixelDensity) );
    }

    private void findAllViewById()
    {
        pagerIV = new ImageView[34];
        for( int i=0;i<34;i++ ) {
            pagerIV[i] = new ImageView(this);
            pagerIV[i].setImageBitmap(haiBmp[i]);
            pagerIV[i].setPadding(0,(int)(20f*mPixelDensity),0,(int)(10f*mPixelDensity));
        }
        pagetTV = (TextView)findViewById(R.id.sample_page_text);
        hnameTV = (TextView)findViewById(R.id.sample_hainame_text);

        mViewPager  = (ViewPager)findViewById(R.id.viewpager);
        mPagerAdapter = new MyPagerAdapter();
        mViewPager.setAdapter(mPagerAdapter);

        mShowAllBtn = (Button)findViewById(R.id.show_all_sample_btn);

        mViewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageSelected(int i) {
                mCurrentPos = i;
                hnameTV.setText( HiInfo.DEFINED_SAMPLE_HAI_NAMES[mCurrentPos]);
                pagetTV.setText( (mCurrentPos+1) + " / 34");
            }
            @Override
            public void onPageScrolled(int i, float f, int j) {
            }
            @Override
            public void onPageScrollStateChanged(int i) {
            }
        });
    }

    private void setButtonAction()
    {
        mShowAllBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //Intent mIntent = new Intent(SampleDetailConfirmActivity.this, SampleConfirmActivity.class);
                //startActivity(mIntent);
                SampleDetailConfirmActivity.this.finish();
            }
        });
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.sample_detail_viewer);
        getScreenSize();

        getHaiBitmapArray();
        findAllViewById();
        setButtonAction();

        Bundle bundle = getIntent().getExtras();
        if( bundle!=null ){
            mCurrentPos = bundle.getInt("SAMPLE_DETAIL_NO_KEY");
        }else{
            mCurrentPos = 0;
        }
        setCurrentPosition(mCurrentPos);

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

    private class MyPagerAdapter extends PagerAdapter {
        @Override
        public Object instantiateItem(ViewGroup container, int position)
        {
            View view = pagerIV[position];
            container.addView(view);
            return view;
        }
        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            ((ViewPager)container).removeView((View)object);
        }
        @Override
        public int getCount() {
            return 34;
        }
        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view.equals(object);
        }
    }

    private void setCurrentPosition(int position) {
        mCurrentPos = position;
        mViewPager.setCurrentItem(mCurrentPos);
        hnameTV.setText( HiInfo.DEFINED_SAMPLE_HAI_NAMES[mCurrentPos]);
        pagetTV.setText( (mCurrentPos+1) + " / 34");
    }

    private void getHaiBitmapArray()
    {
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
        }

        int[][] mHiArray = new int[HiInfo.TOTAL_HI_NUM][HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT];

        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ ){
            for( int j=0;j<HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT;j++ )
            {
                int value = mSampleImg[i*HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT + j] & 0xff;
                mHiArray[i][j] = Color.rgb(value, value, value);
            }
        }
        haiBmp = new Bitmap[HiInfo.TOTAL_HI_NUM];
        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ )
        {
            haiBmp[i] = Bitmap.createBitmap(mHiArray[i], HiInfo.HI_WIDTH, HiInfo.HI_HEIGHT, Config.RGB_565);
        }
    }

    public boolean getIsSaveInSDcard() {
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        return pref.getBoolean("SAVE_IN_SDCARD", false);
    }
}
