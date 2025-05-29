package com.dev.ron;

public class NativeApi {
    static {
        System.loadLibrary("native-lib"); // Load native library
    }

    // Declare native method
    public static native String postVerifyData();
}