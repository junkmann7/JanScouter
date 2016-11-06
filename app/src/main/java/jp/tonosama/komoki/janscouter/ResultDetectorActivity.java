package jp.tonosama.komoki.janscouter;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.OrientationEventListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import jp.tonosama.komoki.janscouterUtil.JanScouterPreference;
import jp.tonosama.komoki.janscouterUtil.JanScouterUtil;
import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.DevLog;

public class ResultDetectorActivity extends Activity {

    private static final String TAG = ResultDetectorActivity.class.getSimpleName();

    private final int MAIN_LAYOUT_ID = 12345;

    public static final String CONDITION_KEY = "CONDITION_KEY";

    private Camera mCamera;
    private ResultDetectorPreview mPreview;
    private ResultDetectorOverlay mPreviewOverlay;
    int numberOfCameras;
    int cameraCurrentlyLocked;
    Uri imgUri = null;

    private byte[] mSampleImg;

    OrientationEventListener mOrientationListener;

    public static final int ORIENTATION_VERTICAL   = 1;
    public static final int ORIENTATION_RIGHT_SIDE = 2;
    public static final int ORIENTATION_BACK_SIDE  = 3;
    public static final int ORIENTATION_LEFT_SIDE  = 4;
    private int orientation = ORIENTATION_LEFT_SIDE;

    // Preferrence
    public boolean isEnableSound = false;
    public boolean isOnlyAnalyze = false;
    public boolean isOnlyPreview = false;

    public boolean[] condition;

    // The first rear facing camera
    int defaultCameraId;

    ProgressDialog progressDialog1;
    ProgressDialog progressDialog2;

    MediaPlayer mp = null;

    public ResultDetectorActivity(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }
    public void getPreference(){
        isEnableSound = getIsEnableSound();
        isOnlyAnalyze = getIsOnlyAnalyze();
        isOnlyPreview = getIsOnlyPreview();
        mPreview.getPreference();
        mPreviewOverlay.getPreference();
    }
    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        // Hide the window title.
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // get condition
        Bundle bundle = getIntent().getExtras();
        if( bundle!=null ){
            condition = bundle.getBooleanArray(CONDITION_KEY);
        }else{
            finish();
        }

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

        // ============================================================================================
        // サンプルデータが無かった場合の処理 -> S
        if( mSampleImg == null ) {
            Intent mIntent = new Intent(ResultDetectorActivity.this,UsageActivity.class);
            startActivity(mIntent);
            finish();
            Toast.makeText(getApplicationContext(), "サンプルデータが見つかりませんでした。\n操作ガイドを確認してください。", Toast.LENGTH_LONG).show();
        }
        // サンプルデータが無かった場合の処理 -> E
        // ============================================================================================

        mPreview = new ResultDetectorPreview(this);
        mPreview.setId(MAIN_LAYOUT_ID);
        setContentView(mPreview);
        mPreviewOverlay = new ResultDetectorOverlay(this);
        addContentView( mPreviewOverlay, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
        getPreference();
        numberOfCameras = 1;

        mOrientationListener = new OrientationEventListener(this, SensorManager.SENSOR_DELAY_UI) {
            @Override
            public void onOrientationChanged(int orientation) {
                //Toast.makeText(getApplicationContext(), "Orientation changed : " + orientation + " !", Toast.LENGTH_SHORT).show();
                if(mPreviewOverlay!=null) mPreviewOverlay.setAngle(orientation);
                changeRotate(orientation);
            }
        };
        mOrientationListener.enable();

        // タップするとピントを合わせることができることを示すトースト
        if( mSampleImg != null ) {
            Toast.makeText(this, getResources().getString(R.string.detection_guide), Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        mCamera = Camera.open();
        cameraCurrentlyLocked = defaultCameraId;
        mPreview.setCamera(mCamera);
        getPreference();
    }

    @Override
    protected void onPause() {
        super.onPause();

        // Because the Camera object is a shared resource, it's very
        // important to release it when the activity is paused.
        if (mCamera != null) {
            mPreview.setCamera(null);
            mCamera.release();
            mCamera = null;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if( mPreview != null ) {
            mPreview.release();
            mPreview = null;
        }
        if( mPreviewOverlay != null ) {
            mPreviewOverlay.release();
            mPreviewOverlay = null;
        }
        mSampleImg = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.camera_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.launch_setting:
            Intent mIntent = new Intent(getApplicationContext(),JanScouterPreference.class);
            startActivity(mIntent);
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }

    public ResultDetectorOverlay getPreviewOverlay(){
        return mPreviewOverlay;
    }
    public ResultDetectorPreview getCameraPreview(){
        return mPreview;
    }

    byte[]    sampleImg;
    byte[]    tehai;
    byte[]    dora;
    byte[]    nakihai;
    boolean[] nakiflag;
    int[]     haiNum;
    byte[]    tehaiImg;
    byte[]    doraImg;
    byte[]    nakiImg;
    int[]     edgeImg;
    int[]     result;
    boolean[] yakulist;

    public void setMatchingDataSet(
            byte[]    sampleImg, // サンプル用の牌画像。入力用。
            byte[]    tehai,     // 解析した手牌の情報。出力用。31432239・・・ みたいな感じで値が入ってて、それぞれ「ピンズの１」とか「ソーズの５」とかを意味してる。
            byte[]    dora,      // ドラの情報。出力用。
            byte[]    nakihai,   // 解析した鳴き牌の情報。出力用。「東東東」とか「ピンズ345」とかの値が入る。
            boolean[] nakiflag,  // 鳴き牌フラグ。
            int[]     haiNum,    // 手牌、鳴き牌、ドラ牌の数が入る。入力用。
            byte[]    tehaiImg,  // 手牌の画像。  入力用。
            byte[]    doraImg,   // ドラ牌の画像。入力用。
            byte[]    nakiImg,   // 鳴き牌の画像。入力用。
            int[]     edgeImg    // 解析画像。デバッグ用。
            )
    {
        this.sampleImg = sampleImg;
        this.tehai     = tehai;
        this.dora      = dora;
        this.nakihai   = nakihai;
        this.nakiflag  = nakiflag;
        this.haiNum    = haiNum;
        this.tehaiImg  = tehaiImg;
        this.doraImg   = doraImg;
        this.nakiImg   = nakiImg;
        this.edgeImg   = edgeImg;
    }

    Handler  mHandler  = new Handler();

    // ============================================================================================
    // 牌が検出された後の処理
    // マッチングと音の再生を行う
    public void previewResult(HiInfo hi_info)
    {
        Log.d(TAG, "previewResult() start.");
        mPreview.mCamera.stopPreview();

        progressDialog1 = new ProgressDialog(this);
        //progressDialog1.setTitle("測定中");
        progressDialog1.setMessage("測定中 ... ");
        progressDialog1.setIndeterminate(false);
        progressDialog1.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        progressDialog1.show();

        if( isEnableSound ) {
            if( mp!=null ) {
                try {
                    mp.prepare();
                } catch (IllegalStateException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            mp = MediaPlayer.create(this, R.raw.repeat_scouter);
            mp.setLooping(true);
            mp.start();
        }

        new Thread(new Runnable() {
        public void run() {

            int matching_result = JanScouterUtil.JanResultMatching(
                    sampleImg,
                    tehai, dora, nakihai, nakiflag,
                    haiNum,
                    tehaiImg, doraImg, nakiImg,
                    edgeImg );

            result    = new int[3];      // 譜、翻、点の結果
            yakulist  = new boolean[60]; // あがり役のリスト
            for( int i=0;i<60;i++ ) yakulist[i] = false; // とりあえず全部 false にしとく -> 役無し
            haiNum[3] = 0; // とりあえず 0 本場としておく

            mHandler.post(new Runnable() {
                public void run(){

                    progressDialog1.dismiss();
                    if( isEnableSound ) {
                        mp.stop();
                        try {
                            mp.prepare();
                        } catch (IllegalStateException e) {
                            e.printStackTrace();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        mp = MediaPlayer.create(getApplicationContext(), R.raw.end);
                        mp.start();
                    }

                    //Intent mIntent = new Intent(getApplicationContext(), PreResultViewerActivity.class);
                    Intent mIntent = new Intent(getApplicationContext(),DetectedResultViewerActivity.class);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_CONDITION, condition);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_HAINUM,    haiNum);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_TEHAI,     tehai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_DORA,      dora);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKI,      nakihai);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_NAKIFLAG,  nakiflag);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_RESULT,    result);
                    mIntent.putExtra(DetectedResultViewerActivity.KEY_RESULT_VIEWER_YAKULIST,  yakulist);
                    mPreviewOverlay.releaseInstance();
                    finish();
                    startActivity(mIntent);
                }
            });
        }
        }).start();
    }

    // ==============================================================================
    // if result pressed (JanScouterDetector)
    public void agariResultPressed(HiInfo hi_info, int guide_width, int guide_height)
    {

    }
    // ==============================================================================
    // if bitmap pressed
    public void previewBitmap()
    {
        mPreview.mCamera.stopPreview();

        if(!isOnlyPreview){
            Bitmap bmp = mPreviewOverlay.getDecodedBitmap();
            long dateTaken   = System.currentTimeMillis();
            String name      = DateFormat.format("yyyy-MM-dd_kk.mm.ss", dateTaken).toString() + ".jpg";
            String filename  = name;
            String directory = Environment.getExternalStorageDirectory().toString() + "/" + "JanScouter";
            String filePath  = directory + "/" + filename;
            Log.d(TAG, "filepath : " + filePath);
            OutputStream outputStream = null;

            try {
                File dir = new File(directory);
                if(!dir.exists()){
                    dir.mkdirs();
                    DevLog.d(TAG, dir.toString() + " create");
                }
                File file = new File(directory, filename);
                if (file.createNewFile()) {
                    outputStream = new FileOutputStream(file);
                    if (bmp != null) {
                        bmp.compress(CompressFormat.JPEG, 100, outputStream);
                    }
                }
            } catch (FileNotFoundException ex) {
                Toast.makeText(this, "FileNotFoundException", Toast.LENGTH_SHORT).show();
                Log.w(TAG, ex);
                return;
            } catch (IOException ex) {
                Toast.makeText(this, "Please insert SD card.", Toast.LENGTH_SHORT).show();
                Log.w(TAG, ex);
                return;
            } finally {
                if (outputStream != null) {
                    try {
                        outputStream.close();
                    } catch (Throwable t) {
                    }
                }
            }

            ContentValues values = new ContentValues(7);
            values.put(Images.Media.TITLE, name);
            values.put(Images.Media.DISPLAY_NAME, filename);
            values.put(Images.Media.DATE_TAKEN, dateTaken);
            values.put(Images.Media.MIME_TYPE, "image/png");
            values.put(Images.Media.DATA, filePath);
            ContentResolver cr = getContentResolver();
            imgUri = cr.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
        }

        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_VIEW);
        intent.setType("image/jpg");
        intent.setData(imgUri);
        startActivity(intent);
    }

    // ==============================================================================
    // if preview touched
    public void touchFocus()
    {
        mPreview.autoFocus();
    }
    // ==============================================================================
    // if shutter pressed
    public void shutterPressed(boolean focusable)
    {
        if(focusable) mPreview.takePicture();
        else          mPreview.takePictureWithoutFocus();
    }

    public void saveImage()
    {
        int width    = mPreview.mPreviewSize.width;
        int height   = mPreview.mPreviewSize.height;
        byte[] mYUV  = mPreview.getCurrentYUV();
        int[] arrRGB = new int[ width * height ];
        JanScouterUtil.decodeYUV2RGB(mYUV, width, height, arrRGB);
        //JanScouterUtil.decodeYUV2GRAY(mYUV, width, height, arrRGB);
        //JanScouterUtil.decodeYUV2GRAY(mYUV, width, height, arrRGB);
        Bitmap bmp = Bitmap.createBitmap(arrRGB, width, height, Config.ARGB_8888);
        Matrix matrix = new Matrix();
        float angle = getOrientationMatrix(true);
        matrix.postRotate(angle);
        bmp = Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight(), matrix, true);

        long dateTaken   = System.currentTimeMillis();
        String name      = DateFormat.format("yyyy-MM-dd_kk.mm.ss", dateTaken).toString() + ".jpg";
        String filename  = name;
        String directory = Environment.getExternalStorageDirectory().toString() + "/" + "JanScouter";
        String filePath  = directory + "/" + filename;
        Log.d(TAG, "filepath : " + filePath);
        OutputStream outputStream = null;

        try {
            File dir = new File(directory);
            if(!dir.exists()){
                dir.mkdirs();
                DevLog.d(TAG, dir.toString() + " create");
            }
            File file = new File(directory, filename);
            if (file.createNewFile()) {
                outputStream = new FileOutputStream(file);
                if (bmp != null) {
                    bmp.compress(CompressFormat.JPEG, 100, outputStream);
                }
            }
        } catch (FileNotFoundException ex) {
            Toast.makeText(this, "FileNotFoundException", Toast.LENGTH_SHORT).show();
            Log.w(TAG, ex);
            return;
        } catch (IOException ex) {
            Toast.makeText(this, "Please insert SD card.", Toast.LENGTH_SHORT).show();
            Log.w(TAG, ex);
            return;
        } finally {
            if (outputStream != null) {
                try {
                    outputStream.close();
                } catch (Throwable t) {
                }
            }
        }

        ContentValues values = new ContentValues(7);
        values.put(Images.Media.TITLE, name);
        values.put(Images.Media.DISPLAY_NAME, filename);
        values.put(Images.Media.DATE_TAKEN, dateTaken);
        values.put(Images.Media.MIME_TYPE, "image/png");
        values.put(Images.Media.DATA, filePath);
        ContentResolver cr = getContentResolver();
        imgUri = cr.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

        mPreviewOverlay.setDecodeBitmap(bmp);
        mPreviewOverlay.invalidate();
    }

    // ==============================================================================
    // if Debug Button Pressed
    public void setDebugBtnPressed(){
        mPreview.setDebugBtnPressed();
    }

    // ==============================================================================
    // invalidate Overlay
    public void invalidateOverlay(){
        mPreviewOverlay.invalidate();
    }
    // ==============================================================================
    // if rotate pressed
    public void changeRotate(int degree){
        if(   0 <= degree && degree <  45 )
            orientation = ORIENTATION_VERTICAL;
        if(  45 <= degree && degree < 135 )
            orientation = ORIENTATION_RIGHT_SIDE;
        if( 135 <= degree && degree < 225 )
            orientation = ORIENTATION_BACK_SIDE;
        if( 225 <= degree && degree < 315 )
            orientation = ORIENTATION_LEFT_SIDE;
        if( 315 <= degree && degree < 360 )
            orientation = ORIENTATION_VERTICAL;
    }
    public float getOrientationMatrix(boolean is_reverse){
        float angle = 0f;
        switch(orientation){
        case ResultDetectorActivity.ORIENTATION_VERTICAL:
            if(is_reverse) angle =  90f;
            else           angle = -90f;
            break;
        case ResultDetectorActivity.ORIENTATION_RIGHT_SIDE:
            angle = 180f;
            break;
        case ResultDetectorActivity.ORIENTATION_BACK_SIDE:
            if(is_reverse) angle = -90f;
            else           angle =  90f;
            break;
        case ResultDetectorActivity.ORIENTATION_LEFT_SIDE:
        default:
            break;
        }
        angle = 0f;
        return angle;
    }
    // ==============================================================================
    // get SharedPreference
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

    // ==============================================================================
    // get the sample hai image byteArray
    public byte[] getSampleHaiArray(){
        return mSampleImg;
    }

    public boolean[] getConditionArray(){
        return condition;
    }
}