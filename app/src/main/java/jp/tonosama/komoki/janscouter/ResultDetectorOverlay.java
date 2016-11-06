package jp.tonosama.komoki.janscouter;

import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.DevLog;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Paint.Style;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class ResultDetectorOverlay extends View {

    private static final String TAG = ResultDetectorOverlay.class.getSimpleName();

    private Context mContext;

    private String mFps = "0";
    private int    mDegree;
    private Bitmap mDecodeBitmap = null;   //
    private Bitmap mShutterNormalBitmap;   //
    private Bitmap mShutterPressedBitmap;  //
    private Bitmap mDebugBtnNormalBitmap;  //
    private Bitmap mDebugBtnPressedBitmap; //

    // =============================
    // デバッグ用３項目の設定
    // -----------------------------
    // その１：エッジ画像
    private int mBitmapPosX = 0;
    private int mBitmapPosY = 0;
    private int mBitmapWidth  = 0;
    private int mBitmapHeight = 0;
    // -----------------------------
    // その２：シャッターボタン
    private int mShutterPosX = 0;
    private int mShutterPosY = 0;
    private int mShutterWidth  = 0;
    private int mShutterHeight = 0;
    // -----------------------------
    // その３：デバッグボタン
    private int mDebugBtnPosX = 0;
    private int mDebugBtnPosY = 0;
    private int mDebugBtnWidth  = 0;
    private int mDebugBtnHeight = 0;
    // =============================

    // =============================
    // スカウターのビットマップ９個
    private Bitmap mScouterBitmapRO;
    private Bitmap mScouterBitmapHaft;
    private int mScouterSize = 5;

    public void releaseInstance(){
    	mContext = null;
    	mDecodeBitmap          = mShutterNormalBitmap   = mShutterPressedBitmap =
    	mDebugBtnNormalBitmap  = mDebugBtnPressedBitmap = null;
    }

    private HiInfo mHiInfo = null;

    private float width, height;
    private final int HANDLER_MESSAGE_ON_SHUTTER    = 1;
    private final int HANDLER_MESSAGE_ON_PREVIEW    = 2;
    private final int HANDLER_MESSAGE_DEBUG_PRESSED = 3;
    private final int HANDLER_MESSAGE_TOUCH_FOCUS   = 4;
    private final int HANDLER_MESSAGE_TOUCH_RESULT  = 5;

    private int guidePosX;
    private int guidePosY;
    private int guideWidth;
    private int guideHeight;

    //private int doraPosX;
    //private int doraPosY;
    //private int doraWidth;
    //private int doraHeight;

    private int tehaiPosX;
    private int tehaiPosY;
    private int tehaiWidth;
    private int tehaiHeight;

    private int nakiPosX;
    private int nakiPosY;
    private int nakiWidth;
    private int nakiHeight;

    private int focusX;
    private int focusY;
    private long focusStartTime = 0;

    private boolean isShutterPressed  = false;
    private boolean isDebugBtnPressed = false;
    private boolean isBitmapPressed   = false;
    private boolean isPreviewPressed  = false;

    private Paint boardPaint;      // ボタン等の背景
    private Paint bmpTouchPaint;   // ミニプレビュータッチ時
    private Paint guidePaint;      // ガイド枠
    private Paint detectPaint;     // 牌の短形
    private Paint focusPaint;      // フォーカス枠
    private Paint debugTextPaint;  // デバッグ用テキスト（FPS: X.X）
    private Paint detectTextPaint; // 検出結果のテキスト（Found XX hai!）
    private Paint miniGuidePaint;  // ドラ等のミニガイド枠
    private Paint guideTextPaint;  // TEHAI/DORA 等のテキスト

    private final float guideStrokeWidth = 4f;

    private Rect  mSttRect;
    private Rect  mEndRect;

    // Preferrence
    private boolean isOnlyAnalyze = false;
    private boolean isOnlyPreview = false;

    public ResultDetectorOverlay(Context context) {
        super(context);
        DevLog.i(TAG, "Instantiated new " + this.getClass());

        mContext = context;
        Resources r = getResources();
        mShutterNormalBitmap   = BitmapFactory.decodeResource(r, R.drawable.btn_camera_shutter_holo);
        mShutterPressedBitmap  = BitmapFactory.decodeResource(r, R.drawable.btn_camera_shutter_pressed_holo);
        mDebugBtnNormalBitmap  = BitmapFactory.decodeResource(r, R.drawable.debug_button_normal);
        mDebugBtnPressedBitmap = BitmapFactory.decodeResource(r, R.drawable.debug_button_pressed);

        // スカウターのビットマップ９個
        mScouterBitmapRO = BitmapFactory.decodeResource(r, R.drawable.scouter_round );
        mScouterBitmapHaft = BitmapFactory.decodeResource(r, R.drawable.scouter_haft );

        // シャッターボタン等の背景
        boardPaint = new Paint();
        boardPaint.setStyle(Paint.Style.FILL);
        boardPaint.setColor(Color.DKGRAY);
        //boardPaint.setAlpha(128);
        boardPaint.setAlpha(255);

        // ミニプレビュータッチ時の背景
        bmpTouchPaint = new Paint();
        bmpTouchPaint.setStyle(Paint.Style.FILL);
        bmpTouchPaint.setColor(Color.CYAN);
        bmpTouchPaint.setAlpha(128);

        // ガイド枠
        guidePaint = new Paint();
        guidePaint.setColor(Color.RED);
        guidePaint.setStyle(Style.STROKE);
        guidePaint.setStrokeWidth(guideStrokeWidth);

        // 検出時の枠
        detectPaint = new Paint();
        detectPaint.setColor(Color.RED);
        detectPaint.setStyle(Style.STROKE);
        detectPaint.setStrokeWidth(5f);

        // フォーカス枠
        focusPaint = new Paint();
        focusPaint.setColor(Color.WHITE);
        focusPaint.setStrokeWidth(2f);
        focusPaint.setStyle(Style.STROKE);

        // デバッグ用のテキスト
        debugTextPaint = new Paint();
        debugTextPaint.setAntiAlias(true);
        debugTextPaint.setColor(Color.WHITE);
        debugTextPaint.setAlpha(218);
        debugTextPaint.setShadowLayer(5, 1, 1, Color.BLACK);

        // 検出結果のテキスト
        detectTextPaint = new Paint();
        detectTextPaint.setAntiAlias(true);
        detectTextPaint.setColor(Color.WHITE);
        detectTextPaint.setAlpha(218);
        detectTextPaint.setShadowLayer(5, 1, 1, Color.BLACK);

        // ミニガイド枠
        miniGuidePaint = new Paint();
        miniGuidePaint.setColor(Color.WHITE);
        miniGuidePaint.setAlpha(218);
        miniGuidePaint.setStyle(Style.STROKE);
        miniGuidePaint.setStrokeWidth(guideStrokeWidth);

        // TEHAI/DORA 等のテキスト
        guideTextPaint = new Paint();
        guideTextPaint.setAntiAlias(true);
        //guideTextPaint.setColor(Color.rgb(255, 160, 0));
        guideTextPaint.setColor(Color.WHITE);
        guideTextPaint.setAlpha(218);
        guideTextPaint.setShadowLayer(5, 1, 1, Color.BLACK);

        mSttRect = new Rect();
        mEndRect = new Rect();

        getPreference();
    }
    public void getPreference(){
        isOnlyAnalyze = ((ResultDetectorActivity)mContext).getIsOnlyAnalyze();
        isOnlyPreview = ((ResultDetectorActivity)mContext).getIsOnlyPreview();
        mScouterSize  = ((ResultDetectorActivity)mContext).getScouterSize();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        width  = w;
        height = h;
    }

    public void setFps(String fps) {
        mFps = fps;
    }
    public void setAngle(int orientation){
    	mDegree = orientation;
    }
    public void setDecodeBitmap(Bitmap decode_bitmap){
        mDecodeBitmap = decode_bitmap;
    }
    public void setHiInfo(HiInfo hi_info){
        mHiInfo = hi_info;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        canvas.drawColor(Color.TRANSPARENT);

        Matrix matrix = new Matrix();
        float angle = ((ResultDetectorActivity)mContext).getOrientationMatrix(false);
        matrix.postRotate(angle);
        // ==================================================================
        // 文字サイズの定義 -> S
        debugTextPaint.setTextSize(height/32);
        detectTextPaint.setTextSize(height/12);
        guideTextPaint.setTextSize(height/24);
        // 文字サイズの定義 -> E
        // ==================================================================
        // ガイドの位置定義 -> S
        guideWidth  = (int)(width  - height/mScouterSize*1.06f);
        guideHeight = (int)(height - height/mScouterSize*1.12f);
        guidePosX   = (int)(width  / 2 - guideWidth *1/2);
        guidePosY   = (int)(height / 2 - guideHeight*1/2);
        // ガイドの位置定義 -> E
        // ==================================================================
        // ドラの位置定義 -> S
        //doraWidth  = (int)(guideWidth  * 0.95f);
        //doraHeight = (int)(guideHeight * 0.95f / 3f);
        //doraPosX   = (int)(width / 2f - doraWidth / 2f);
        //doraPosY   = (int)(guidePosY * 1.02f);
        // ドラの位置定義 -> E
        // ==================================================================
        // 手牌の位置定義 -> S
        tehaiWidth  = (int)(guideWidth  * 0.95f);
        tehaiHeight = (int)(guideHeight * 0.90f / 3f);
        tehaiPosX   = (int)(width / 2f - tehaiWidth / 2f);
        tehaiPosY   = (int)(height/2-tehaiHeight/2);
        // 手牌の位置定義 -> E
        // ==================================================================
        // 鳴き牌の位置定義 -> S
        nakiWidth  = (int)(guideWidth  * 0.95f);
        nakiHeight = (int)(guideHeight * 0.90f / 3f);
        nakiPosX   = (int)(width / 2f - nakiWidth / 2f);
        nakiPosY   = (int)(height - guidePosY * 1.2f - nakiHeight);
        // 鳴き牌の位置定義 -> E
        // ==================================================================
        // デバッグボタンの位置定義 -> S
        mDebugBtnWidth  = (int)(height/5/2*1.5f);
        mDebugBtnHeight = (int)(height/5/2*1.5f);
        mDebugBtnPosX   = (int)(width  - height/mScouterSize/1.2f - mDebugBtnWidth);
        mDebugBtnPosY   = (int)((guidePosY * 1.02f) + (guideHeight * 0.95f / 3f) / 2 - mDebugBtnHeight / 2);
        // デバッグボタンの位置定義 -> E
        // ==================================================================
        // シャッターボタンの位置定義 -> S
        mShutterWidth  = (int)(height/5/2*1.5f);
        mShutterHeight = (int)(height/5/2*1.5f);
        mShutterPosX   = (int)(width  - height/mScouterSize/1.2f - mShutterWidth);
        mShutterPosY   = (int)(height/2 - mShutterHeight/2);
        // シャッターボタンの位置定義 -> E
        // ==================================================================
        // DecodedBitmap の位置定義 -> S
        if( mDecodeBitmap!=null )
        {
            mBitmapWidth  = (int)(height/5/2*1.5f);
            mBitmapHeight = (int)(mBitmapWidth*mDecodeBitmap.getHeight()/mDecodeBitmap.getWidth()*1.5f);
            mBitmapPosX   = (int)(width  - height/mScouterSize/1.2f - mBitmapWidth);
            mBitmapPosY   = (int)(nakiPosY + nakiHeight / 2 - mBitmapHeight / 2);
        }
        // DecodedBitmap の位置定義 -> E
        // ==================================================================
        // フォーカス枠の表示 -> S
        if(focusStartTime + 1000 > System.currentTimeMillis() ){
            float focus_len = height / 6;
            canvas.drawRect(
                    focusX - focus_len*1/2,
                    focusY - focus_len*1/2,
                    focusX + focus_len*1/2,
                    focusY + focus_len*1/2, focusPaint );
        }
        // フォーカス枠の表示 -> E
        // ==================================================================
        // ==================================================================
        // 牌ガイドの表示 -> S
        float haiWidth  = tehaiWidth / 15f;
        float haiHeight = haiWidth * 1.5f ;
        //canvas.drawRect(
        //        doraPosX * 1.2f,
        //        doraPosY + (doraHeight-haiHeight)/1.5f,
        //        doraPosX * 1.2f + haiWidth,
        //        doraPosY + (doraHeight-haiHeight)/1.5f + haiHeight, miniGuidePaint );
        canvas.drawRect(
                tehaiPosX * 1.2f,
                tehaiPosY + (tehaiHeight-haiHeight)/1.5f,
                tehaiPosX * 1.2f + haiWidth,
                tehaiPosY + (tehaiHeight-haiHeight)/1.5f + haiHeight, miniGuidePaint );
        canvas.drawRect(
                nakiPosX * 1.2f,
                nakiPosY + (nakiHeight-haiHeight)/1.5f,
                nakiPosX * 1.2f + haiWidth,
                nakiPosY + (nakiHeight-haiHeight)/1.5f + haiHeight, miniGuidePaint );
        // 牌ガイドの表示 -> E
        // ==================================================================
        // ==================================================================
        // ドラ/手牌/鳴き牌検出用のガイド線 -> S
        float textHeight = guideTextPaint.getFontMetrics().ascent;
        float textWidth  = guideTextPaint.measureText("ドラ牌");
        // --------
        // ドラ
        //canvas.drawRect(
        //        doraPosX,
        //        doraPosY,
        //        doraPosX + doraWidth,
        //        doraPosY + doraHeight, miniGuidePaint );
        //canvas.drawText(
        //        "ドラ牌",
        //        doraPosX * 1.02f + 4,
        //        doraPosY + (doraHeight-haiHeight)/1.5f   - 8f, guideTextPaint);
        // --------
        // 手牌
        canvas.drawRect(
                tehaiPosX,
                tehaiPosY,
                tehaiPosX + tehaiWidth,
                tehaiPosY + tehaiHeight, miniGuidePaint );
        canvas.drawText(
                "手 牌",
                tehaiPosX * 1.02f + 4 + textWidth/10,
                tehaiPosY + (tehaiHeight-haiHeight)/1.5f - 8f,guideTextPaint);
        // --------
        // 鳴き牌
        canvas.drawRect(
                nakiPosX,
                nakiPosY,
                nakiPosX + nakiWidth,
                nakiPosY + nakiHeight, miniGuidePaint );
        canvas.drawText(
                "鳴き牌",
                nakiPosX * 1.02f + 4,
                nakiPosY + (nakiHeight-haiHeight)/1.5f   - 8f, guideTextPaint);
        // ドラ/手牌/鳴き牌検出用のガイド線 -> E
        // ==================================================================
        // ==================================================================
        // 牌検出用のガイド文言 -> S
        String guideStr     = "この枠内に牌をセットしてください";
        textWidth           = debugTextPaint.measureText(guideStr);
        textHeight          = debugTextPaint.getFontMetrics().ascent;
        float guideTextPosX = width  / 2 - textWidth / 2;
        float guideTextPosY = height / 2 + guideHeight * 0.95f *0.48f;
        if(isOnlyAnalyze) guideTextPosX -= mShutterWidth / 3;
        canvas.drawText(
            guideStr,
            guideTextPosX,
            guideTextPosY,
            debugTextPaint );
        // 牌検出用のガイド文言 -> E
        // ==================================================================
        // ==================================================================
        // 牌検出結果の表示 -> S
        if(mHiInfo!=null){
            textWidth  = detectTextPaint.measureText(mHiInfo.getDetectResultMsg());
            textHeight = detectTextPaint.getFontMetrics().ascent;
            float resultTextPosX = width  / 2 - textWidth  / 2;
            float resultTextPosY = height / 2 - textHeight / 2;
            if(isOnlyAnalyze) resultTextPosX -= mShutterWidth / 3;
            canvas.drawText(
                mHiInfo.getDetectResultMsg(),
                resultTextPosX,
                resultTextPosY,
                detectTextPaint );
        }
        // 牌検出結果の表示 -> E
        // ==================================================================
        if( isOnlyAnalyze ) {

            // ==================================================================
            // デバッグ用文字表示 -> S
            String text;
            text = "Detection: OFF";
            textWidth  = debugTextPaint.measureText(text);
            text = "FPS: " + mFps;
            canvas.drawText(
                    text,
                    width / 2 - textWidth / 2,
                    height/mScouterSize*0.6f + Math.abs(debugTextPaint.getFontMetrics().ascent)*1.2f,
                    debugTextPaint );
            if( isOnlyPreview ) {
                text = "Detection: OFF";
                canvas.drawText(
                    text,
                    width / 2 - textWidth / 2,
                    height/mScouterSize*0.6f + Math.abs(debugTextPaint.getFontMetrics().ascent)*2.4f,
                    debugTextPaint );
            }else{
                text = "Detection: ON";
                canvas.drawText(
                    text,
                    width / 2 - textWidth / 2,
                    height/mScouterSize*0.6f + Math.abs(debugTextPaint.getFontMetrics().ascent)*2.4f,
                    debugTextPaint );
            }
            text = "Degree: " + mDegree;
            canvas.drawText(
                    text,
                    width / 2 - textWidth / 2,
                    height/mScouterSize*0.6f + Math.abs(debugTextPaint.getFontMetrics().ascent)*3.6f,
                    debugTextPaint );
            // デバッグ用文字表示 -> E
            // ==================================================================
        }
        // ==================================================================
        // スカウター の表示 -> S
        // -- LeftTop --
        mSttRect.set(0,0,mScouterBitmapRO.getWidth()/2,mScouterBitmapRO.getHeight()/2);
        mEndRect.set(0,0,(int)(height/mScouterSize),(int)(height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- LeftCenter --
        mSttRect.set(0,mScouterBitmapRO.getHeight()/2-1,mScouterBitmapRO.getWidth()/2,mScouterBitmapRO.getHeight()/2+1);
        mEndRect.set(0,(int)(height/mScouterSize),(int)(height/mScouterSize),(int)(height-height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- LeftBottom --
        mSttRect.set(0,mScouterBitmapRO.getHeight()/2,mScouterBitmapRO.getWidth()/2,mScouterBitmapRO.getHeight());
        mEndRect.set(0,(int)(height-height/mScouterSize),(int)(height/mScouterSize),(int)(height));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- RightTop --
        mSttRect.set(mScouterBitmapRO.getWidth()/2,0,mScouterBitmapRO.getWidth(),mScouterBitmapRO.getHeight()/2);
        mEndRect.set((int)(width-height/mScouterSize),0,(int)(width),(int)(height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- RightCenter --
        mSttRect.set(mScouterBitmapRO.getWidth()/2,mScouterBitmapRO.getHeight()/2-1,mScouterBitmapRO.getWidth(),mScouterBitmapRO.getHeight()/2+1);
        mEndRect.set((int)(width-height/mScouterSize),(int)(height/mScouterSize),(int)(width),(int)(height-height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- RightBottom --
        mSttRect.set(mScouterBitmapRO.getWidth()/2,mScouterBitmapRO.getHeight()/2,mScouterBitmapRO.getWidth(),mScouterBitmapRO.getHeight());
        mEndRect.set((int)(width-height/mScouterSize),(int)(height-height/mScouterSize),(int)(width),(int)(height));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- TopCenter --
        mSttRect.set(mScouterBitmapRO.getWidth()/2-1,0,mScouterBitmapRO.getWidth()/2+1,mScouterBitmapRO.getHeight()/2);
        mEndRect.set((int)(height/mScouterSize),0,(int)(width-height/mScouterSize),(int)(height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- BottomCenter --
        mSttRect.set(mScouterBitmapRO.getWidth()/2-1,mScouterBitmapRO.getHeight()/2,mScouterBitmapRO.getWidth()/2+1,mScouterBitmapRO.getHeight());
        mEndRect.set((int)(height/mScouterSize),(int)(height-height/mScouterSize),(int)(width-height/mScouterSize),(int)(height));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- CenterCenter --
        mSttRect.set(mScouterBitmapRO.getWidth()/2-1,mScouterBitmapRO.getHeight()/2-1,mScouterBitmapRO.getWidth()/2+1,mScouterBitmapRO.getHeight()/2+1);
        mEndRect.set((int)(height/mScouterSize),(int)(height/mScouterSize),(int)(width-height/mScouterSize),(int)(height-height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapRO,mSttRect,mEndRect, null);
        // -- 柄の部分 --
        mSttRect.set(0,0,mScouterBitmapHaft.getWidth(),mScouterBitmapHaft.getHeight());
        mEndRect.set((int)(width-height/mScouterSize),(int)(height/mScouterSize),(int)(width),(int)(height-height/mScouterSize));
        canvas.drawBitmap(mScouterBitmapHaft,mSttRect,mEndRect, null);
        // スカウター の表示 -> E
        // ==================================================================

        if(isOnlyAnalyze) {
            // ==================================================================
            // Decoded Bitmap の表示 -> S
            if( mDecodeBitmap!=null )
            {
                Bitmap mMiniPreview = Bitmap.createBitmap(mDecodeBitmap,0,0, mDecodeBitmap.getWidth(), mDecodeBitmap.getHeight(), matrix, true);
                mSttRect.set(
                    0,0,mMiniPreview.getWidth(),mMiniPreview.getHeight() );
                mEndRect.set(
                    mBitmapPosX,
                    mBitmapPosY,
                    mBitmapPosX + mBitmapWidth,
                    mBitmapPosY + mBitmapHeight);
                canvas.drawBitmap(mMiniPreview,mSttRect,mEndRect, null);
                if( isBitmapPressed ){
                    isBitmapPressed = false;
                    canvas.drawRect(mEndRect, bmpTouchPaint);
                    handler.sendEmptyMessage(HANDLER_MESSAGE_ON_PREVIEW);
                }
            }
            // Decoded Bitmap の表示 -> E
            // ==================================================================
            // ==================================================================
            // シャッターボタン の表示 -> S
            Bitmap mShutterBitmap = ( isShutterPressed ) ?
                    Bitmap.createBitmap(mShutterPressedBitmap,0,0, mShutterPressedBitmap.getWidth(), mShutterPressedBitmap.getHeight(), matrix, true) :
                    Bitmap.createBitmap(mShutterNormalBitmap,0,0,  mShutterNormalBitmap.getWidth(),  mShutterNormalBitmap.getHeight(),  matrix, true) ;
                    mSttRect.set(
                        0,0,mShutterBitmap.getWidth(),mShutterBitmap.getHeight());
                    mEndRect.set(
                        mShutterPosX,
                        mShutterPosY,
                        mShutterPosX + mShutterWidth,
                        mShutterPosY + mShutterHeight );
                    canvas.drawBitmap(mShutterBitmap,mSttRect,mEndRect, null);
                    if( isShutterPressed ){
                        isShutterPressed = false;
                        handler.sendEmptyMessage(HANDLER_MESSAGE_ON_SHUTTER);
                    }
            // シャッターボタン の表示 -> E
            // ==================================================================
            // ==================================================================
            // デバッグ用ボタン の表示 -> S
            Bitmap mRotateBitmap = ( isDebugBtnPressed ) ?
                Bitmap.createBitmap(mDebugBtnPressedBitmap,0,0, mDebugBtnPressedBitmap.getWidth(), mDebugBtnPressedBitmap.getHeight(), matrix, true) :
                Bitmap.createBitmap(mDebugBtnNormalBitmap,0,0,  mDebugBtnNormalBitmap.getWidth(),  mDebugBtnNormalBitmap.getHeight(),  matrix, true) ;
                mSttRect.set(
                    0,0,mRotateBitmap.getWidth(),mRotateBitmap.getHeight());
                mEndRect.set(
                    mDebugBtnPosX,
                    mDebugBtnPosY,
                    mDebugBtnPosX + mDebugBtnWidth,
                    mDebugBtnPosY + mDebugBtnHeight );
                canvas.drawBitmap(mRotateBitmap,mSttRect,mEndRect, null);
                if( isDebugBtnPressed ){
                    isDebugBtnPressed  = false;
                    handler.sendEmptyMessage(HANDLER_MESSAGE_DEBUG_PRESSED);
                }
            // デバッグ用ボタン の表示 -> E
            // ==================================================================
        }

        if(isOnlyPreview) return;

        // ==================================================================
        // 牌検出に成功 -> S
        if(mHiInfo!=null && mHiInfo.getDetectResult() >= 14)
        {
            detectPaint.setColor(Color.YELLOW);
        }
        // 牌検出に成功 -> E
        // ==================================================================
        // ==================================================================
        // 牌の短形表示 -> S
        if( mHiInfo!=null && (mHiInfo.getDetectResult() > 0) )
        {
            for(int i=0;i<mHiInfo.getDetectResult();i++)
            {
                float pos_x = guidePosX + mHiInfo.getHiPoint().get(i).x;
                float pos_y = guidePosY + mHiInfo.getHiPoint().get(i).y;
                float hi_width  = mHiInfo.getHiSize().get(i).x;
                float hi_height = mHiInfo.getHiSize().get(i).y;
                canvas.drawRect(
                        pos_x ,
                        pos_y ,
                        pos_x + hi_width,
                        pos_y + hi_height, detectPaint );
            }
        }
        // 牌の短形表示 -> E
        // ==================================================================
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        int posX = (int)event.getX();
        int posY = (int)event.getY();

        int sttBitmapX = 0,sttBitmapY = 0,endBitmapX = 0,endBitmapY = 0;

        switch (event.getAction()) {
        case MotionEvent.ACTION_DOWN:

            if( mDecodeBitmap!=null ){
                sttBitmapX = mBitmapPosX;
                endBitmapX = mBitmapPosX + mBitmapWidth;
                sttBitmapY = mBitmapPosY;
                endBitmapY = mBitmapPosY + mBitmapHeight;
            }

            if( mShutterPosX <= posX && posX <= mShutterPosX + mShutterWidth && mShutterPosY <= posY && posY <= mShutterPosY + mShutterHeight ){
                isShutterPressed = true;
                this.invalidate();
            }
            else if( mDebugBtnPosX <= posX && posX <= mDebugBtnPosX + mDebugBtnWidth && mDebugBtnPosY <= posY && posY <= mDebugBtnPosY + mDebugBtnHeight ){
                isDebugBtnPressed = true;
                this.invalidate();
            }
            else if( sttBitmapX <= posX && posX <= endBitmapX && sttBitmapY <= posY && posY <= endBitmapY ){
                isBitmapPressed = true;
                this.invalidate();
            }else{
                isPreviewPressed = true;
                this.invalidate();
            }

            break;

        case MotionEvent.ACTION_UP:
            if( isDebugBtnPressed ){
            }
            if( isShutterPressed ){
            }
            if( isBitmapPressed ){
            }
            if( isPreviewPressed ){
                handler.sendEmptyMessage(HANDLER_MESSAGE_TOUCH_FOCUS);
                focusStartTime = System.currentTimeMillis();
                focusX = posX;
                focusY = posY;
                isPreviewPressed = false;
            }
            break;
        }
        return true;
    }

    private final Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            switch(msg.what){
            case HANDLER_MESSAGE_ON_SHUTTER:
                ((ResultDetectorActivity)mContext).shutterPressed(false);
                break;
            case HANDLER_MESSAGE_TOUCH_RESULT:
                ((ResultDetectorActivity)mContext).previewResult(mHiInfo);
                break;
            case HANDLER_MESSAGE_ON_PREVIEW:
                ((ResultDetectorActivity)mContext).previewBitmap();
                break;
            case HANDLER_MESSAGE_DEBUG_PRESSED:
                ((ResultDetectorActivity)mContext).setDebugBtnPressed();
                break;
            case HANDLER_MESSAGE_TOUCH_FOCUS:
                ((ResultDetectorActivity)mContext).touchFocus();
                break;
            }
        }
    };

    // ========================================================================
    // Draw Object の情報取得
    public Bitmap getDecodedBitmap(){
        return mDecodeBitmap;
    }
    public Point[] getGuidePointInfo()
    {
        Point guidePos  = new Point(guidePosX,guidePosY);
        Point guideSize = new Point(guideWidth,guideHeight);
        Point[] guideInfo = {guidePos,guideSize};
        return guideInfo;
    }

    public void release() {
        if(mDecodeBitmap!=null)          mDecodeBitmap.recycle();
        if(mShutterNormalBitmap!=null)   mShutterNormalBitmap.recycle();
        if(mShutterPressedBitmap!=null)  mShutterPressedBitmap.recycle();
        if(mDebugBtnNormalBitmap!=null)  mDebugBtnNormalBitmap.recycle();
        if(mDebugBtnPressedBitmap!=null) mDebugBtnPressedBitmap.recycle();

        if(mScouterBitmapRO!=null) mScouterBitmapRO.recycle();
        if(mScouterBitmapHaft!=null) mScouterBitmapHaft.recycle();
        mHiInfo = null;
    }
}