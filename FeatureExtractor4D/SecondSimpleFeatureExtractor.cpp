#include "StdAfx.h"
#include "SecondSimpleFeatureExtractor.h"


#define NUMBER_OF_PARTS 3


std::vector<float> SecondSimpleFeatureExtractor::getFeatureVector(std::vector<Point3D> points) {

	int length = points.size();
	int numOfCuts = (length / NUMBER_OF_PARTS) - 1;
	int sizeOfBlock = length / NUMBER_OF_PARTS;

	std::vector<float> featureVector;

	float sumX, sumY, sumZ = 0;

	for(int i = 0; i < NUMBER_OF_PARTS; ++i) {

		

		sumX = 0;
		sumY = 0;
		sumZ = 0;

		//last block
		if(i == NUMBER_OF_PARTS - 1) {
			for(int j = i * sizeOfBlock; j < length; ++j) {
				sumX += points[j].X;
				sumY += points[j].Y;
				sumZ += points[j].Z;
			}
		} else {

			for(int j = i * sizeOfBlock; j < (i+1)*sizeOfBlock;++j) {
				sumX += points[j].X;
				sumY += points[j].Y;
				sumZ += points[j].Z;
			}
		}

		featureVector.push_back(sumX);
		featureVector.push_back(sumY);
		featureVector.push_back(sumZ);
	}

	return featureVector;
}
