#pragma once

// local header
#include "CyclicBuffer.h"
#include "../FeatureExtractor4D/SecondSimpleFeatureExtractor.h"
#include "../FeatureExtractor4D/DTWFeatureExtractor.h"
#include "../SVM/GestureSVM.h"
#include "Datasource.h"
#include "Device.h"
#include "Constants.h"
#include "../DTW/dtw.h"


// Header for NITE
#include "XnVNite.h"
#include <vector>

bool g_UseDTW_NN = false;
bool g_IsTrainMode = false;
int g_CurrentTrainClassID = -1;
int g_predictedClass = -1;
std::map<int, char*> gestureNames;

//global list for training
vector<XnPoint3D> g_pointList4Training;

// global cyclic buffer
CyclicBuffer<XnPoint3D> g_pointBuffer(BUFFER_SIZE);
CyclicBuffer<int> g_lastPredictedGestures(LAST_PREDICTED_GESTURE_BUFFER_SIZE);

IFeatureExtractor4D* g_featureExtractor;
GestureSVM g_gestureSVM;
GestureSVM g_PreGestureSVM(true); // one class svm to decide if a feature vector is a gesture or not
int frequencyCounter = FEATURE_VECTOR_FREQUENCY;

// convert 3D points form XnPoint to Point3D
inline Point3D convertPoint(XnPoint3D* xnPoint) {
	return Point3D(xnPoint->X, xnPoint->Y, xnPoint->Z);
}

DTW g_dtw;
/**
* creates a trainings vectorm from the train points list, converts it to an Point3D Vector
* calls the getFeatureVector Method on featureExtractor and returns the FeatureVector
*
*/
std::vector<float> extractTrainingFeatureVector() 
{
#ifdef DEBUG_FLAG
	printf("***Getting TrainingFeatureVector...:\n");
#endif
	std::vector<Point3D> pVector;
	for(vector<XnPoint3D>::iterator it = g_pointList4Training.begin(); it != g_pointList4Training.end(); ++it) {
		pVector.push_back(convertPoint(&(*it)));
	}

#ifdef DEBUG_FLAG
	std::vector<float> fVector = g_featureExtractor->getFeatureVector(pVector);

	int i = 0;
	for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
		printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n",i,*iter,*(iter+1),*(iter+2));
		i++;
	}
#endif

	return g_featureExtractor->getFeatureVector(pVector);
}

/**
* creates a window with an vector from buffer, converts it to an Point3D Vector
* calls the getFeatureVector Method on featureExtractor and returns the FeatureVector
*
*/
std::vector<float> extractWindowedFeatureVectorFromBuffer(int size) {
#ifdef DEBUG_ALL
	printf("Getting WindowedFeatureVector...size: %d \n",size);
#endif
	std::vector<Point3D> pVector;

	if(!g_pointBuffer.isFull())
	{
		printf("Error: Buffer not full!!!\n");
		return std::vector<float>();
	}			

	// set iterator to actual element + pos
	XnPoint3D* actPoint = g_pointBuffer.setIterator(BUFFER_SIZE - size);

#ifdef DEBUG_ALL
	printf("Setting Iterator to: X %.5f Y %.5f Z %.5f\n", actPoint->X, actPoint->Y, actPoint->Z);
	printf("***RAW Buffer Content***\n");
#endif

	// push points from buffer to featureVector
	// for (int i = 0; i < size; i++) { // from beginning of buffer
	for (int i = BUFFER_SIZE - size; i < BUFFER_SIZE; i++) {
		XnPoint3D* p = g_pointBuffer.next();
		pVector.push_back(convertPoint(p));

#ifdef DEBUG_ALL					
		printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n", i, p->X, p->Y, p->Z);
#endif
	}

#ifdef DEBUG_ALL // print out feature vectors
	printf("***FeatureVector Content***\n");
	std::vector<float> fVector = g_featureExtractor.getFeatureVector(pVector);
	int i = 0;
	for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
		printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n",i,*iter,*(iter+1),*(iter+2));
		i++;
	}
#endif

	return g_featureExtractor->getFeatureVector(pVector);
}
/*
   does dtw next neighbour training
*/
void doDTWTraining()
{
	g_IsTrainMode = true;
	/*
		define the set of gestures to be trained. possible gestures are find by the makros starting with
		'GESTURE_'. the value correspondes to the gesture name in the db.
	*/
	std::vector<string> gestureToTrain;
	gestureToTrain.push_back(GESTURE_SWIPE);
	gestureToTrain.push_back(GESTURE_PUSH);
	gestureToTrain.push_back(GESTURE_L);
	gestureToTrain.push_back(GESTURE_O);
	gestureToTrain.push_back(GESTURE_Z);
	Datasource d;

	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str(),0,20);
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;
	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{

		g_CurrentTrainClassID = (*iter)->getGestureId();  

		//clear current training list (buffer)
		g_pointList4Training.clear();
		g_pointList4Training = (*iter)->getHandPoints();
		
		std::vector<float> feature  = extractTrainingFeatureVector();
		
		g_dtw.addTrainingFeatureFromRawData(feature,g_CurrentTrainClassID);
	}
	cout << "Finished DTW NN Training  with " << oniFiles.size() << "Features" << endl;
	
	
	double totalCvAcc = 0.0;
	for(int i = 0; i < 1;i++)
	{
		totalCvAcc+= g_dtw.doCrossValidation(5);
	}
	g_IsTrainMode = false;
	cout << "Total dwt-nn cross validation acc = " << totalCvAcc / 10 << endl;
}


/**
* sets the training mode
*/
void doTraining()
{
	g_IsTrainMode = true;
	/*
		define the set of gestures to be trained. possible gestures are find by the makros starting with
		'GESTURE_'. the value correspondes to the gesture name in the db.
	*/
	std::vector<string> gestureToTrain;
	gestureToTrain.push_back(GESTURE_SWIPE);
	gestureToTrain.push_back(GESTURE_PUSH);
	gestureToTrain.push_back(GESTURE_L);
	gestureToTrain.push_back(GESTURE_O);
	gestureToTrain.push_back(GESTURE_Z);
	Datasource d;

	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str());
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;

	std::vector<std::vector<float> > allFeatures;
	std::vector<int> featureClassIdx;
	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
#ifdef DEBUG_FLAG
		std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
		std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
		std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;
		std::cout << "oniFileID: " << (*iter)->getFileId() << std::endl;
#endif
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
#ifdef DEBUG_FLAG
		std::cout << "Training class: \n" << g_CurrentTrainClassID << std::endl;
		printf("Amount of Training Hand Points: %d\n",g_pointList4Training.size());
#endif
		std::vector<float> feature  = extractTrainingFeatureVector();
		
		// train gesture svm 
		allFeatures.push_back(feature);
		featureClassIdx.push_back(g_CurrentTrainClassID);
		g_gestureSVM.train(feature, g_CurrentTrainClassID);
		if(USE_PRE_SVM)
		{
			g_PreGestureSVM.train(feature,1);
		}
	}
	
	//do parameter search via cross validation
	if(DO_SVM_PARAM_SEARCH)	
	{	
	   g_gestureSVM.doParameterSearch(-5,  15, 2.,	-15, 3, 2., 5);
	}
		
	//do retraining with same data for better prob values
	for(int i = 0; i < TRAINING_LOOPS-1; i++)
	{
		for(unsigned int k = 0; k < allFeatures.size();k++)
		{
			g_gestureSVM.train(allFeatures.at(k), featureClassIdx.at(k));
		}
	}

	g_gestureSVM.doCrossValidation(5,true);
	//generate model with optimized gamma and C
	g_gestureSVM.generateModel(); 

	g_gestureSVM.saveModel(SVM_MODEL_FILE);
	
	if(USE_PRE_SVM)
	{
		g_PreGestureSVM.doParameterSearch(-5,  15, 2.,-15, 3, 2., 5);
		for(int i = 0; i < TRAINING_LOOPS-1; i++)
		{
			for(unsigned int k = 0; k < allFeatures.size();k++)
			{
				g_PreGestureSVM.train(allFeatures.at(k), 1);
			}
		}
		g_PreGestureSVM.generateModel();
		g_PreGestureSVM.saveModel(SVM_PRE_MODEL_FILE);	
	}
}

void doQuery()
{
	//clock_t tQ;
	//tQ = clock();
	PredictionResult result;
	int numberWindows = sizeof(BUFFER_WINDOWS) / sizeof(double);
	double maxProb = -2;
	int maxClass = -1;
	float minDistance = 100000;
	for(int i = 0; i < numberWindows;i++)
	{
#ifdef DEBUG_FLAG
		printf("**BufferWindows: %f\n",BUFFER_WINDOWS[i]);
#endif
		std::vector<float> feature  = extractWindowedFeatureVectorFromBuffer(BUFFER_SIZE * BUFFER_WINDOWS[i]);

		if(g_UseDTW_NN)
		{
			GlobalFeature* resultFeature = g_dtw.getNextNeighbour(feature);
			if(resultFeature != 0 && resultFeature->distance < minDistance)
			{
				minDistance = resultFeature->distance;
				maxClass = resultFeature->classLabel;
			}
		}
		else
		{
					// check if gesture is classified as class at all
					result.classID = 1;
					if(USE_PRE_SVM)
					{
						result = g_PreGestureSVM.predictGesture(feature);
			#ifdef DEBUG_FLAG
						printf("Pre-Predicted(buffer_window: %f) as: %d\n", BUFFER_WINDOWS[i], result.classID);
			#endif
					}
					// if classID > 0 gesture passed pre svm classification, now predicted gesture in multi class svm
					if(result.classID > 0)
					{
						result = g_gestureSVM.predictGesture(feature);	
						if(result.probabilitie >= maxProb)
						{
							maxProb = result.probabilitie;
							maxClass = result.classID;
						}
			#ifdef DEBUG_FLAG
						printf("Predicted as Class (buffer_window: %f) : %d with probability: %f\n\n\n", BUFFER_WINDOWS[i], result.classID, result.probabilitie);
			#endif
					}
		}
	}
	// set the class with the highes probabiltie as predictedClass
	g_lastPredictedGestures.push(maxClass);
	
	if(USE_LAST_PREDICTED_GESTURE_HEURISTIC)
	{
		if(g_lastPredictedGestures.isFull())
		{
			std::vector<int> histo (gestureNames.size(),0);
			int maxCount = 0;
			g_lastPredictedGestures.resetIterator();
			for (int i = 0; i < g_lastPredictedGestures.getSize(); i++) {
				int c = *g_lastPredictedGestures.next();
				histo[c] = histo[c] +1;
			}
			for (int i = 1; i < gestureNames.size();i++)
			{
				if(histo[i] > maxCount)
				{	
					maxCount = histo[i];
					maxClass = i;
				}
			}
		}
	}
#ifdef DEBUG_ALL					
		printf("\t\t %d \t X: %.5f, Y: %.5f, Z: %.5f\n", i, p->X, p->Y, p->Z);
#endif
	
	g_predictedClass = maxClass;
	
	//tQ = clock() - tQ;
	//printf ("*** SVM Querry neeeds %d clicks (%f seconds).\n",tQ,((float)tQ)/CLOCKS_PER_SEC);
}

//method is used to to evaluation/debug tests..gets called with the argument -test
void runTest()
{
		g_IsTrainMode = true;
	/*
		define the set of gestures to be trained. possible gestures are find by the makros starting with
		'GESTURE_'. the value correspondes to the gesture name in the db.
	*/
	std::vector<string> gestureToTrain;
	gestureToTrain.push_back(GESTURE_SWIPE);
	gestureToTrain.push_back(GESTURE_PUSH);
	gestureToTrain.push_back(GESTURE_L);
	gestureToTrain.push_back(GESTURE_O);
	gestureToTrain.push_back(GESTURE_Z);
	Datasource d;

	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str());
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;

	std::vector<std::vector<float> > allFeatures;
	std::vector<int> featureClassIdx;
	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{

		g_CurrentTrainClassID = (*iter)->getGestureId();  

		//clear current training list (buffer)
		g_pointList4Training.clear();
		g_pointList4Training = (*iter)->getHandPoints();
		std::vector<float> feature  = extractTrainingFeatureVector();
		
		// train gesture svm 
		allFeatures.push_back(feature);
		featureClassIdx.push_back(g_CurrentTrainClassID);
		g_gestureSVM.train(feature, g_CurrentTrainClassID);
	}
	
	g_gestureSVM.doCrossValidation(5,true);

}

// detection query from DB
void queryWithTrainingData()
{
	std::vector<string> gestureToTrain;
	gestureToTrain.push_back(GESTURE_SWIPE);
	gestureToTrain.push_back(GESTURE_PUSH);
	gestureToTrain.push_back(GESTURE_L);
	gestureToTrain.push_back(GESTURE_Z);
	gestureToTrain.push_back(GESTURE_O);

	Datasource d;

	std::vector<OniFileDataSet*> oniFiles;
	for(std::vector<string>::iterator iter = gestureToTrain.begin(); iter != gestureToTrain.end();++iter)
	{
		std::vector<OniFileDataSet*> oniFilesTemp = d.getOniFileDatasetsByGesture((char*)iter->c_str());
		oniFiles.insert( oniFiles.end(), oniFilesTemp.begin(), oniFilesTemp.end());
	}


	std::vector<OniFileDataSet*>::iterator iter;

	for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
#ifdef DEBUG_FLAG
		std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
		std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
		std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;
		std::cout << "oniFileID: " << (*iter)->getFileId() << std::endl;
#endif
		g_pointList4Training.clear();
		g_pointList4Training = (*iter)->getHandPoints();

		std::vector<float> feature  = extractTrainingFeatureVector();

		PredictionResult result = g_gestureSVM.predictGesture(feature);
#ifdef DEBUG_FLAG
		printf("Predicted as Class : %d with probability: %f\n\n",result.classID, result.probabilitie);
#endif
	}
}
