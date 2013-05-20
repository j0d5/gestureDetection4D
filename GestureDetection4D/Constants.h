#pragma once

// paths
#define WINDOW_NAME "GestureDetection4D"
#define SVM_MODEL_FILE "../../SVM_ModelDTW.txt"
#define SVM_PRE_MODEL_FILE "../../SVM_Pre_ModelDTW.txt"
#define SAMPLE_XML_PATH "../../Sample-Tracking.xml" // xml to initialize OpenNI

#define USE_HAND_POINTS_FROM_DB 1	// use hand points from database

// FeatureExtractor settings
#define NORMALIZE_FEATURE_VECTOR 0	// normalize featureVector data
#define FEATURE_VECTOR_SIZE 5		// simpleFeatureVector size
#define FEATURE_VECTOR_FREQUENCY 1	// query only every n-th hand update

/*SecondSimpleFeatureExtractor settings*/
#define NUMBER_OF_PARTS 5

// SVM Settings
#define USE_PRE_SVM 0				// use one class prediction for not a gesture class
#define TRAINING_LOOPS 1			// how often the data from DB will be trained
#define DO_SVM_PARAM_SEARCH	1		// do svm parameter search

// Buffer Settings
#define BUFFER_SIZE 100				// buffer size
//const double BUFFER_WINDOWS[] = {0.3,0.4,0.5,0.6,0.8,1.}; // extraction windows 0.1 - 1.0
//const double BUFFER_WINDOWS[] = {0.2,0.4,0.8,1.}; // extraction windows 0.1 - 1.0
const double BUFFER_WINDOWS[] = {0.5}; // extraction windows 0.1 - 1.0

//Last Predicted Gesture buffer
#define LAST_PREDICTED_GESTURE_BUFFER_SIZE 6
#define USE_LAST_PREDICTED_GESTURE_HEURISTIC 1
// Gestures used for training mode
#define GESTURE_SWIPE "Swipe"
#define GESTURE_PUSH "Push"
#define GESTURE_L "Letter L"
#define GESTURE_Z "Letter Z"
#define GESTURE_O "Letter O"
#define GESTURE_NO "NO_GESTURE"
