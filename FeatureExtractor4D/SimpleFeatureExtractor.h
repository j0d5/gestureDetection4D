#ifndef SIMPLEFEATUREEXTRACTOR_H_
#define SIMPLEFEATUREEXTRACTOR_H_

#include "IFeatureExtractor4D.h"

class SimpleFeatureExtractor :
	public IFeatureExtractor4D
{
public:
	std::vector<float> getFeatureVector(std::vector<Point3D> points);
	SimpleFeatureExtractor(void);
	virtual ~SimpleFeatureExtractor(void);
};

#endif
