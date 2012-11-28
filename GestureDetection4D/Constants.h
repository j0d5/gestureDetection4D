#pragma once

#define BUFFER_SIZE 120

//use every n-th hand update
#define FEATURE_VECTOR_FREQUENCY 1
#define WINDOW_NAME "GestureDetection4D"
#define SVM_MODEL_FILE "../../SVM_Model.txt"
#define SVM_PRE_MODEL_FILE "../../SVM_Pre_Model.txt"
//#define DEBUG_FLAG
#define USE_PRE_SVM 0
#define USED_PRE_FRAMES 0
#define USE_HAND_POINTS_FROM_DB 1

/*FeatureExtractor settings*/
#define NORMALIZE_FEATURE_VECTOR 0
#define FEATURE_VECTOR_SIZE 5

/*SecondSimpleFeatureExtractor settings*/
#define NUMBER_OF_PARTS 4

const double BUFFER_WINDOWS[] = {0.3,0.4,0.5,0.6,0.8,1.};
