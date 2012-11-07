#pragma once

#include "Connection.h"
#include "OniFileDataSet.h"
/**
*
* This class keeps the Connection to the Database 
* and abstracts the DB from the main application.
*
* @author Florian Gref
* @author Pascal Mueller pascal_mueller@gmx.net
* 
*/

#define LOCAL_SAVEDIR "./OniFiles"

class Datasource
{
private:
  Connection* conn;
public:
  Datasource(void);
  // Returns all the oniFiles which contains the gesture.
  std::vector <OniFileDataSet*> getOniFileDatasetsByGesture(char* gesture); 
  // Looks up a special oniFIle by Name in the DB
  OniFileDataSet* getOniFileDataSetByName(char* name);

  // Creates a new OniFileDataSet and stores it in the DB.
  void createNewFileEntry(char* name,XnPoint3D* start, char* gesture);

  ~Datasource(void);
};

