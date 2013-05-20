#include "stdafx.h"
#include "dtw.h"

   inline float DTW::minOf3 (float x, float y , float z)
   {
       if(x < y)
       {
           return x < z ? x : z;
       }
       else //x is greater y
       {
           return y < z ? y : z;
       }
   }


  DTW::DTW () : D(INIT_SIZE_OF_D,std::vector<float>(INIT_SIZE_OF_D,INF)) {
    D[0][0] = 0;
   }

  DTW::~DTW ()
  {
	  std::vector<GlobalFeature*>::iterator iter;
	  for(iter = trainingSet.begin(); iter != trainingSet.end();iter++)  
	  {
		  delete *iter;
	  }
  }

  inline static int minIndexOf3 (float x, float y , float z)
   {
       if(x < y)
       {
           return x < z ? 1 : 3;
       }
       else //x is greater y
       {
           return y < z ? 2 : 3;
       }
   }
  inline int DTW::getPathLength(int startI, int startJ)
  {
	  int i = startI;
	  int j = startJ;
	  int minIdx;
	  int length = 0;
	  while ( i != 1 || j !=1 )
	  {
		  if(i == 1 && j > 1)
			j--;
		  else if(j == 1 && i > 1)
			i--;
		  else
		  {
			  minIdx = minIndexOf3(D[i-1][j],D[i][j-1],D[i-1][j-1]);
			  if(minIdx == 1)
				  i--;
			  else if(minIdx == 2)
				  j--;
			  else if(minIdx == 3)
			  {
				  i--;
				  j--;
			  }
		  }
		  length++;
	  }
	  //printf("DTW Path length for n = %d,m = %d : %d \n",startI,startJ,length);
	  return length;
  }
  /*
   Todo: Implement DTW Beam Search here...

   */
  inline float getFastDTWDistance(std::vector<LocalFeature>& t, std::vector<LocalFeature>& r)
  {

	  for(int i = 1; i < t.size();i++)
      {



	  }

	  return -1.;
  }
    /*
        callculates the dtw distance in O(n^2)
      */
    inline float DTW::getDTWDistance(std::vector<LocalFeature>& r,std::vector<LocalFeature>& t)
    {
        for(int i = 1; i < t.size();i++)
        {
            for(int j = 1; j < r.size();j++)
            {
                D[i][j] = t[i].distance(r[j]) + minOf3 (D[i-1][j],D[i][j-1],D[i-1][j-1]);
            }
        }
		//return D[t.size()-1][r.size()-1] / t.size();
		return D[t.size()-1][r.size()-1] / this->getPathLength(t.size()-1,r.size()-1);
    }

    void DTW::addTrainingFeature(GlobalFeature* g)
    {
        trainingSet.push_back(g);
    }

	GlobalFeature* DTW::getNextNeighbour(std::vector<float>& feature)
	{
		std::vector<LocalFeature> localFeatures;
		for(int i =0; i < feature.size(); i+=3)
		{
			localFeatures.push_back(LocalFeature(feature[i],feature[i+1],feature[i+2]));		
		}
		return this->getNextNeighbour(localFeatures);
	}
    GlobalFeature* DTW::getNextNeighbour(std::vector<LocalFeature>& test)
    {
		assert(!trainingSet.empty());

        float minDistance = INF;
        std::vector<GlobalFeature*>::iterator iter;
        std::vector<GlobalFeature*>::iterator iterNN;

        for(iter = trainingSet.begin();iter != trainingSet.end();iter++)
        {
            float currentDistance = this->getDTWDistance((*iter)->localFeatures,test);
            if( currentDistance < minDistance )
            {
                    iterNN = iter;
                    minDistance = currentDistance;
            }
        }
		if(minDistance < INF)
		{
			(*iterNN)->distance = minDistance;
			return *iterNN;
		}
		return 0;
    }
	void DTW::addTrainingFeatureFromRawData(std::vector<float>& feature, int classID)
	{
		GlobalFeature* f = new GlobalFeature(classID);
		
		for(int i =0; i < feature.size(); i+=3)
		{
			f->localFeatures.push_back(LocalFeature(feature[i],feature[i+1],feature[i+2]));		
		}
		trainingSet.push_back(f);
	}
	// random generator function:
    int myrandom (int i) { return std::rand()%i;}	
	/**
		Subdivides the given trainingSet into n Folds and performse the cross validation.
		return cv accurancs in percent
	*/
	
	float DTW::doCrossValidation(int nFolds)
	{
		std::srand ( unsigned ( std::time(0) ) );
		std::random_shuffle(trainingSet.begin(),trainingSet.end(),myrandom);
		std::vector<GlobalFeature*> allFeatures(trainingSet);
		int step = allFeatures.size() / nFolds;
			
		std::vector<GlobalFeature*> testFold;
		std::vector<GlobalFeature*>::iterator iter;
		
		int correct = 0;
		for(int i = 0;i < nFolds;i++)
		{
			iter = trainingSet.begin() +  i*step;	
			testFold.insert(testFold.begin(),iter,iter + step);
			trainingSet.erase(iter,iter + step);

			//do cross validation		
			for(iter = testFold.begin(); iter!= testFold.end();iter++)
			{
				GlobalFeature* result = this->getNextNeighbour((*iter)->localFeatures);
				if(result->classLabel == (*iter)->classLabel)
					correct++;
			}
			testFold.clear();
			trainingSet.assign(allFeatures.begin(),allFeatures.end());		
		}

		float cvAcc = (float) correct / (step * nFolds) * 100;
		printf("****DTW NN CrossValidation Accurancy: %f \n\n",cvAcc);
		return cvAcc;
	}