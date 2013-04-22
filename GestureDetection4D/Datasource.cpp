#include "Datasource.h"

Datasource::Datasource(void)
{
	this->conn=new Connection();
}

/// get onifile datasets by gesture name, optional start and end idx can be set to get only a subsetz of entrys!
std::vector <OniFileDataSet*> Datasource::getOniFileDatasetsByGesture(char* gesture, int startIdx, int endIdx){
	MYSQL_RES *res;	// the results
	MYSQL_ROW dbRow;	// the results row (line by line)
	char query[1024];
	vector <OniFileDataSet*> dataSets;

	
	sprintf(query,"SELECT DISTINCT(of.name ) FROM (oniFile AS of INNER JOIN gesture2oni AS g2o	ON of.idOniFile = g2o.oniFile_idOniFile) INNER JOIN gesture g	ON g2o.gesture_idGesture=g.idGesture WHERE g.name='%s'",gesture);
	if(startIdx >= 0 && endIdx >= 1)
		sprintf(query + strlen(query),"LIMIT %d , %d",startIdx,endIdx);
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

/// get all onifile datasets
std::vector <OniFileDataSet*> Datasource::getOniFileDatasets() {
	MYSQL_RES *res;	// the results
	MYSQL_ROW dbRow;	// the results row (line by line)
	char query[512];
	vector <OniFileDataSet*> dataSets;

	sprintf_s(query,"SELECT DISTINCT(of.name) FROM (oniFile AS of INNER JOIN gesture2oni AS g2o ON of.idOniFile = g2o.oniFile_idOniFile) INNER JOIN gesture g ON g2o.gesture_idGesture=g.idGesture");
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

/// get onifile dataset by a specific oni name
OniFileDataSet* Datasource::getOniFileDataSetByName(char* name){
	OniFileDataSet* oni=new OniFileDataSet(name, this->conn);
	return oni;
}

/// create a new entry for gesture with handpoints
void Datasource::createNewFileEntry(char* name,XnPoint3D* start, char* gesture, std::vector<XnPoint3D>& handPoints){
	OniFileDataSet dataset(name, start, this->conn, gesture);
	dataset.insertHandPoints3D(handPoints,this->conn);
}

/// get all gesture names from DB
std::map<int, char*> Datasource::getGestureNames()  {
	MYSQL_RES *res;	// the results
	MYSQL_ROW dbRow;	// the results row (line by line)
	char query[512];
	std::map<int, char*> gestures;
	
	sprintf_s(query,"SELECT g.idGesture, g.name FROM kinect.gesture AS g");
	res = this->conn->mysql_perform_query(query);
	
	if (res == 0) {
		std::cerr << "There are no gestures." << std::endl;
	} else {
		// int fieldNumbers = mysql_num_fields(res);

		while ((dbRow = mysql_fetch_row(res))) {
			gestures.insert(pair<int, char*>(atoi(dbRow[0]), dbRow[1]));
		}
	}
	return gestures;
}

Datasource::~Datasource(void)
{
}
