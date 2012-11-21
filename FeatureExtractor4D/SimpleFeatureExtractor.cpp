#include "StdAfx.h"
#include "SimpleFeatureExtractor.h"



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
	//todo: if points.size() <= FEATURE_VECTOR_SIZE extrapolate point data to get valid feature vector

	return normalizedPoints;
}

std::vector<float> SimpleFeatureExtractor::getFeatureVector(std::vector<Point3D> points)
{
	std::vector<Point3D>::iterator itr;

	if(points.empty()) {
		printf("Empty point list!!!");
	}

	points = normalizeVector(points);

	// summe der beträge der punkte
	float SumAbsVec = 0.0;
	//Point3D offsetOrigin = points.at(0);
	
	std::vector<Point3D> differenceVectors;
	for (itr = points.begin(); itr != points.end()-1; itr++ )
	{
		Point3D diffVec = (*(itr+1) - *itr);
		differenceVectors.push_back(diffVec);
		SumAbsVec += diffVec.norm();
	}

	std::vector<float> featureVec; 
	//normalisierung
	for (itr = differenceVectors.begin(); itr != differenceVectors.end(); itr++)
	{
		
		float scale =1.0;
		if(NORMALIZE_FEATURE_VECTOR)	
		{
			scale = (itr->norm() / SumAbsVec) * (1 / itr->norm());
		}
		featureVec.push_back(itr->X * scale);
		featureVec.push_back(itr->Y * scale);
		featureVec.push_back(itr->Z * scale);
	}

	return featureVec;

}