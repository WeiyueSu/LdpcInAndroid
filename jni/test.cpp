/*
 * MyTest.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: wing
 */

#include <MyLdpc.h>
#include<iostream>
#include<time.h>
#include "test.h"
using namespace std;

void getResult(const char* openCLProgramTextNative, char* resultStr, int z,
		int srcLength, int batchSize, float snr, int deType,int times,int rate2) {

	clock_t start, end;

	int ldpcN = z * 24;
	int ldpcK = ldpcN / 6 * 5;
	int ldpcM = ldpcN - ldpcK;
	enum rate_type rate = rate_5_6;


	Coder coder(ldpcK,ldpcN, rate_5_6);
	//coder.times=times;
	srand(time(0));
	coder.kernelSourceCode = openCLProgramTextNative;

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

	int errNum = 0;
	float sd = 1 / (pow(10, snr / 20));
	coder.test(priorCode, postCode, coder.getPriorCodeLength(srcLength), sd);
	switch (deType) {
	case DecodeMS:
		coder.addDecodeType(DecodeMS);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeMS);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		LOGD("MS:%f", decodeTime);
		break;
	case DecodeSP:
		coder.addDecodeType(DecodeSP);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeSP);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		LOGD("SP:%f", decodeTime);
		break;
	case DecodeCPU:
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeCPU);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		break;
	case DecodeTDMP:
		coder.addDecodeType(DecodeTDMP);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeTDMP);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		break;
	case DecodeTDMPCL:
		coder.addDecodeType(DecodeTDMPCL);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeTDMPCL);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		break;
	case DecodeMSCL:
		coder.addDecodeType(DecodeMSCL);
		start = clock();
		coder.decode(postCode, newSrcCode, srcLength, DecodeMSCL);
		end = clock();
		decodeTime = (double) (end - start) / CLOCKS_PER_SEC;
		break;
	}
	for (int i = 0; i < srcLength; ++i) {
		if (srcCode[i] != newSrcCode[i])
			++errNum;
	}

	float throughPut = srcLength / decodeTime;
	float ber=errNum/srcLength;
	sprintf(resultStr, "throughPut=%f\nber=%f\nerrNum=%d\ndecodeTime=%f\n", throughPut,ber,
			errNum, decodeTime);
	free(srcCode);
	free(priorCode);
	free(postCode);
	free(newSrcCode);

}

