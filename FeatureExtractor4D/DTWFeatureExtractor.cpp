#include "DTWFeatureExtractor.h"
#include <math.h>

DTWFeatureExtractor::DTWFeatureExtractor(void)
{
}


DTWFeatureExtractor::~DTWFeatureExtractor(void)
{
}

/**
	Feature Vector contains the local feature (x',y',z',...)
*/
std::vector<float> DTWFeatureExtractor::getFeatureVector(std::vector<Point3D> points)
{
	//calculate mean, std deviation and tangent slope angle?
	double middleX = 0.0;
	double middleY = 0.0;
	double middleZ = 0.0;
	double sigmaY = 0.0;
	double sigmaX = 0.0;
	double sigmaZ = 0.0;

	std::vector<Point3D>::iterator itr;
	for (itr = points.begin(); itr != points.end(); itr++ )
	{
		middleX+=itr->X;
		middleY+=itr->Y;
		middleZ+=itr->Z;
	}
	middleX = middleX / points.size();
	middleY = middleY / points.size();
	middleZ = middleZ / points.size();

	/*
	//calculate sigma 
	for (itr = points.begin(); itr != points.end(); itr++ )
	{
		sigmaY+= pow(middleY - itr->Y,2);
		sigmaX+= pow(middleX - itr->X,2);
		sigmaZ+= pow(middleZ - itr->Z,2);
	}
	sigmaY = sqrt(sigmaY / (points.size() -1));
	sigmaX = sqrt(sigmaX / (points.size() -1));
	sigmaZ = sqrt(sigmaZ / (points.size() -1));*/



	//build feature vector
	std::vector<float> featureVector;
	float x;
	float y;
	float z;
	for (itr = points.begin(); itr != points.end(); itr++ )
	{
		x = (itr->X - middleX) ;
		y = (itr->Y - middleY) ;
		z = (itr->Z - middleZ) ;
		//todo: tangent slop angle

		featureVector.push_back(x);
		featureVector.push_back(y);
		featureVector.push_back(z);
	}

	return featureVector;
}