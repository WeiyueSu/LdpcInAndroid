/*
 * MyTest.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: wing
 */

#include<iostream>
#include<time.h>
#include "MyLdpc.h"
#include "test.h"
using namespace std;

//argv[1]=srcLength,argv[2]=batchSize;
double getResult(const char* openCLProgramTextNative, char * resultStr, int z,
		int srcLength, int batchSize, float errVar, int deType) {

	clock_t start, end;
	int ldpcN = z * 24;
	int ldpcK = ldpcN / 6 * 5;
	int ldpcM = ldpcN - ldpcK;
	enum rate_type rate = rate_5_6;

	Coder coder(ldpcK, ldpcN, rate);
	srand(time(0));

	//int srcLength = 10;
	char * srcCode = (char*) malloc(srcLength * sizeof(char));
	char * priorCode = (char*) malloc(coder.getPriorCodeLength(srcLength));
	float * postCode = (float*) malloc(
			coder.getPostCodeLength(srcLength) * sizeof(float));
	char * newSrcCode = (char*) malloc(srcLength);

	for (int i = 0; i < srcLength; i++) {
		srcCode[i] = 'a' + i % 26;
	}
	double decodeTime;
	coder.forEncoder();
	coder.forDecoder(batchSize);
	//coder.forDecoder(1);
	start = clock();
	coder.encode(srcCode, priorCode, srcLength);
	end = clock();
	double encodeTime = (double) (end - start) / CLOCKS_PER_SEC;
	cout << "encode time=" << encodeTime << endl;

	coder.test(priorCode, postCode, coder.getPriorCodeLength(srcLength),
			errVar);
	switch (deType) {
	case DecodeMS:
		coder.addDecodeType(DecodeMS);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeMS);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		LOGD("MS:%f",decodeTime);
		int errNum = 0;
		for (int i = 0; i < srcLength; ++i) {
			if (srcCode[i] != newSrcCode[i])
				++errNum;
		}
		LOGD("ErrNum=%d",errNum);
		break;
	case DecodeSP:
		coder.addDecodeType(DecodeSP);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeSP);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		LOGD("SP:%f",decodeTime);
		int errNum = 0;
		for (int i = 0; i < srcLength; ++i) {
			if (srcCode[i] != newSrcCode[i])
				++errNum;
		}
		LOGD("ErrNum=%d",errNum);
		break;
	case DecodeCPU:
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeCPU);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		LOGD("CPU:%f",decodeTime);
		int errNum = 0;
		for (int i = 0; i < srcLength; ++i) {
			if (srcCode[i] != newSrcCode[i])
				++errNum;
		}
		LOGD("ErrNum=%d",errNum);
		break;
	}
	free(srcCode);
	free(priorCode);
	free(postCode);
	free(newSrcCode);
}

