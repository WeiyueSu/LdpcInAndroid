/*
 * MyTest.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: wing
 */

#include "MyLdpc.h"
#include "test.h"
#include<stdlib.h>
#include<time.h>

double getResult(const char* openCLProgramTextNative, char * resultStr, int z,
		int srcLength, int batchSize) {
	int ldpcN = z * 24;
	int ldpcK = ldpcN / 6 * 5;
	int ldpcM = ldpcN - ldpcK;
	enum rate_type rate = rate_5_6;

	Coder coder(ldpcK, ldpcN, rate, openCLProgramTextNative);
	//srand(time(0));

	char * srcCode = (char*) malloc(srcLength * sizeof(char));
	char * priorCode = (char*) malloc(coder.getPriorCodeLength(srcLength));
	float * postCode = (float*) malloc(
			coder.getPostCodeLength(srcLength) * sizeof(float));
	char * newSrcCode = (char*) malloc(srcLength);

	for (int i = 0; i < srcLength; i++) {
		srcCode[i] = 'a' + i % 26;
	}

	coder.forEncoder();
	coder.forDecoder(batchSize);
	clock_t start,end;
	start=clock();
	coder.encode(srcCode, priorCode, srcLength);
	end=clock();
	LOGD("Encode time=%lf",(double)(end-start)/CLOCKS_PER_SEC);

	coder.test(priorCode, postCode, coder.getPriorCodeLength(srcLength), 0.1);
	start=clock();
	coder.decode(postCode, newSrcCode, srcLength);
	end=clock();
	double result=(double)(end-start)/CLOCKS_PER_SEC;
	LOGD("Decode time=%lf",(double)(end-start)/CLOCKS_PER_SEC);

	strncpy(resultStr, newSrcCode, srcLength);
	resultStr[srcLength] = '\0';

	free(srcCode);
	free(priorCode);
	free(postCode);
	free(newSrcCode);
	return result;
}
