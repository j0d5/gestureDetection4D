#include "Datasource.h"


Datasource::Datasource(void)
{
  this->conn=new Connection();
}

std::vector <OniFileDataSet*> Datasource::getOniFileDatasetsByGesture(char* gesture){
  MYSQL_RES *res;	// the results
  MYSQL_ROW dbRow;	// the results row (line by line)
  char query[512];
  vector <OniFileDataSet*> dataSets;

  sprintf(query,"SELECT DISTINCT(of.name ) FROM (oniFile AS of INNER JOIN gesture2oni AS g2o	ON of.idOniFile = g2o.oniFile_idOniFile) INNER JOIN gesture g	ON g2o.gesture_idGesture=g.idGesture WHERE g.name='%s'",gesture);
  res= this->conn->mysql_perform_query(query);
  if (res==0) {
    std::cerr << "There are no oniFile with gesture:  "<< gesture << std::endl;
  } else {
    int fieldNumbers = mysql_num_fields(res);
    while ((dbRow = mysql_fetch_row(res))) {
      OniFileDataSet* oni= new OniFileDataSet(dbRow[0], this->conn);
      dataSets.push_back(oni);
    }
  }
  return dataSets;
}

std::vector <OniFileDataSet*> Datasource::getOniFileDatasets() {
  MYSQL_RES *res;	// the results
  MYSQL_ROW dbRow;	// the results row (line by line)
  char query[512];
  vector <OniFileDataSet*> dataSets;

  sprintf(query,"SELECT DISTINCT(of.name) FROM (oniFile AS of INNER JOIN gesture2oni AS g2o ON of.idOniFile = g2o.oniFile_idOniFile) INNER JOIN gesture g ON g2o.gesture_idGesture=g.idGesture");
  res = this->conn->mysql_perform_query(query);
  if (res == 0) {
    std::cerr << "There are no oniFile." << std::endl;
  } else {
    int fieldNumbers = mysql_num_fields(res);
    while ((dbRow = mysql_fetch_row(res))) {
      OniFileDataSet* oni= new OniFileDataSet(dbRow[0], this->conn);
      dataSets.push_back(oni);
    }
  }
  return dataSets;
}

OniFileDataSet* Datasource::getOniFileDataSetByName(char* name){
  OniFileDataSet* oni=new OniFileDataSet(name, this->conn);
  return oni;
}
void Datasource::createNewFileEntry(char* name,XnPoint3D* start, char* gesture){

  OniFileDataSet* dataset= new OniFileDataSet(name,  start, this->conn, gesture);
}

Datasource::~Datasource(void)
{
}
