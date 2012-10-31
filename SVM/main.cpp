// SVM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FeatureExtractor4D/SimpleFeatureExtractor.h"
#include <vector>
#include "svm.h"
#include "GestureSVM.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const char *error_msg;

	std::vector<Point3D> inputClass1;
	inputClass1.push_back(Point3D(1,1,1));
	inputClass1.push_back(Point3D(5,1,1));
	inputClass1.push_back(Point3D(10,1,1));

	std::vector<Point3D> inputClass2;
	inputClass2.push_back(Point3D(1,1,1));
	inputClass2.push_back(Point3D(1,1,1));
	inputClass2.push_back(Point3D(1,1,1));

	std::vector<Point3D> toPredict;
	toPredict.push_back(Point3D(8,1,1));
	toPredict.push_back(Point3D(12,1,1));
	toPredict.push_back(Point3D(16,1,1));


	IFeatureExtractor4D* extractor = new SimpleFeatureExtractor();

	std::vector<std::vector<float>> featureSet;
	featureSet.push_back(extractor->getFeatureVector(inputClass2));
	featureSet.push_back(extractor->getFeatureVector(inputClass1));
	

	/*
	//init svm for trainings
	svm_problem problem;
	problem.l = featureSet.size();		//number of trainings data (2 training vectors here
	double classes[] = {1,2}; //classes in order of training data 
	problem.y = classes;		

	//build svm structure of trainings data	
	std::vector<svm_node*> allNodes;
	std::vector<std::vector<float>>::iterator iter;
	for(iter = featureSet.begin(); iter != featureSet.end();iter++)
	{
		svm_node* vec = new svm_node[iter->size()];
		convertToSVMVec(*iter,vec);
		
		allNodes.push_back(vec);
	}
	problem.x = allNodes.data();

	for(int i=0; i < problem.l;i++)
	{
		svm_node* vec = problem.x[i];
		for(int j = 0; j < 6 ;j++)
		{
			printf("node: (%d,%f) \t",vec[j].index,vec[j].value);
		}
		printf("\n\n");
	}
	//svm parameters
	svm_parameter param;
	
	//set default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	error_msg = svm_check_parameter(&problem,&param);

	if(error_msg)
	{
		fprintf(stderr,"ERROR: %s\n",error_msg);
		exit(1);
	}
	
	svm_model* model = svm_train(&problem,&param);
	
	
	//prediction
	std::vector<float> featurePredict = extractor->getFeatureVector(toPredict);
	svm_node* preVec = new svm_node[featurePredict.size()]; 
	convertToSVMVec(featurePredict,preVec);
	double predictedClass = svm_predict(model,preVec);
	*/

	GestureSVM svm;

	std::vector<std::vector<float>> featuresClass1;
	std::vector<std::vector<float>> featuresClass2;
	
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
	
	std::vector<std::vector<float>>::iterator iter;
	
	for(iter = featuresClass1.begin(); iter != featuresClass1.end();iter++)
	{
		svm.train(*iter,1);
	}

	for(iter = featuresClass2.begin(); iter != featuresClass2.end();iter++)
	{
		svm.train(*iter,2);
	}

	svm.generateModel();

	std::vector<float> predict;
	predict.push_back(1.5);
	predict.push_back(3.2);

	double predictedClass = svm.predictGesture(predict);
	printf("Predicted as Class %f",predictedClass);
	
	getchar();

	return 0;
}

