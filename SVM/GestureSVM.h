#pragma once
#include "svm.h"
#include <vector>
#include <string>

struct PredictionResult
{
    int classID;
	int classIDWithoutProb;
    float probabilitie;
	PredictionResult(int classID = -42, int classIDWithoutProb = -42, float probabilitie = -1):
	classID(classID),probabilitie(probabilitie),classIDWithoutProb(classIDWithoutProb) {}
};

class GestureSVM
{
private:
	std::vector<svm_node*> mFeatureSet;
	std::vector<double> mClassIdx;
	
	svm_problem mProblem;
	//svm parameters
	svm_parameter mParam;
	//svm model
	svm_model* mModel;

    bool mIsModel;
	bool mIsInit;
	void initProblemAndParam();
	


public:
	//adds one feature vector with given class id to svm model
	void train(const std::vector<float>& feature,int classId);
	
	void doParameterSearch(int c_begin, int c_end, int c_step,int g_begin, int g_end, int g_step, int folds);
	double doCrossValidation(int folds);
	//finishes trainings mode an generates a svm model
	void generateModel();

    /*
        classifies a given feature vector, returns a PredictionResult
        predeiction is only possible if model is allready generated!
     */
    PredictionResult predictGesture(const std::vector<float>& feature);

    //save model to file
    void saveModel(std::string filePath);

    //load model from file (implies genearteModel!)
    void loadModel(std::string filePath);
	
	GestureSVM(bool isOneClassSVM = false);
	~GestureSVM(void);
};

