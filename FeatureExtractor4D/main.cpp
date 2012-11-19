// FeatureExtractor4D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleFeatureExtractor.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Point3D> input;
	input.push_back(Point3D(1,1,1));
	input.push_back(Point3D(2,2,2));
	input.push_back(Point3D(3,3,3));
	input.push_back(Point3D(4,4,4));
	input.push_back(Point3D(5,5,5));
	input.push_back(Point3D(6,6,6));
	input.push_back(Point3D(7,7,7));
	input.push_back(Point3D(8,8,8));
	input.push_back(Point3D(9,9,9));
	input.push_back(Point3D(10,10,10));
	input.push_back(Point3D(11,11,11));
	input.push_back(Point3D(12,12,12));
	input.push_back(Point3D(13,13,13));
	input.push_back(Point3D(14,14,14));
	input.push_back(Point3D(15,15,15));
	input.push_back(Point3D(16,16,16));
	input.push_back(Point3D(17,17,17));
	input.push_back(Point3D(18,18,18));
	input.push_back(Point3D(19,19,19));

	IFeatureExtractor4D* extractor = new SimpleFeatureExtractor();

	std::vector<float> feature = extractor->getFeatureVector(input);

	std::vector<float>::iterator iter;

	for(int i = 0; i < feature.size();i++)
	{
		printf("X: %f \n",feature[i]);
		
	}
	printf("end..");
	getchar();
	return 0;
}

