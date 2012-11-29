#pragma once

// paths
#define WINDOW_NAME "GestureDetection4D"
#define SVM_MODEL_FILE "../../SVM_Model.txt"
#define SVM_PRE_MODEL_FILE "../../SVM_Pre_Model.txt"
#define SAMPLE_XML_PATH "../../Sample-Tracking.xml" // xml to initialize OpenNI

// general settings
#define DEBUG_FLAG					// add debugging information
//#define DEBUG_ALL
#define USE_HAND_POINTS_FROM_DB 1	// use hand points from database

// FeatureExtractor settings
#define NORMALIZE_FEATURE_VECTOR 0	// normalize featureVector data
#define FEATURE_VECTOR_SIZE 5		// simpleFeatureVector size
#define FEATURE_VECTOR_FREQUENCY 1	// vector generation frequency

/*SecondSimpleFeatureExtractor settings*/
#define NUMBER_OF_PARTS 4

// SVM Settings
#define USE_PRE_SVM 1				// use one class prediction for not a gesture class
#define TRAINING_LOOPS 5			// how often the data from DB will be trained

// Buffer Settings
#define BUFFER_SIZE 120				// buffer size
const double BUFFER_WINDOWS[] = {0.3,0.4,0.5,0.6,0.8,1.}; // extraction windows 0.1 - 1.0

// Gestures used for training mode
#define GESTURE_SWIPE "Swipe"
#define GESTURE_PUSH "Push"
#define GESTURE_L "Letter L"
#define GESTURE_Z "Letter Z"
#define GESTURE_O "Letter O"
#define GESTURE_NO "NO_GESTURE"
