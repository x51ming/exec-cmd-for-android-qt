package org.qtproject.example;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import org.qtproject.qt.android.bindings.QtService;

public class QtAndroidService extends QtService
{
    private static final String TAG = "QtAndroidService";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "Destroying Service");
    }
}
