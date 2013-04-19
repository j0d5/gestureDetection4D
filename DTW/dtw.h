#ifndef DTW_H
#define DTW_H

#endif // DTW_H

#include <vector>
#include <math.h>

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

   inline float minOf3 (float x, float y , float z)
   {
       if(x < y)
       {
           return x < z ? x : z;
       }
       else //x is greater y
       {
           return y < z ? y : z;
       }
   }

public:

  DTW () : D(INIT_SIZE_OF_D,std::vector<float>(INIT_SIZE_OF_D,INF)) {
    D[0][0] = 0;
   }

    /*
        callculates the dtw distance in O(n^2)
      */
    inline float getDTWDistance(std::vector<LocalFeature>& t, std::vector<LocalFeature>& r)
    {
        for(int i = 1; i < t.size();i++)
        {
            for(int j = 1; j < r.size();j++)
            {
                D[i][j] = t[i].distance(r[j]) + minOf3 (D[i-1][j],D[i][j-1],D[i-1][j-1]);
            }
        }
        return D[t.size()-1][r.size()-1];
    }

    void addTrainingFeature(GlobalFeature* g)
    {
        trainingSet.push_back(g);
    }

	GlobalFeature* getNextNeighbour(std::vector<float>& feature)
	{
		std::vector<LocalFeature> localFeatures;
		for(int i =0; i < feature.size(); i+=3)
		{
			localFeatures.push_back(LocalFeature(feature[i],feature[i+1],feature[i+2]));		
		}
		return this->getNextNeighbour(localFeatures);
	}
    GlobalFeature* getNextNeighbour(std::vector<LocalFeature>& test)
    {
		assert(!trainingSet.empty());

        float minDistance = INF;
        std::vector<GlobalFeature*>::iterator iter;
        std::vector<GlobalFeature*>::iterator iterNN;

        for(iter = trainingSet.begin();iter != trainingSet.end();iter++)
        {
            float currentDistance = this->getDTWDistance(test,(*iter)->localFeatures);
            if( currentDistance < minDistance )
            {
                    iterNN = iter;
                    minDistance = currentDistance;
            }
        }
		if(minDistance < INF)
		{
			(*iterNN)->distance = minDistance;
			return *iterNN;
		}
		return 0;
    }
	void addTrainingFeatureFromRawData(std::vector<float>& feature, int classID)
	{
		GlobalFeature* f = new GlobalFeature(classID);
		
		for(int i =0; i < feature.size(); i+=3)
		{
			f->localFeatures.push_back(LocalFeature(feature[i],feature[i+1],feature[i+2]));		
		}
		trainingSet.push_back(f);
	}
};
