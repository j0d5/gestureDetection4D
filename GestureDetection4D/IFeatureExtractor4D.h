#pragma once
#include <vector>

struct Point3D
{
	float X;
	float Y;
	float Z;
};

class IFeatureExtractor4D
{
public:
	virtual std::vector<float> getFeatureVector(std::vector<Point3D> points) = 0;
};

