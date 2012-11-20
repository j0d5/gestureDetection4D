#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <sys\types.h> 
#include <sys\stat.h> 
#include <time.h>
#include <XnOpenNI.h>
#include <winsock2.h>
#include <mysql.h>
#include "Connection.h"
#include <vector>

// Helper function
struct spl
{
	char *pointers[256];
	int count;
};

/**
* @author Pascal Mueller, Florian Gref
* @email pascal_mueller@gmx.net, florian.gref@gmail.com
*
* This class maps a OniFile with all depending metadata like the fileSize, Name....
* 
* 
*/
class OniFileDataSet
{
private:
	XnPoint3D* start;
	int fileId;
	int gestureId;
	char* filename;
	char* gestureName;
	int size;
	char* filepath;
	// writes all attribute to the DB
	void save(Connection* conn);
	// looks up the gestureId from DB
	int getGestureIdFromDB(char* gestureNamechar, Connection* conn);
	// looks up the oniFileId from db
	int getOniFileIdByName(char* fileName, Connection* conn);
	// fills all attribute from this class with values from the db
	void readFromDB(char* name, Connection* conn);
	std::vector<XnPoint3D> m_handPoints;

public:
	struct spl split (char *in, char delim);
	int getFileId();
	char* getFileName();
	int getSize();
	char* gesture;
	char* getFilepath();
	void setFileId(int fileId);
	void setFileName(char* filename);
	void setSize(int size);
	void setFilePath(char* filepath);
	void setGestureId(int id);
	int getGestureId();
	void setGestureName(char* name);
	char* getGestureName();
	std::vector<XnPoint3D> getHandPoints();
	// Creates a new DataSet with all infos from the db
	OniFileDataSet(char* fileName, Connection* conn);
	// Creates a DataSet and stores the information to the db
	OniFileDataSet(char* fileName,XnPoint3D* start, Connection *conn, char* gesture);
	~OniFileDataSet(void);
	void insertHandPoints3D(std::vector<XnPoint3D>& handPoints,Connection *conn);
};

