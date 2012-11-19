#include "StdAfx.h"
#include "SimpleFeatureExtractor.h"

#define FEATURE_VECTOR_SIZE 25

SimpleFeatureExtractor::SimpleFeatureExtractor(void)
{
}


SimpleFeatureExtractor::~SimpleFeatureExtractor(void)
{
}

/*
/	This method normalizes a vector 
/
*/
std::vector<Point3D> SimpleFeatureExtractor::normalizeVector(std::vector<Point3D> points) 
{
	std::vector<Point3D> normalizedPoints(FEATURE_VECTOR_SIZE);
	
	// more points than needed 
	if (points.size() > FEATURE_VECTOR_SIZE) {
		int numOfCuts = FEATURE_VECTOR_SIZE - 2 + 1;
		int stepSize = (points.size() - 2) / numOfCuts;
		int currentCut = 0;
		int leftover = (points.size() - 2) % numOfCuts;

		normalizedPoints[0] = points[0];
		
		std::vector<Point3D>::iterator iter = normalizedPoints.begin();
		++iter;

		for (int i = stepSize; i < points.size() - 1; i += stepSize) {
			++currentCut;
			if(currentCut >= (numOfCuts - leftover - 1)) {
				*iter++ = points[i++];

			} else {
				*iter++ = points[i];
			}
		}

		normalizedPoints[FEATURE_VECTOR_SIZE - 1] = points[points.size() - 1];
	}

	return normalizedPoints;
}

std::vector<float> SimpleFeatureExtractor::getFeatureVector(std::vector<Point3D> points)
{
	if(points.empty()) {
		printf("Empty point list!!!");
	}

	points = normalizeVector(points);

	std::vector<Point3D>::iterator itr;
	//Point3D offsetOrigin = points.at(0);
	std::vector<float> featureVec;
	for (itr = points.begin(); itr != points.end()-1; itr++ )
	{
		Point3D diffVec = (*(itr+1) - *itr);
		featureVec.push_back(diffVec.X);
		featureVec.push_back(diffVec.Y);
		featureVec.push_back(diffVec.Z);
	}



	return featureVec;

}