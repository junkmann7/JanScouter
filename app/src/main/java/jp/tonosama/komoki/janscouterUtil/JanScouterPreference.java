package jp.tonosama.komoki.janscouterUtil;

import jp.tonosama.komoki.janscouter.R;
import jp.tonosama.komoki.util.DevLog;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.util.Log;

public class JanScouterPreference extends PreferenceActivity {

    private static final String TAG = JanScouterPreference.class.getSimpleName();

    private ListPreference mPrefScouterSize;

    public JanScouterPreference(){
        DevLog.i(TAG, "Instantiated new " + this.getClass());
    }
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        addPreferencesFromResource(R.xml.pref);

        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        String scouterSize = pref.getString("SCOUTER_SIZE", "5");
        mPrefScouterSize = (ListPreference)findPreference("SCOUTER_SIZE");
        mPrefScouterSize.setValue(scouterSize);
        mPrefScouterSize.setSummary(mPrefScouterSize.getEntry());

        mPrefScouterSize.setOnPreferenceChangeListener(new OnPreferenceChangeListener(){
            @Override
            public boolean onPreferenceChange(Preference preference,Object newValue) {
                mPrefScouterSize.setValue((String)newValue);
                mPrefScouterSize.setSummary(mPrefScouterSize.getEntry());
                return false;
            }
        });
    }
}