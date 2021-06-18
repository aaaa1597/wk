package com.tks.cg3dviewer.modelformat;

/**
 * Created by jun on 2021/06/07
 */
public class MQO {
    static {
        System.loadLibrary("cg3dviewer");
    }

    public static native boolean init();
    public static native void setDrawArea(int width, int height);
    public static native void draw();
    public static native void setTouchAngle(float aTouchAngleX, float aTouchAngleY);
}
