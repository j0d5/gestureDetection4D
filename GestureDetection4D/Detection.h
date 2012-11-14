#pragma once

// local header
#include "CyclicBuffer.h"
#include "../FeatureExtractor4D/SimpleFeatureExtractor.h"
#include "../SVM/GestureSVM.h"
#include "Datasource.h"
#include "Device.h"
#include "Constants.h"

// Header for NITE
#include "XnVNite.h"

bool g_IsTrainMode = false;
int g_CurrentTrainClassID = -1;

// global cyclic buffer
CyclicBuffer<XnPoint3D> g_pointBuffer(BUFFER_SIZE);
SimpleFeatureExtractor g_featureExtractor;
GestureSVM g_gestureSVM;
GestureSVM g_PreGestureSVM;
int frequencyCounter = FEATURE_VECTOR_FREQUENCY;


Point3D convertPoint(XnPoint3D* xnPoint) {
	Point3D p;
	p.X = xnPoint->X;
	p.Y = xnPoint->Y;
	p.Z = xnPoint->Z;
	return p;
}

/**
 * creates a vector from buffer, converts it to an Point3D Vector
 * calls the getFeatureVector Method on featureExtractor and returns the FeatureVector
 *
 */
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

/**
 * creates a window with an vector from buffer, converts it to an Point3D Vector
 * calls the getFeatureVector Method on featureExtractor and returns the FeatureVector
 *
 */
std::vector<float> extractWindowedFeatureVectorFromBuffer(int size) {
	printf("Getting WindowedFeatureVector...\n");
	std::vector<Point3D> pVector;

	if(!g_pointBuffer.isFull())
	{
		printf("Error: Buffer not full!!!\n");
		return std::vector<float>();
	}

	for (int i = 0; i < size; i++) {
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

/**
 * sets the training mode
 */
void doTraining()
{
	g_IsTrainMode = true;

	Datasource d;
	//maybe todo: add flag to oni entrys to mark training data!
	std::vector<OniFileDataSet*> oniFiles = d.getOniFileDatasets();

	std::vector<OniFileDataSet*>::iterator iter;

	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
		std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
		std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
		std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;

		g_CurrentTrainClassID = (*iter)->getGestureId();  

		//replays the current ONI File in trainig mode. 
		playFileSilent((*iter)->getFilepath());

		printf("Training class %d\n", g_CurrentTrainClassID);
		printf("Extract feature Vector from buffer\n");
		std::vector<float> feature  = extractFeatureVectorFromBuffer();
		g_gestureSVM.train(feature, g_CurrentTrainClassID);

	}


	//generate and save svm model after after training all oni files
	g_gestureSVM.generateModel(); // this has to be done after collecting feature vectors
	g_gestureSVM.saveModel(SVM_MODEL_FILE);
}

void doQuery()
{
	std::vector<float> feature  = extractFeatureVectorFromBuffer();

	double predictedClass = g_gestureSVM.predictGesture(feature);
	printf("Predicted as Class %f\n",predictedClass);
}