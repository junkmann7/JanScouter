package jp.tonosama.komoki.janscouterUtil;

@SuppressWarnings("JniMissingFunction")
public final class JanScouterUtil {

    public JanScouterUtil() {
    }

    public static native byte[] rotateYUV(byte[] img, int imgWidth, int imgHeight, int[] out );

    public static native void decodeYUV2BIN(byte[] img, int imgWidth, int imgHeight, int[] out );

    public static native void decodeYUV2GRAY(byte[] img, int imgWidth, int imgHeight, int[] out );

    public static native void decodeYUV2RGB(byte[] img, int imgWidth, int imgHeight, int[] out );

    public static native int JanSampleDetector(
        byte[]  img,              // カメラから取得したスルー画像。
        int     imgWidth,         // byte[] img の横サイズ。
        int     imgHeight,        // byte[] img の縦サイズ。
        int     guidePosX,        // ガイド枠の開始位置 X 座標
        int     guidePosY,        // ガイド枠の開始位置 Y 座標
        int     guideWidth,       // ガイド枠の横サイズ
        int     guideHeight,      // ガイド枠の縦サイズ
        int[]   pointX,           // 各牌のX座標。出力用。
        int[]   pointY,           // 各牌のY座標。出力用。
        int[]   tileWidth,        // 各牌の横幅。出力用。
        int[]   tileHeight,       // 各牌の縦幅。出力用。
        byte[]  hiArray,          // 各牌の2値画像。出力用。
        int[]   edgeImg,          // 解析画像。デバッグ用。
        boolean isDebugBtnPressed // デバッグボタン押下判定。デバッグ用。
        );

    public static native int JanResultDetector(
        byte[]    img,              // カメラから取得したスルー画像。
        int[]     imgSize,          // byte[] img の横サイズ、縦サイズ、ガイド枠の開始 X 座標、Y 座標、ガイド枠の横サイズ、縦サイズが入る。入力用。
        int[]     pointX,           // 各牌のX座標。出力用。
        int[]     pointY,           // 各牌のY座標。出力用。
        int[]     tileWidth,        // 各牌の横幅。出力用。
        int[]     tileHeight,       // 各牌の縦幅。出力用。
        byte[]    hiArray,          // 各牌の2値画像。出力用。
        boolean[] nakiflag,         // 鳴き牌フラグ。
        int[]     haiNum,           // 手牌、鳴き牌、ドラ牌の数が入る。出力用。
        byte[]    tehaiImg,         // 手牌の画像。  出力用。
        byte[]    doraImg,          // ドラ牌の画像。出力用。
        byte[]    nakiImg,          // 鳴き牌の画像。出力用。
        int[]     edgeImg,          // 解析画像。デバッグ用。
        boolean   isDebugBtnPressed // デバッグボタン押下判定。デバッグ用。
        );

    public static native int JanResultMatching(
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
        );

    public static native int JanComputeResult(
        boolean[] condition,   // あがり条件
        byte[]    tehai,       // 解析した手牌の情報。31432239・・・ みたいな感じで値が入ってて、それぞれ「ピンズの１」とか「ソーズの５」とかを意味してる。
        byte[]    dora,        // ドラの情報。
        byte[]    nakihai,     // 解析した鳴き牌の情報。「東東東」とか「ピンズ345」とかの値が入る。
        boolean[] nakiflag,    // 鳴き牌かどうかの情報。
        int[]     haiNum,      // 手牌、鳴き牌、ドラ牌の数、本場数が入る。入力用。
        int[]     result,      // フ、ハン、点数の値。出力用。
        boolean[] yakulist,    // あがり役名のリスト。出力用。
        boolean   isReCompute  // 手動修正による実行かどうか。
        );

    /* * *
     * condition[0]  : ツモ
     * condition[1]  : 立直
     * condition[2]  : 一発
     * condition[3]  : 嶺上
     * condition[4]  : 海底
     * condition[5]  : Ｗ立直
     * condition[6]  : オープン
     * condition[7]  : 場風は東
     * condition[8]  : 自風は東
     * condition[9]  : 自風は南
     * condition[10] : 自風は西
     * condition[11] : 自風は北
     */

    static
    {
        System.loadLibrary("JanScouterUtil");
    }
}
