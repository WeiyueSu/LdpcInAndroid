#include <jni.h>
#include <string>
#include <iostream>
#include"test.h"
#include <android/log.h>

extern "C" jdouble Java_com_github_wing02_ldpcinandroid_MainActivity_stringFromJNI(
		JNIEnv* env, jobject thiz, jstring openCLProgramText, jint z,
		jint srcLength, jint batchSize) {

	__android_log_print(ANDROID_LOG_DEBUG, "MyOpenCL.cpp",
			"what the fuck MyOpenCL");
	LOGD("start MyOpenCL");
	LOGD("z=%d,srcLength=%d,batchSize=%d", z, srcLength, batchSize);
	char resultStr[50000];
	const char* openCLProgramTextNative = env->GetStringUTFChars(
			openCLProgramText, 0);

	//LOGD("OpenCL text is %s",openCLProgramTextNative);
	jdouble result=getResult(openCLProgramTextNative, resultStr, z, srcLength, batchSize);

	return result;
	//return (env)->NewStringUTF(resultStr);
}
