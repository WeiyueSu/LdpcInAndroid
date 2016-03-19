#include <jni.h>
#include <string>
#include<cstring>
#include <iostream>
#include<iosfwd>
#include <android/log.h>
#include <test.h>
#include"cl.hpp"

extern "C" jstring Java_com_github_wing02_ldpcinandroid_MainActivity_stringFromJNI(
		JNIEnv* env, jobject thiz, jstring openCLProgramText, jint z,
		jint srcLength, jint batchSize, jfloat snr, jint deType, jint times,
		jint rate) {

	LOGD("start MyOpenCL");
	LOGD("z=%d,srcLength=%d,batchSize=%d", z, srcLength, batchSize);
	const char* openCLProgramTextNative = env->GetStringUTFChars(
			openCLProgramText, 0);

	char resultStr[1000] = "";

	getResult(openCLProgramTextNative, resultStr, z, srcLength, batchSize, snr,
			deType, times, rate);

	return (env)->NewStringUTF(resultStr);
}
extern "C" jstring Java_com_github_wing02_ldpcinandroid_MainActivity_getLog(
		JNIEnv* env, jobject thiz) {

	cl_int errNum;
	std::vector<cl::Platform> platformList;
	cl::Platform::get(&platformList);

	cl_context_properties cprops[] = { CL_CONTEXT_PLATFORM,
			(cl_context_properties) (platformList[0])(), 0 };

	cl::Context context = cl::Context(CL_DEVICE_TYPE_GPU, cprops, NULL, NULL,
			&errNum);
	//create commandqueue
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

	size_t size;
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_NAME, 0, NULL, &size);
	char *name = (char*) alloca(sizeof(char) * size);
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_NAME, size, name, NULL);

	//
	size_t workItemSize[3];
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_MAX_WORK_ITEM_SIZES,
			3 * sizeof(size_t), workItemSize, NULL);
	size_t workGroupSize;
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_MAX_WORK_GROUP_SIZE, 0,
	NULL, &size);
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_MAX_WORK_GROUP_SIZE, size,
			&workItemSize, NULL);
	cl_ulong globalMem;
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_GLOBAL_MEM_SIZE,
			sizeof(cl_ulong), &globalMem, NULL);
	cl_ulong localMem;
	errNum = clGetDeviceInfo(devices[0](), CL_DEVICE_LOCAL_MEM_SIZE,
			sizeof(cl_ulong), &localMem, NULL);

	cl::Context context2 = cl::Context(CL_DEVICE_TYPE_CPU, cprops, NULL, NULL,
			&errNum);
	std::vector<cl::Device> devices2 = context2.getInfo<CL_CONTEXT_DEVICES>();
	char log[1000] = "";
	sprintf(log,
			"platformNum=%d;deviceNum=%d;deviceName=%s,cpuNum=%d;workItemSize=%d %d %d;workgroupSize=%d;globalMem=%llu;localMem=%llu;",
			platformList.size(), devices.size(), name, devices2.size(),
			workItemSize[0], workItemSize[1], workItemSize[2], workGroupSize,
			globalMem, localMem);
	return (env)->NewStringUTF(log);
}
