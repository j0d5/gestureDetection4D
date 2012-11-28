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
#include <vector>

bool g_IsTrainMode = false;
int g_CurrentTrainClassID = -1;

int g_predictedClass = -1;

//global list for training
vector<XnPoint3D> g_pointList4Training;

// global cyclic buffer
CyclicBuffer<XnPoint3D> g_pointBuffer(BUFFER_SIZE);
SimpleFeatureExtractor g_featureExtractor;
GestureSVM g_gestureSVM;
//one class svm to decide if a feature vector is a gesture or not
GestureSVM g_PreGestureSVM(true);
int frequencyCounter = FEATURE_VECTOR_FREQUENCY;


Point3D convertPoint(XnPoint3D* xnPoint) {
	Point3D p;
	p.X = xnPoint->X;
	p.Y = xnPoint->Y;
	p.Z = xnPoint->Z;
	return p;
}

/**
* creates a trainings vectorm from the train points list, converts it to an Point3D Vector
* calls the getFeatureVector Method on featureExtractor and returns the FeatureVector
*
*/
std::vector<float> extractTrainingFeatureVector() 
{
	printf("***Getting TrainingFeatureVector...:\n");
	std::vector<Point3D> pVector;
		for(vector<XnPoint3D>::iterator it = g_pointList4Training.begin(); it != g_pointList4Training.end(); ++it) {
			pVector.push_back(convertPoint(&(*it)));
		}

				#ifdef DEBUG_FLAG
						std::vector<float> fVector = g_featureExtractor.getFeatureVector(pVector);

						int i = 0;
						for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
							
							printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n",i,*iter,*(iter+1),*(iter+2));
							i++;
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
	printf("Getting WindowedFeatureVector...size: %d \n",size);
	std::vector<Point3D> pVector;

	if(!g_pointBuffer.isFull())
	{
		printf("Error: Buffer not full!!!\n");
		return std::vector<float>();
	}			
	
				g_pointBuffer.resetIterator();
				printf("***RAW Buffer Content***\n");
				for (int i = 0; i < size; i++) {
		
					XnPoint3D* p = g_pointBuffer.next();
					pVector.push_back(convertPoint(p));
					
					#ifdef DEBUG_FLAG					
						printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n",i,p->X,p->Y,p->Z);
					#endif
				}
				g_pointBuffer.resetIterator();

				#ifdef DEBUG_FLAG
						std::vector<float> fVector = g_featureExtractor.getFeatureVector(pVector);

						int i = 0;
						for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
							
							printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n",i,*iter,*(iter+1),*(iter+2));
							i++;
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

	std::vector<string> gestureToTrain;
	gestureToTrain.push_back("Swipe");
	gestureToTrain.push_back("Push");
	gestureToTrain.push_back("Letter L");

	Datasource d;

	
	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str());
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;

	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
		std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
		std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
		std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;
		std::cout << "oniFileID: " << (*iter)->getFileId() << std::endl;

		g_CurrentTrainClassID = (*iter)->getGestureId();  

		//clear current training list (buffer)
		g_pointList4Training.clear();
		if(USE_HAND_POINTS_FROM_DB)
		{
			g_pointList4Training = (*iter)->getHandPoints();
		}
		//replay oni file and re-detect hand points..
		else 
		{
			// replays the current ONI File in trainig mode. 
			playFileSilent((*iter)->getFilepath());	
		}
		std::cout << "Training class: \n" << g_CurrentTrainClassID << std::endl;
		printf("Amount of Training Hand Points: %d\n",g_pointList4Training.size());
		std::vector<float> feature  = extractTrainingFeatureVector();
		if(USE_PRE_SVM)
		{
			// train one class svm 
			for(int i = 0; i < 10; i++)
			{
				g_PreGestureSVM.train(feature,1);
			}
		}
		// train gesture svm
		// train one class svm 
		for(int i = 0; i < 10; i++)
			g_gestureSVM.train(feature, g_CurrentTrainClassID);
		
	}

	//generate and save svm model after after training all oni file data sets
	g_gestureSVM.generateModel(); // this has to be done after collecting feature vectors
	g_gestureSVM.saveModel(SVM_MODEL_FILE);
	if(USE_PRE_SVM)
	{
		g_PreGestureSVM.generateModel();
		g_PreGestureSVM.saveModel(SVM_PRE_MODEL_FILE);
	}
}

void doQuery()
{
	PredictionResult result;
	int numberWindows = sizeof(BUFFER_WINDOWS) / sizeof(double);
	float maxProb = -2;
	int maxClass = 0;
	for(int i = 0; i < numberWindows;i++)
	{
		printf("**BufferWindows: %f\n",BUFFER_WINDOWS[i]);
		std::vector<float> feature  = extractWindowedFeatureVectorFromBuffer(BUFFER_SIZE * BUFFER_WINDOWS[i]);

		// check if gesture is classified as class at all
		result.classID = 1;
		if(USE_PRE_SVM)
		{
			result = g_PreGestureSVM.predictGesture(feature);
			printf("Pre-Predicted(buffer_window: %f) as: %f\n", BUFFER_WINDOWS[i], result.classID);
			maxClass = result.classID;
		}
		// if classID > 0 gesture passed pre svm classification, now predicted gesture in multi class svm
		if(result.classID > 0)
		{
			result = g_gestureSVM.predictGesture(feature);
			if(result.probabilitie > maxProb)
			{
				maxProb = result.probabilitie;
				maxClass = result.classID;
			}
			printf("Predicted as Class (buffer_window: %f) : %d with probability: %f\n\n\n", BUFFER_WINDOWS[i], result.classID, result.probabilitie);
		}
	}
	// set the class with the highes probabiltie as predictedClass
	g_predictedClass = maxClass;
}

void queryWithTrainingData()
{
	std::vector<string> gestureToTrain;
	gestureToTrain.push_back("Swipe");
	gestureToTrain.push_back("Push");
	gestureToTrain.push_back("Letter L");

	Datasource d;

	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str());
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;

	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
		std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
		std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
		std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;
		std::cout << "oniFileID: " << (*iter)->getFileId() << std::endl;

		g_pointList4Training.clear();
		g_pointList4Training = (*iter)->getHandPoints();

		std::vector<float> feature  = extractTrainingFeatureVector();

		PredictionResult result = g_gestureSVM.predictGesture(feature);
		printf("Predicted as Class : %d with probability: %f\n\n",result.classID, result.probabilitie);
	}
}
