#include "StdAfx.h"
#include "SimpleFeatureExtractor.h"
/*
const Point3D operator-(Point3D const& v1, Point3D const& v2)
{
	Point3D result;
	result.X = v1.X - v2.X;
	result.Y = v1.Y - v2.Y;
	result.Z = v1.Z - v2.Z;
	return result;
};*/
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