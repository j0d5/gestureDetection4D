#pragma once

#include "stdafx.h"
#include <vector>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>

static const int INIT_SIZE_OF_D = 1000;
static const float INF = 1000000;

/**
DTW Class provides dtw distance calculation with dynamic programming in O(n*m) for to sequences
of local Features with length n,m. Furthermore provides simple implementation of NextNeighbour
classification based on the dtw distance.

by Johannes Wetzel
*/


/*
  a LocalFeature is an element of a time sequenze..if u wish to have a higher
  dimension just add it here...caution:take care of reimplementing the distance function!

*/
struct LocalFeature {
    float x;
    float y;
    float z;
    LocalFeature(float X, float Y, float Z): x(X), y(Y), z(Z) { }
    //euclidean distance
    float distance(const LocalFeature &p)
    {
       return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
    }

};
/*
    A Global Feature is a vector of local features with additional classLabel and
    optional distance (used as classification distance)
  */
struct GlobalFeature {
    int classLabel;
    float distance;
    std::vector<LocalFeature> localFeatures;

    GlobalFeature(int label,std::vector<LocalFeature> f = std::vector<LocalFeature>()) :
        localFeatures(f), classLabel(label) {}

};


class DTW {

private: 
   //viterbi-matrix
   std::vector<std::vector<float> > D;

   //training data
   std::vector<GlobalFeature*> trainingSet;

   inline float minOf3 (float x, float y , float z);
   inline int getPathLength(int startI, int startJ);

public:
   DTW ();
  ~DTW ();

    /*
        callculates the dtw distance in O(n^2)
      */
    inline float getDTWDistance(std::vector<LocalFeature>& t, std::vector<LocalFeature>& r);
   
	//not implemented yet..
	inline float getFastDTWDistance(std::vector<LocalFeature>& t, std::vector<LocalFeature>& r);
	GlobalFeature* getNextNeighbour(std::vector<float>& feature);
    GlobalFeature* getNextNeighbour(std::vector<LocalFeature>& test);

	void addTrainingFeatureFromRawData(std::vector<float>& feature, int classID);
    void addTrainingFeature(GlobalFeature* g);
	/**
		Subdivides the given trainingSet into n Folds and performse the cross validation.
		return cv accurancs in percent
	*/
	float doCrossValidation(int nFolds);
};
