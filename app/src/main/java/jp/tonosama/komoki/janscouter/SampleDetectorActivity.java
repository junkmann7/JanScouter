package jp.tonosama.komoki.janscouter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.text.format.DateFormat;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.OrientationEventListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import jp.tonosama.komoki.janscouterUtil.JanScouterPreference;
import jp.tonosama.komoki.janscouterUtil.JanScouterUtil;
import jp.tonosama.komoki.objectInfo.HiInfo;
import jp.tonosama.komoki.util.DevLog;

public class SampleDetectorActivity extends Activity {

    private static final String TAG = SampleDetectorActivity.class.getSimpleName();

    private final int MAIN_LAYOUT_ID = 12345;

    private Camera mCamera;
    private SampleDetectorPreview mPreview;
    private SampleDetectorOverlay mPreviewOverlay;
    int numberOfCameras;
    int cameraCurrentlyLocked;
    Uri imgUri = null;

    OrientationEventListener mOrientationListener;

    // Preferrence
    public boolean isEnableSound = false;
    public boolean isOnlyAnalyze = false;
    public boolean isOnlyPreview = false;

    public static final int ORIENTATION_VERTICAL   = 1;
    public static final int ORIENTATION_RIGHT_SIDE = 2;
    public static final int ORIENTATION_BACK_SIDE  = 3;
    public static final int ORIENTATION_LEFT_SIDE  = 4;
    private int orientation = ORIENTATION_LEFT_SIDE;

    // The first rear facing camera
    int defaultCameraId;

    MediaPlayer mp = null;

    public SampleDetectorActivity(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }
    public void getPreference() {
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

        mPreview = new SampleDetectorPreview(this);
        mPreview.setId(MAIN_LAYOUT_ID);
        setContentView(mPreview);
        mPreviewOverlay = new SampleDetectorOverlay(this);
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
        Toast.makeText(this, getResources().getString(R.string.detection_guide), Toast.LENGTH_LONG).show();
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

    public SampleDetectorOverlay getPreviewOverlay(){
        return mPreviewOverlay;
    }
    public SampleDetectorPreview getCameraPreview(){
        return mPreview;
    }

    // ============================================================================================
    // 牌が検出された後の処理
    // 検出結果ダイアログの表示と音の再生を行う
    public void previewResult(HiInfo hi_info)
    {
        mPreview.mCamera.stopPreview();
        final byte[] mAllHiArray = hi_info.getHiArray();
        int[][] mHiArray = new int[HiInfo.TOTAL_HI_NUM][HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT];

        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ ){
            for( int j=0;j<HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT;j++ )
            {
                int value = mAllHiArray[i*HiInfo.HI_WIDTH*HiInfo.HI_HEIGHT + j] & 0xff;
                mHiArray[i][j] = Color.rgb(value, value, value);
            }
        }

        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Result");

        ScrollView   wrapScrollView     = new ScrollView(this);
        LinearLayout resultLinearLayout = new LinearLayout(this);
        resultLinearLayout.setOrientation(LinearLayout.VERTICAL);

        Bitmap[]    hiBmp = new Bitmap[hi_info.getHiTotalNum()];
        ImageView[] image = new ImageView[hi_info.getHiTotalNum()];
        LinearLayout[] eachRowLLayout = new LinearLayout[4];

        for( int i=0;i<4;i++ )
        {
            eachRowLLayout[i] = new LinearLayout(this);
            eachRowLLayout[i].setOrientation(LinearLayout.HORIZONTAL);
        }
        for( int i=0;i<HiInfo.TOTAL_HI_NUM;i++ )
        {
            image[i] = new ImageView(this);
            hiBmp[i] = Bitmap.createBitmap(mHiArray[i], HiInfo.HI_WIDTH, HiInfo.HI_HEIGHT, Config.RGB_565);
            image[i].setImageBitmap(hiBmp[i]);
            image[i].setScaleType(ImageView.ScaleType.CENTER);
            image[i].setPadding(10, 10, 5, 5);
            eachRowLLayout[i/9].addView(image[i], ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
            if(i%9==8 || i==HiInfo.TOTAL_HI_NUM-1) resultLinearLayout.addView(eachRowLLayout[i/9], ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        }
        wrapScrollView.addView(resultLinearLayout, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        dialog.setView(wrapScrollView);
        dialog.setPositiveButton(R.string.scouter_scan_ok, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                if( getIsSaveInSDcard() ) {
                    // ============================================================================================
                    // sdcard に保存するパターン
                    // デバッグのため現状はこっちで
                    String filename  = HiInfo.SAMPLE_IMAGE_FILE_NAME;
                    String directory = Environment.getExternalStorageDirectory().toString() + HiInfo.SAMPLE_IMAGE_FILE_PATH;
                    OutputStream outputStream = null;

                    try {
                        File dir = new File(directory);
                        if(!dir.exists()){
                            dir.mkdirs();
                            DevLog.d(TAG, dir.toString() + " create");
                        }
                        File file = new File(directory, filename);
                        if( file.exists() ){
                            file.delete();
                        }
                        if (file.createNewFile()) {
                            outputStream = new FileOutputStream(file);
                            if (mAllHiArray != null) {
                                outputStream.write(mAllHiArray);
                            }
                        }
                    } catch (FileNotFoundException ex) {
                        Toast.makeText(getApplicationContext(), "FileNotFoundException", Toast.LENGTH_SHORT).show();
                        DevLog.w(TAG, ex);
                        return;
                    } catch (IOException ex) {
                        Toast.makeText(getApplicationContext(), "IOException", Toast.LENGTH_SHORT).show();
                        DevLog.w(TAG, ex);
                        return;
                    } finally {
                        if (outputStream != null) {
                            try {
                                outputStream.close();
                            } catch (Throwable t) {
                            }
                        }
                    }
                    Toast.makeText(SampleDetectorActivity.this, directory + "/" + filename + "\nにサンプル牌のデータを保存しました", Toast.LENGTH_LONG).show();
                } else {
                    // ============================================================================================
                    // /data/data/～ に保存するパターン
                    // とりあえずは保留
                    String filename  = HiInfo.SAMPLE_IMAGE_FILE_NAME;
                    OutputStream outputStream = null;
                    try {
                        outputStream = openFileOutput(filename, MODE_PRIVATE);
                        if (mAllHiArray != null) {
                            outputStream.write(mAllHiArray);
                        }
                    } catch (FileNotFoundException ex) {
                        Toast.makeText(getApplicationContext(), "FileNotFoundException", Toast.LENGTH_SHORT).show();
                        DevLog.w(TAG, ex);
                        return;
                    } catch (IOException ex) {
                        Toast.makeText(getApplicationContext(), "Please insert SD card.", Toast.LENGTH_SHORT).show();
                        DevLog.w(TAG, ex);
                        return;
                    } finally {
                        if (outputStream != null) {
                            try {
                                outputStream.close();
                            } catch (Throwable t) {
                            }
                        }
                    }
                    Toast.makeText(SampleDetectorActivity.this, "サンプル牌のデータを保存しました", Toast.LENGTH_LONG).show();
                }
                dialog.dismiss();
                //mPreview.mCamera.startPreview();
                //mPreview.restartProcessFrame();
                finish();
            }
        });
        dialog.setNegativeButton(R.string.scouter_scan_ng, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                dialog.dismiss();
                mPreview.mCamera.startPreview();
                mPreview.restartProcessFrame();
            }
        });
        dialog.show();

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
            mp = MediaPlayer.create(this, R.raw.end);
            mp.start();
        }
    }

    // ==============================================================================
    // if bitmap pressed
    public void previewBitmap()
    {
        mPreview.mCamera.stopPreview();

        if( !getIsOnlyPreview() ) {
            Bitmap bmp = mPreviewOverlay.getDecodedBitmap();
            long dateTaken   = System.currentTimeMillis();
            String name      = DateFormat.format("yyyy-MM-dd_kk.mm.ss", dateTaken).toString() + ".jpg";
            String filename  = name;
            String directory = Environment.getExternalStorageDirectory().toString() + "/" + "JanScouter";
            String filePath  = directory + "/" + filename;
            DevLog.d(TAG, "filepath : " + filePath);
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
                DevLog.w(TAG, ex);
                return;
            } catch (IOException ex) {
                Toast.makeText(this, "Please insert SD card.", Toast.LENGTH_SHORT).show();
                DevLog.w(TAG, ex);
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
        DevLog.d(TAG, "saveImage() start.");

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
        DevLog.d(TAG, "filepath : " + filePath);
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
            DevLog.w(TAG, ex);
            return;
        } catch (IOException ex) {
            Toast.makeText(this, "Please insert SD card.", Toast.LENGTH_SHORT).show();
            DevLog.w(TAG, ex);
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

        DevLog.d(TAG, "saveImage() end.");
    }
    // ==============================================================================
    // if Debug Button Pressed
    public void setDebugBtnPressed() {
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
        case SampleDetectorActivity.ORIENTATION_VERTICAL:
            if(is_reverse) angle =  90f;
            else           angle = -90f;
            break;
        case SampleDetectorActivity.ORIENTATION_RIGHT_SIDE:
            angle = 180f;
            break;
        case SampleDetectorActivity.ORIENTATION_BACK_SIDE:
            if(is_reverse) angle = -90f;
            else           angle =  90f;
            break;
        case SampleDetectorActivity.ORIENTATION_LEFT_SIDE:
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
}