package jp.tonosama.komoki.janscouter;

import jp.tonosama.komoki.util.AdUtil;
import jp.tonosama.komoki.util.DevLog;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.LayoutInflater;
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

public class UsageActivity extends Activity {

    private static final String TAG = UsageActivity.class.getSimpleName();

    private AdView adView;

    final int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
    final int MP = LinearLayout.LayoutParams.MATCH_PARENT;

    private int   mScreenWidth;
    private int   mScreenHeight;
    private float mPixelDensity;

    private MyPagerAdapter mPagerAdapter;
    private ViewPager      mViewPager;
    private ImageView[]    mBalls;

    private ImageView mPrevArrow;
    private ImageView mNextArrow;

    private int mCurrentPos = 0;

    private int   guideNum = 13;
    private int[] pageId = {
        R.layout.guide00,   R.layout.guide01,   R.layout.guide02,
        R.layout.guide03,   R.layout.guide04,   R.layout.guide05,
        R.layout.guide06,   R.layout.guide07,   R.layout.guide08,
        R.layout.guide09,   R.layout.guide10,   R.layout.guide11,
        R.layout.guide12,
    };
    private int[] guideTextId = {
        R.id.guide_text_00, R.id.guide_text_01, R.id.guide_text_02,
        R.id.guide_text_03, R.id.guide_text_04, R.id.guide_text_05,
        R.id.guide_text_06, R.id.guide_text_07, R.id.guide_text_08,
        R.id.guide_text_09, R.id.guide_text_10, R.id.guide_text_11,
        R.id.guide_text_12,
    };
    private int[] mBallId = {
        R.id.ball_no00,     R.id.ball_no01,     R.id.ball_no02,
        R.id.ball_no03,     R.id.ball_no04,     R.id.ball_no05,
        R.id.ball_no06,     R.id.ball_no07,     R.id.ball_no08,
        R.id.ball_no09,     R.id.ball_no10,     R.id.ball_no11,
        R.id.ball_no12,
    };

    public UsageActivity(){
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
        mBalls = new ImageView[guideNum];
        for( int i=0;i<guideNum;i++ ) {
            mBalls[i] = (ImageView)findViewById(mBallId[i]);
        }

        mViewPager  = (ViewPager)findViewById(R.id.viewpager);
        mPagerAdapter = new MyPagerAdapter();
        mViewPager.setAdapter(mPagerAdapter);

        mPrevArrow = (ImageView)findViewById(R.id.arr_prev);
        mNextArrow = (ImageView)findViewById(R.id.arr_next);

        int btmSize = (int)(mScreenWidth/10f);
        LinearLayout bll = (LinearLayout)findViewById(R.id.guide_bottom_area);
        bll.setLayoutParams(new LinearLayout.LayoutParams(MP,btmSize));
        for( int i=0;i<guideNum;i++ ) {
            mBalls[i].setLayoutParams(new LinearLayout.LayoutParams(btmSize/2,btmSize/2));
        }
        int topSize    = (int)(mScreenWidth/6f);
        int topTxtSize = (int)(mScreenWidth/15f/mPixelDensity);
        TextView ttxt = (TextView)findViewById(R.id.guide_top_area);
        ttxt.setLayoutParams(new LinearLayout.LayoutParams(MP,topSize));
        ttxt.setTextSize(topTxtSize);

        mViewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageSelected(int position) {
                mCurrentPos = position;
                DevLog.d(TAG, "mCurrentPos : " + mCurrentPos );
                int btnTextSize = (int)( mScreenWidth  /  25f / mPixelDensity );
                ((TextView)findViewById(guideTextId[mCurrentPos])).setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
                ((TextView)findViewById(guideTextId[mCurrentPos])).setShadowLayer(1.5f, 1.5f, 1.5f, Color.DKGRAY);
                for( int i=0;i<mBalls.length;i++ ) {
                    if(i==mCurrentPos) mBalls[i].setImageResource(R.drawable.dball_enable);
                    else               mBalls[i].setImageResource(R.drawable.dball_disable);
                }
                mPrevArrow.setVisibility(View.VISIBLE);
                mNextArrow.setVisibility(View.VISIBLE);

                if( mCurrentPos==mBalls.length-1 ) {
                    Button exitBtn = (Button)findViewById(R.id.btn_exit_guidance);
                    exitBtn.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            UsageActivity.this.finish();
                            Intent mIntent = new Intent(UsageActivity.this, SampleDetectorActivity.class);
                            startActivity(mIntent);
                        }
                    });
                }
            }
            @Override
            public void onPageScrolled(int i, float f, int j) {
                mPrevArrow.setVisibility(View.INVISIBLE);
                mNextArrow.setVisibility(View.INVISIBLE);
            }
            @Override
            public void onPageScrollStateChanged(int i) {
                if(mCurrentPos!=0)          mPrevArrow.setVisibility(View.VISIBLE);
                if(mCurrentPos!=guideNum-1) mNextArrow.setVisibility(View.VISIBLE);
            }
        });
    }

    private void setButtonAction()
    {
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getScreenSize();
        setContentView(R.layout.usage);

        findAllViewById();
        setButtonAction();

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
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    private void setCurrentPosition(int position) {
        mCurrentPos = position;
        mViewPager.setCurrentItem(mCurrentPos);
        for( int i=0;i<mBalls.length;i++ ) {
            if(i==mCurrentPos) mBalls[i].setImageResource(R.drawable.dball_enable);
            else               mBalls[i].setImageResource(R.drawable.dball_disable);
        }
        if(mCurrentPos==0)          mPrevArrow.setVisibility(View.INVISIBLE);
        if(mCurrentPos==guideNum-1) mNextArrow.setVisibility(View.INVISIBLE);
    }

    private class MyPagerAdapter extends PagerAdapter {
        @Override
        public Object instantiateItem(ViewGroup container, int position) {
            DevLog.d(TAG, "instantiateItem() : " + position );
            LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            View view = (View)inflater.inflate(pageId[position], null);
            container.addView(view);
            int btnTextSize = (int)( mScreenWidth  /  25f / mPixelDensity );
            ((TextView)findViewById(guideTextId[position])).setTextSize(TypedValue.COMPLEX_UNIT_DIP,btnTextSize);
            ((TextView)findViewById(guideTextId[position])).setShadowLayer(1.5f, 1.5f, 1.5f, Color.DKGRAY);
            return view;
        }
        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            DevLog.d(TAG, "destroyItem() : " + position );
            ((ViewPager)container).removeView((View)object);
        }
        @Override
        public int getCount() {
            return mBalls.length;
        }
        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view.equals(object);
        }
    }
}
