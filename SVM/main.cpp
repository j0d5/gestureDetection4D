// SVM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FeatureExtractor4D/SimpleFeatureExtractor.h"
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Point3D> input;
	input.push_back(Point3D(1,1,1));
	input.push_back(Point3D(3,2,1));
	input.push_back(Point3D(4,5,1));
	IFeatureExtractor4D* extractor = new SimpleFeatureExtractor();

	std::vector<float> feature = extractor->getFeatureVector(input);

	return 0;
}

