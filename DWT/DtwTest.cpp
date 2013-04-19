// DWT.cpp : Defines the entry point for the console application.
//
/*
#include "dtw.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
	 DTW dtw;

           cout << "Vector DTW Test" << endl;
           std::vector<LocalFeature> mainVec;
           std::vector<LocalFeature> testVec;
           LocalFeature p1(1, 2, 3);
           LocalFeature p2(2, 3, 4);

           LocalFeature p3(3, 2, 3);
           LocalFeature p4(2, 2, 3);

           mainVec.push_back(p1);
           mainVec.push_back(p2);
           mainVec.push_back(p4);

           testVec.push_back(p3);
           testVec.push_back(p4);

           GlobalFeature global(1,mainVec);
           dtw.addTrainingFeature(global);

           GlobalFeature result = dtw.getNextNeighbour(testVec);

           cout << "Distance: " << result.distance << endl;
           cout << "Label: " << result.classLabel << endl;

           system("PAUSE");
	return 0;
}
*/
