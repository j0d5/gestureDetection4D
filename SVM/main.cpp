// SVM.cpp : Defines the entry point for the console application.
//
#include <vector>
#include "svm.h"
#include "GestureSVM.h"

int main(int argc, char* argv[])
{
	GestureSVM svm;

    std::vector<std::vector<float> > featuresClass1;
    std::vector<std::vector<float> > featuresClass2;

    std::vector<float> c1;
    c1.push_back(0.1);
    c1.push_back(2.2);
    featuresClass1.push_back(c1);
    c1.clear();

    c1.push_back(0.2);
    c1.push_back(2.5);
    featuresClass1.push_back(c1);
    c1.clear();

    c1.push_back(0.15);
    c1.push_back(2.3);
    featuresClass1.push_back(c1);
    c1.clear();

    //class 2
    std::vector<float> c2;
    c2.push_back(3.1);
    c2.push_back(0.2);
    featuresClass2.push_back(c2);
    c2.clear();

    c2.push_back(3.2);
    c2.push_back(0.5);
    featuresClass2.push_back(c2);
    c2.clear();

    c2.push_back(3.15);
    c2.push_back(0.3);
    featuresClass2.push_back(c2);
    c2.clear();

    //svm.train(extractor->getFeatureVector(inputClass1),1);
    //svm.train(extractor->getFeatureVector(inputClass1),2);

    std::vector<std::vector<float> >::iterator iter;

  
    for(iter = featuresClass1.begin(); iter != featuresClass1.end();iter++)
    {
        svm.train(*iter,1);

    }

    for(iter = featuresClass2.begin(); iter != featuresClass2.end();iter++)
    {
        svm.train(*iter,2);
    }

    svm.generateModel();
    svm.saveModel(std::string("model.txt"));
    svm.loadModel(std::string("model.txt"));

    std::vector<float> predict;
    predict.push_back(2.15);
    predict.push_back(0.3);


    PredictionResult result = svm.predictGesture(predict);
    printf("Predicted as Class %d with probabilite %f\n",result.classID,result.probabilitie);


     getchar();

    return 0;
}

