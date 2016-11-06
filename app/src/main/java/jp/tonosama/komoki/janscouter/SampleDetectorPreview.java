package jp.tonosama.komoki.janscouter;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Matrix;
import android.graphics.Point;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;

import java.io.IOException;
import java.util.List;

import jp.tonosama.komoki.janscouterUtil.JanScouterUtil;
import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.DevLog;

public class SampleDetectorPreview extends ViewGroup implements SurfaceHolder.Callback, Runnable {

    private static final String TAG = SampleDetectorPreview.class.getSimpleName();

    private SurfaceView mSurfaceView;
    private SurfaceHolder mHolder;
    public  Size mPreviewSize;
    private List<Size> mSupportedPreviewSizes;
    public  Camera mCamera;
    private Context mContext;
    private byte[]   mBuffer;
    private byte[]   mFrame;
    private boolean  mThreadRun;

    private Bitmap bmp;
    private String fps = "";
    private byte[] mYUV   = null;
    private int[] edgeImg = null;

    // for JanDetector
    private HiInfo  mHiInfo    = null;
    private Point[] mGuideInfo = null;
    private byte[]  mHiArray   = null;
    private int[]   mHiPointX  = null;
    private int[]   mHiPointY  = null;
    private int[]   mHiWidth   = null;
    private int[]   mHiHeight  = null;

    private final int HANDLER_MESSAGE_INVALIDATE_OVERLAY = 1;
    private final int HANDLER_MESSAGE_DETECTED_OVER_34   = 2;

    private boolean isStopProcessFrame = false;
    public void restartProcessFrame(){
        isStopProcessFrame = false;
    }
    private boolean isDebugBtnPressed  = false;
    public void setDebugBtnPressed(){
    	isDebugBtnPressed  = true;
    }

    // Preferrence
    public boolean isOnlyAnalyze = false;
    public boolean isOnlyPreview = false;

    public SampleDetectorPreview(Context context) {
        super(context);
        DevLog.i(TAG, "Instantiated new " + this.getClass());

        mContext = context;
        mSurfaceView = new SurfaceView(context);
        addView(mSurfaceView);

        mHolder = mSurfaceView.getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        mHiInfo   = new HiInfo();
        mHiPointX = new int[HiInfo.SCAN_HI_NUM];
        mHiPointY = new int[HiInfo.SCAN_HI_NUM];
        mHiWidth  = new int[HiInfo.SCAN_HI_NUM];
        mHiHeight = new int[HiInfo.SCAN_HI_NUM];
        mGuideInfo = new Point[2];
        mHiArray  = new byte[64*96*34];
    }
    public void getPreference(){
        isOnlyAnalyze = ((SampleDetectorActivity)mContext).getIsOnlyAnalyze();
        isOnlyPreview = ((SampleDetectorActivity)mContext).getIsOnlyPreview();
    }
    public void setCamera(Camera camera) {
        mCamera = camera;
        if (mCamera != null) {
            mSupportedPreviewSizes = mCamera.getParameters().getSupportedPreviewSizes();
            requestLayout();

            mCamera.setPreviewCallbackWithBuffer(new PreviewCallback() {
                public void onPreviewFrame(byte[] data, Camera camera) {
                    synchronized (SampleDetectorPreview.this) {
                        System.arraycopy(data, 0, mFrame, 0, data.length);
                        SampleDetectorPreview.this.notify();
                    }
                    camera.addCallbackBuffer(mBuffer);
                }
            });
        }
    }

    public void switchCamera(Camera camera) {
        setCamera(camera);
        try {
            camera.setPreviewDisplay(mHolder);
        } catch (IOException exception) {
            DevLog.e(TAG, "IOException caused by setPreviewDisplay()", exception);
        }
        Camera.Parameters parameters = camera.getParameters();
        parameters.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
        requestLayout();

        camera.setParameters(parameters);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        final int width  = resolveSize(getSuggestedMinimumWidth(),  widthMeasureSpec);
        final int height = resolveSize(getSuggestedMinimumHeight(), heightMeasureSpec);
        setMeasuredDimension(width, height);

        if (mSupportedPreviewSizes != null) {
            mPreviewSize = getOptimalPreviewSize(mSupportedPreviewSizes, width, height);
        }
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if (changed && getChildCount() > 0) {
            final View child = getChildAt(0);

            int width = r - l;
            int height = b - t;

            int previewWidth = width;
            int previewHeight = height;
            if (mPreviewSize != null) {
                previewWidth = mPreviewSize.width;
                previewHeight = mPreviewSize.height;
                // kozawa add s
                width  = mPreviewSize.width;
                height = mPreviewSize.height;
                // kozawa add e
            }

            if (width * previewHeight > height * previewWidth) {
                final int scaledChildWidth = previewWidth * height / previewHeight;
                child.layout((width - scaledChildWidth) / 2, 0,
                        (width + scaledChildWidth) / 2, height);
            } else {
                final int scaledChildHeight = previewHeight * width / previewWidth;
                child.layout(0, (height - scaledChildHeight) / 2,
                        width, (height + scaledChildHeight) / 2);
            }

            DevLog.d(TAG, "previewWidth = " + previewWidth + ", previewHeight = " + previewHeight);
        }
    }

    public void surfaceCreated(SurfaceHolder holder)
    {
        try {
            if (mCamera != null) {
                mCamera.setPreviewDisplay(holder);

                (new Thread(SampleDetectorPreview.this)).start();

            }
        } catch (IOException exception) {
            DevLog.e(TAG, "IOException caused by setPreviewDisplay()", exception);
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder)
    {
        if (mCamera != null) {
            mCamera.stopPreview();
        }
    }

    public void run() {
        mThreadRun = true;
        while (mThreadRun) {
            synchronized (this) {
                try {
                    this.wait();
                    processFrame(mFrame);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.1;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Try to find an size match aspect ratio and size
        for (Size size : sizes) {
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        // Cannot find the one match the aspect ratio, ignore the requirement
        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
        /*
        List<String> FocusModes = parameters.getSupportedFocusModes();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD){
            if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_INFINITY)){
                parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_INFINITY);
            }
        }*/
        requestLayout();

        /* Now allocate the buffer */
        int size = parameters.getPreviewSize().width * parameters.getPreviewSize().height;
        size  = size * ImageFormat.getBitsPerPixel(parameters.getPreviewFormat()) / 8;
        mBuffer = new byte[size];
        /* The buffer where the current frame will be copied */
        mFrame = new byte [size];
        mCamera.addCallbackBuffer(mBuffer);

        mCamera.setParameters(parameters);
        mCamera.startPreview();
        new Thread() {
            @Override
            public void run() {
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                autoFocus();
            }
        }.start();
    }

    private long previousTimeMillis = 0;
    public String getFpf(long currentTimeMillis)
    {
        String fps_str = "0";
        if( previousTimeMillis == 0 ){
            previousTimeMillis = currentTimeMillis;
            return fps_str;
        }
        float fps = (1000f / (currentTimeMillis - previousTimeMillis));
        fps_str = String.format("%1$3.1f",fps);
        previousTimeMillis = currentTimeMillis;

        return fps_str;
    }

    // オートフォーカス
    public void autoFocus(){
        if( mCamera != null ){
            mCamera.autoFocus( new Camera.AutoFocusCallback() {
                @Override
                public void onAutoFocus(boolean success, Camera camera) {
                }
            });
        }
    }
    // 写真撮影（フォーカスあり）
    public void takePicture(){
        if( mCamera != null ){
            mCamera.autoFocus( new Camera.AutoFocusCallback() {
                @Override
                public void onAutoFocus(boolean success, Camera camera) {
                    new Thread(new Runnable(){
                        public void run() {
                            try{
                                Thread.sleep(3000);
                            }catch(Exception e){}
                        };
                    }).start();
                    mCamera.stopPreview();
                    ((SampleDetectorActivity)mContext).saveImage();
                    mCamera.startPreview();
                }
            });
        }
    }
    // 写真撮影（フォーカスなし）
    public void takePictureWithoutFocus(){
        if( mCamera != null ){
            mCamera.stopPreview();
            ((SampleDetectorActivity)mContext).saveImage();
            mCamera.startPreview();
        }
    }
    public byte[] getCurrentYUV(){
        return mYUV;
    }

    private void processFrame(final byte[] data)
    {
        if( data == null || mPreviewSize==null || isStopProcessFrame) return;

        mYUV = data;

        // ==================================================================
        // FPS の更新と描画更新要求 -> S
        fps = getFpf(System.currentTimeMillis());
        ((SampleDetectorActivity)mContext).getPreviewOverlay().setFps( fps );
        handler.sendEmptyMessage(HANDLER_MESSAGE_INVALIDATE_OVERLAY);
        // FPS の更新と描画更新要求 -> E
        // ==================================================================

        if(isOnlyPreview) return;

        // ==================================================================
        // ネイティブの処理 -> S
        mGuideInfo = ((SampleDetectorActivity)mContext).getPreviewOverlay().getGuidePointInfo();
        int imgWidth    = mPreviewSize.width;
        int imgHeight   = mPreviewSize.height;
        int guidePosX   = mGuideInfo[0].x;
        int guidePosY   = mGuideInfo[0].y;
        int guideWidth  = mGuideInfo[1].x;
        int guideHeight = mGuideInfo[1].y;
        if( mHiInfo==null ) mHiInfo = new HiInfo();
        if( edgeImg==null ) edgeImg = new int[ guideWidth * guideHeight ];
        DevLog.d(TAG,"guideWidth: " + guideWidth + ", guideHeight: " + guideHeight );

        int result = JanScouterUtil.JanSampleDetector(
            data, imgWidth, imgHeight,
            guidePosX,  guidePosY,
            guideWidth, guideHeight,
            mHiPointX,  mHiPointY,
            mHiWidth,   mHiHeight,
            mHiArray,   edgeImg,
            isDebugBtnPressed );

        if( isDebugBtnPressed ) isDebugBtnPressed = false;
        DevLog.d(TAG, "JanDetector result : " + result);
        if(edgeImg!=null) mHiInfo.setHiInfo(result, mHiPointX, mHiPointY, mHiWidth, mHiHeight,edgeImg.clone());
        // 34 以上の牌が見つかったら牌画像をセット
        // 34 以上の牌が検出されたら自動的に結果を表示する
        if( result >= 34 )
        {
            mCamera.stopPreview();
            mHiInfo.setHiArray(mHiArray);
            handler.sendEmptyMessage(HANDLER_MESSAGE_DETECTED_OVER_34);
            isStopProcessFrame = true;
            return;
        }
        if( ((SampleDetectorActivity)mContext).getPreviewOverlay() != null )
            ((SampleDetectorActivity)mContext).getPreviewOverlay().setHiInfo(mHiInfo);
        // ネイティブの処理 -> E
        // ==================================================================

        // ==================================================================
        // 縦画面用、横画面用に Bitmap を用意 -> S
        if( edgeImg != null ) {
            bmp = Bitmap.createBitmap(edgeImg, guideWidth, guideHeight, Config.RGB_565);
            Matrix matrix = new Matrix();
            float angle = ((SampleDetectorActivity)mContext).getOrientationMatrix(true);
            matrix.postRotate(angle);
            bmp = Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight(), matrix, true);
            if( ((SampleDetectorActivity)mContext).getPreviewOverlay() != null )
                ((SampleDetectorActivity)mContext).getPreviewOverlay().setDecodeBitmap(bmp);
        }
        // 縦画面用、横画面用に Bitmap を用意 -> E
        // ==================================================================
    }

    // オーバーレイの描画更新
    private final Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            switch(msg.what){
            case HANDLER_MESSAGE_INVALIDATE_OVERLAY:
                ((SampleDetectorActivity)mContext).invalidateOverlay();
                break;
            case HANDLER_MESSAGE_DETECTED_OVER_34:
                ((SampleDetectorActivity)mContext).invalidateOverlay();
                ((SampleDetectorActivity)mContext).previewResult(mHiInfo);
                break;
            }
        }
    };

    public void waitPreview(long time){
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void release() {
        if(bmp!=null) bmp.recycle();
        edgeImg   = null;
        mYUV      = null;
        mHiArray  = null;
        mBuffer   = null;
        mFrame    = null;
        mHiInfo   = null;
    }
}