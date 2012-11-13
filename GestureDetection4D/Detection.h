#pragma once

// local header
#include "CyclicBuffer.h"
#include "../FeatureExtractor4D/SimpleFeatureExtractor.h"
#include "../SVM/GestureSVM.h"


// Header for NITE
#include "XnVNite.h"

#define BUFFER_SIZE 30
//use every n-th hand update
#define FEATURE_VECTOR_FREQUENCY 3
#define WINDOW_NAME "GestureDetection4D"
#define SVM_MODEL_FILE "../../SVM_Model.txt"
#define DEBUG_FLAG


bool g_IsTrainMode = false;
int g_CurrentTrainClassID = -1;

// global cyclic buffer
CyclicBuffer<XnPoint3D> g_pointBuffer(BUFFER_SIZE);
SimpleFeatureExtractor g_featureExtractor;
GestureSVM g_gestureSVM;
int frequencyCounter = FEATURE_VECTOR_FREQUENCY;


Point3D convertPoint(XnPoint3D* xnPoint) {
	Point3D p;
	p.X = xnPoint->X;
	p.Y = xnPoint->Y;
	p.Z = xnPoint->Z;
	return p;
}
std::vector<float> extractFeatureVectorFromBuffer() {
        printf("Getting FeatureVector...\n");
        std::vector<Point3D> pVector;

		if(!g_pointBuffer.isFull())
		{
			printf("Error: Buffer not full!!!\n");
			return std::vector<float>();
		}
		for (int i = 0; i < BUFFER_SIZE; i++) {
                pVector.push_back(convertPoint(g_pointBuffer.next()));
        }

		#ifdef DEBUG_FLAG
        std::vector<float> fVector = g_featureExtractor.getFeatureVector(pVector);

        for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
                printf("FeatureVector X: %.2f, Y: %.2f, Z: %.2f\n",*iter,*(iter+1),*(iter+2));
        }
		#endif

		return g_featureExtractor.getFeatureVector(pVector);
}



