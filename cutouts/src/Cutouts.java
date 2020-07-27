package com.defold.android;

import android.app.Activity;
import android.view.DisplayCutout;
import android.os.Build;

class Cutouts {
    public static int[] GetOffsets(final Activity activity) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                final DisplayCutout displayCutout = activity.getWindow().getDecorView().getRootWindowInsets().getDisplayCutout();
                if (displayCutout != null) {
                    int[] offsets = {
                        displayCutout.getSafeInsetLeft(),
                        displayCutout.getSafeInsetRight(),
                        displayCutout.getSafeInsetTop(),
                        displayCutout.getSafeInsetBottom()
                    };
                    return offsets;
                }
            }
        }
        catch (Exception e) {}

        int[] fallback = {0,0,0,0};
        return fallback;
    }
}
