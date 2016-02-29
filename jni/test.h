/*
 * test.h
 *
 *  Created on: Feb 27, 2016
 *      Author: wing
 */

#ifndef TEST_H_
#define TEST_H_
#include <android/log.h>

double getResult(const char* openCLProgramTextNative, char * resultStr, int z,
		int srcLength, int batchSize, float errVar, int deType);
#define  LOG_TAG    "MyOpenCLTag"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif /* TEST_H_ */
