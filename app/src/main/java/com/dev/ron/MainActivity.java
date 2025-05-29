package com.dev.ron;

import android.os.Bundle;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "NativeAPI";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // Your layout if any

        // Call native method
        String response = NativeApi.postVerifyData();

        // Show Toast with the response
        Toast.makeText(this, "Response: " + response, Toast.LENGTH_LONG).show();
    }
}