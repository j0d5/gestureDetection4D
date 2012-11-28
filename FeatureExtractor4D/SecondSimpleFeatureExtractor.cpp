#include "StdAfx.h"
#include "SecondSimpleFeatureExtractor.h"


#define NUMBER_OF_PARTS 3


std::vector<float> SecondSimpleFeatureExtractor::getFeatureVector(std::vector<Point3D> points) {

	if(points.empty()) {
		printf("Empty point list!!!");
		// return std::vector<float>();
	}

	// differenz vektor
	std::vector<Point3D> differenceVectors;
	std::vector<Point3D>::iterator itr;
	for (itr = points.begin(); itr != points.end()-1; itr++ )
	{
		Point3D diffVec = (*(itr+1) - *itr);
		differenceVectors.push_back(diffVec);
	}

	std::vector<float> featureVector;
	float sumX, sumY, sumZ = 0;
	int length = differenceVectors.size();
	int numOfCuts = (length / NUMBER_OF_PARTS) - 1;
	int sizeOfBlock = length / NUMBER_OF_PARTS;
	
	for(int i = 0; i < NUMBER_OF_PARTS; ++i) {
		sumX = 0;
		sumY = 0;
		sumZ = 0;

		//last block
		if(i == NUMBER_OF_PARTS - 1) {
			for(int j = i * sizeOfBlock; j < length; ++j) {
				sumX += abs(differenceVectors[j].X);
				sumY += abs(differenceVectors[j].Y);
				sumZ += abs(differenceVectors[j].Z);
			}
		} else {

			for(int j = i * sizeOfBlock; j < (i+1)*sizeOfBlock;++j) {
				sumX += abs(differenceVectors[j].X);
				sumY += abs(differenceVectors[j].Y);
				sumZ += abs(differenceVectors[j].Z);
			}
		}

		featureVector.push_back(sumX);
		featureVector.push_back(sumY);
		featureVector.push_back(sumZ);
	}

	return featureVector;
}
