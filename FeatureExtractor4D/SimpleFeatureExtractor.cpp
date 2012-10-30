#include "StdAfx.h"
#include "SimpleFeatureExtractor.h"

SimpleFeatureExtractor::SimpleFeatureExtractor(void)
{
}


SimpleFeatureExtractor::~SimpleFeatureExtractor(void)
{
}

std::vector<float> SimpleFeatureExtractor::getFeatureVector(std::vector<Point3D> points)
{
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