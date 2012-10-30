// FeatureExtractor4D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleFeatureExtractor.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Point3D> input;
	input.push_back(Point3D(1,1,1));
	input.push_back(Point3D(3,2,1));
	input.push_back(Point3D(4,5,1));
	IFeatureExtractor4D* extractor = new SimpleFeatureExtractor();

	std::vector<float> feature = extractor->getFeatureVector(input);

	std::vector<float>::iterator iter;
	for(iter = feature.begin(); iter != feature.end();iter+=3)
	{
		printf("X: %f, Y:%f, Z:%f",*iter,*(iter+1),*(iter+2));
		
	}
	printf("end..");
	getchar();
	return 0;
}

