
#include "IFeatureExtractor4D.h"

#pragma once
class SimpleFeatureExtractor :
	public IFeatureExtractor4D
{
public:
	std::vector<float> getFeatureVector(std::vector<Point3D> points);
	SimpleFeatureExtractor(void);
	virtual ~SimpleFeatureExtractor(void);
};

