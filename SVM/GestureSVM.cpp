#include "GestureSVM.h"

void convertToSVMVec(const std::vector<float>& vec, svm_node* destination)
{
		svm_node node;
		for(int i = 0;i < vec.size();i++)
		{
			node.index = i+1;
			node.value = (double) vec.at(i);
			destination[i] = node;
		}
		node.index =-1;
		destination[vec.size()] = node;		
}
void GestureSVM::train(const std::vector<float>& feature,int classId)
{
	svm_node* vec = new svm_node[feature.size()+1];
	convertToSVMVec(feature,vec);
	mFeatureSet.push_back(vec);
	mClassIdx.push_back(classId);
}
int GestureSVM::predictGesture(const std::vector<float>& feature)
{
	svm_node* preVec = new svm_node[feature.size()+1]; 
	convertToSVMVec(feature,preVec);
	int result = (int) svm_predict(mModel,preVec);
	delete[] preVec;
	return result;
}
void GestureSVM::generateModel()
{
	//init svm for trainings
	mProblem.l = mFeatureSet.size();		//number of trainings data (2 training vectors here
	mProblem.y = mClassIdx.data();		

	//build svm structure of trainings data		
	mProblem.x = mFeatureSet.data();

	//set default values
	mParam.svm_type = C_SVC;
	mParam.kernel_type = RBF;
	mParam.degree = 3;
	mParam.gamma = 0;	// 1/num_features
	mParam.coef0 = 0;
	mParam.nu = 0.5;
	mParam.cache_size = 100;
	mParam.C = 1;
	mParam.eps = 1e-3;
	mParam.p = 0.1;
	mParam.shrinking = 1;
	mParam.probability = 0;
	mParam.nr_weight = 0;
	mParam.weight_label = NULL;
	mParam.weight = NULL;

	const char* error_msg = svm_check_parameter(&mProblem,&mParam);

	if(error_msg)
	{
		fprintf(stderr,"ERROR: %s\n",error_msg);
		exit(1);
	}
	
	mModel = svm_train(&mProblem,&mParam);

}
GestureSVM::GestureSVM(void)
{
}


GestureSVM::~GestureSVM(void)
{

	svm_free_and_destroy_model(&mModel);
	svm_destroy_param(&mParam);
		
	std::vector<svm_node*>::iterator iter;
	for(iter = mFeatureSet.begin(); iter != mFeatureSet.end();iter++)
	{
		delete[] *iter;
	}
}
