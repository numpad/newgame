package com.as.example;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class main_activity extends Activity {
	// public static native int hello_cpp(); // declare c or c++ function (synonymous with c's extern)

	static 
	{
		// System.loadLibrary("android_studio_example"); // load c or c++ lib
	}

	@Override
	protected void onCreate(Bundle arg0) 
	{
		Log.d("Hello world!", "I'm Java");

		// hello_cpp(); // call c or c++ function 

		super.onCreate(arg0);
	}
}

