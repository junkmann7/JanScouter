package jp.tonosama.komoki.util;

import android.util.Log;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Wrapper class for android.util.Log class.
 *
 * This class can control if output or not to the logcat.
 * This class doesn't take care of thread-safety.
 */
public class DevLog {

    private static boolean LOG_ENABLED = false;

    private static final String DEFAULT_TAG = "[JanScouter] ";

    /**
     * Check if log output is enabled or not.
     *
     * @param enable if true, then log output is enabled, otherwise not.
     */
    public static void setEnable(boolean enable) {
        LOG_ENABLED = enable;
    }

    /**
     * Check if log output is enabled or not.
     *
     * @return boolean return true if log output is enabled.
     */
    public static boolean isEnabled() {
        return LOG_ENABLED;
    }

    public static void v(String str) {
        if (LOG_ENABLED) {
            v(DEFAULT_TAG, str);
        }
    }

    private static void v(String userTag, String str) {
        if (LOG_ENABLED) {
            Log.v(userTag, str);
        }
    }

    public static void d(String str) {
        if (LOG_ENABLED) {
            d(DEFAULT_TAG, str);
        }
    }

    public static void d(String userTag, String str) {
        if (LOG_ENABLED) {
            Log.d(userTag, str);
        }
    }

    public static void i(String str) {
        if (LOG_ENABLED) {
            i(DEFAULT_TAG, str);
        }
    }

    public static void i(String userTag, String str) {
        if (LOG_ENABLED) {
            Log.i(userTag, str);
        }
    }

    public static void w(String str) {
        if (LOG_ENABLED) {
            w(DEFAULT_TAG, str);
        }
    }

    private static void w(String userTag, String str) {
        if (LOG_ENABLED) {
            Log.w(userTag, str);
        }
    }

    public static void w(String userTag, Exception ex) {
        if (LOG_ENABLED) {
            Log.w(userTag, ex);
        }
    }

    public static void e(String str) {
        if (LOG_ENABLED) {
            e(DEFAULT_TAG, str);
        }
    }

    public static void e(String userTag, String str) {
        if (LOG_ENABLED) {
            Log.e(userTag, str);
        }
    }

    public static void e(String userTag, String str, Exception exception) {
        if (LOG_ENABLED) {
            Log.e(userTag, str, exception);
        }
    }

    public static void stackTrace(Throwable e) {
        if (LOG_ENABLED) {
            Log.e(DEFAULT_TAG, "Stacktrace Logger", e);
        }
    }

    public static void stacktrace(String userTag, Throwable e) {
        if (LOG_ENABLED) {
            Log.e(userTag, "Stacktrace Logger", e);
        }
    }
}
