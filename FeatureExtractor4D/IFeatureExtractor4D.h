#ifndef IFEATUREEXTRATOR4D_H_
#define IFEATUREEXTRATOR4D_H_

#include <vector>

struct Point3D
{
	float X;
	float Y;
	float Z;
	Point3D(float x = 0, float y = 0, float z = 0) : X(x),Y(y),Z(z){};
	
	const Point3D operator-(Point3D const& v2)
	{
		Point3D result;
		result.X = this->X - v2.X;
		result.Y = this->Y - v2.Y;
		result.Z = this->Z - v2.Z;
		return result;
	};
	float norm(){
	return  sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
	};

};

class IFeatureExtractor4D
{
public:
	virtual std::vector<float> getFeatureVector(std::vector<Point3D> points) = 0;
};

#endif
