package com.tks.cg3dviewer;

import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.tks.cg3dviewer.modelformat.FBX;
import com.tks.cg3dviewer.modelformat.MQO;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CG3DViewerActivity extends AppCompatActivity {
    private GLSurfaceView mGLSurfaceView = null;
    public AssetManager mAssetManager = null;

    static {
        System.loadLibrary("cg3dviewer");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cg3dviewer);

        mGLSurfaceView = new GLSurfaceView(getApplication());
        mGLSurfaceView.setEGLContextClientVersion(2);
        mGLSurfaceView.setZOrderOnTop(true);
        mGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        mGLSurfaceView.setEGLConfigChooser(8,8,8,8,16,8);
        addContentView(mGLSurfaceView, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
        mGLSurfaceView.setRenderer(new CG3DRender());

        mAssetManager = getResources().getAssets();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLSurfaceView.onPause();
    }

    private float mTouchAngleX, mTouchAngleY;
    private float mLastX,mLastY;
    private int mHeight;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch(event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mLastX = event.getX();
                mLastY = event.getY();
                return true;
            case MotionEvent.ACTION_MOVE:
            {
                float factor = 100f / mHeight;
                float dx = factor * (event.getX() - mLastX);
                float dy = factor * (event.getY() - mLastY);
                mTouchAngleX = Math.max(Math.min(mTouchAngleX+dy,90f),-90f);
                mTouchAngleY += dx;
                setTouchAngle(mTouchAngleX, mTouchAngleY);
                MQO.setTouchAngle(mTouchAngleX, mTouchAngleY);
                mLastX = event.getX();
                mLastY = event.getY();
            }
            return true;
        }
        return super.onTouchEvent(event);
    }

    public static native boolean init(AssetManager mAssetManager, String[] object);
    public static native void setDrawArea(int width, int height);
    public static native void draw();
    public static native void setTouchAngle(float aTouchAngleX, float aTouchAngleY);

    private static List<String> getFiles( android.content.res.AssetManager am, String file) {
        List<String> retList = new ArrayList<String>();
        try {
            String[] subfiles = am.list(file);
            if(subfiles.length != 0) {
                Arrays.stream(subfiles).forEach(f -> {
                    retList.addAll(getFiles(am, file+"/"+f));
                });
            }
            else {
                retList.add(file);
                return retList;
            }
        }
        catch (IOException e) { return new ArrayList<String>();}
        return retList;
    }

    private class CG3DRender implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            /* Asset配下のファイル一覧を取得 */
            AtomicReference<List<String>> atomfilelist = new AtomicReference<List<String>>();
            atomfilelist.set(new ArrayList<String>());
            try {
                String[] files = CG3DViewerActivity.this.getAssets().list("");
                Arrays.stream(files).forEach(f -> {
                    if(f.equals("images")) return;
                    if(f.equals("webkit")) return;
                    List<String> filelist = atomfilelist.get();
                    filelist.addAll(getFiles(CG3DViewerActivity.this.getAssets(), f));
                    atomfilelist.set(filelist);
                });

            }
            catch (IOException e) { }

            boolean ret = init(CG3DViewerActivity.this.mAssetManager, atomfilelist.get().toArray(new String[atomfilelist.get().size()]));
            if(!ret) throw new RuntimeException("CG3DViewer.init()で失敗");
            boolean ret1 = MQO.init();
            if(!ret1) throw new RuntimeException("MQO.init()で失敗");
            boolean ret2 = FBX.init();
            if(!ret2) throw new RuntimeException("FBX.init()で失敗");
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            mHeight = height;
            setDrawArea(width, height);
            MQO.setDrawArea(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            draw();
            MQO.draw();
        }
    }
}
