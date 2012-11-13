#include "OniFileDataSet.h"

/** 
* Helper function!
* Converts char to int via stream. Only to use with real numers in char.
* @return the converted int
*/
int char2int(char* c){
  std::stringstream ss(c);
  int i;
  if( ss >> i ){
  }else{
    std::cout << "error";
  }
  return i;
}



OniFileDataSet::OniFileDataSet(char* fileName,XnPoint3D* start, Connection *conn, char* gesture){
  this->start=start;
  this->gesture=gesture;
	struct stat fileStat; 
	int err = stat( fileName, &fileStat );  
	// Extract size
	char *tmp = new char[strlen(fileName)+1];
	strcpy(tmp,fileName);
	this->size=fileStat.st_size;
	this->filepath=fileName;
	// Extract filename and filepath from given filestring
	
	spl s=this->split(tmp,'/');
	this->filename=s.pointers[s.count-1];
	
    this->save(conn);
}

OniFileDataSet::OniFileDataSet(char* fileName, Connection* conn){
  this->readFromDB(fileName, conn);
}

struct spl OniFileDataSet::split (char *in, char delim)
{
	struct spl sp;
	sp.count = 1;
	sp.pointers[0] = in;

	while (*++in)
	{
		if (*in == delim)
		{
			*in = 0;
			sp.pointers[sp.count++] = in+1;
		}
	}

	return sp;
}
int OniFileDataSet::getFileId(){
	return this->fileId;
}
int OniFileDataSet::getSize (){
	return this->size;
}
char* OniFileDataSet::getFileName(){
	return this->filename;
}
char* OniFileDataSet::getFilepath(){
	return this->filepath;
}


void OniFileDataSet::setFileId(int fileId){
	this->fileId=fileId;
}


void OniFileDataSet::setSize(int size){
	this->size=size;
}
void OniFileDataSet::setFileName(char* filename){
	this->filename=filename;
}
void OniFileDataSet::setFilePath(char* filepath){
	this->filepath=filepath;
}

void OniFileDataSet::save(Connection *conn){

  char *query;
  int gestureID;
  int oniId;
  query= new char[256];
  // write Metadata of the oni file
  sprintf(query , "INSERT INTO onifile (name, size,filePath) VALUES ('%s', %d, '%s')", this->getFileName(), this->getSize(), this->getFilepath()); 
  conn->mysql_perform_query(query);
  // get the new oniId
  oniId=this->getOniFileIdByName(this->getFileName(), conn);

  // get gesture id
  gestureID= this->getGestureId(gesture, conn);
  try{
  // Write gesture2oni
  sprintf(query,  "INSERT INTO gesture2oni ( gesture_idGesture,  oniFile_idOniFile ,startPoint_X,startPoint_Y,startPoint_Z ) VALUES ('%d','%d','%f','%f','%f' )", gestureID, oniId,this->start->X,this->start->Y,this->start->Z);
  conn->mysql_perform_query(query);
  }catch(exception e){
	 
  }
}

/**
* Get the GestureId from the database which correspronds to the gesture name.
*
* @return int gestureID with the id, if gestureID=-1 then there was no gesture found in db
*/
int OniFileDataSet::getGestureId(char* gestureName, Connection* conn){
  MYSQL_RES *res;	// the results
  char query[256];
  MYSQL_ROW dbRow;	// the results row (line by line)
  int gestureID;
  sprintf(query,  "SELECT idGesture FROM gesture WHERE name='%s'", gestureName);
  res = conn->mysql_perform_query(query);
  if (res==0) {
    std::cerr << "There is no gesture with name:  "<< gestureName << std::endl;
    gestureID=-1;
  } else {
    int fieldNumbers = mysql_num_fields(res);
    while ((dbRow = mysql_fetch_row(res))) {
      gestureID= char2int(dbRow[0]);
    }
  }
  return gestureID;
}

void OniFileDataSet::readFromDB(char* name, Connection* conn){
  MYSQL_RES *res;	// the results
 MYSQL_ROW dbRow;	// the results row (line by line)
 char query[512];
 sprintf(query,"SELECT  of.idOniFile , of.name, of.size, of.filePath FROM oniFIle AS of WHERE of.name='%s'",name);
 res= conn->mysql_perform_query(query);
 if (res==0) {
    std::cerr << "There is no oniFile with name:  "<< name << std::endl;
  } else {
    int fieldNumbers = mysql_num_fields(res);
    while ((dbRow = mysql_fetch_row(res))) {
      this->setFileId(char2int(dbRow[0]));
      this->setFileName(dbRow[1]);
      this->setSize(char2int(dbRow[2]));
      this->setFilePath(dbRow[3]);
    }
 }
}

/**
* Get the OniId from the database which correspronds to the oni name.
*
* @return int fileID with the id, if fileId=-1 then there was no oniFile.
*/

int OniFileDataSet::getOniFileIdByName(char* fileName, Connection* conn){
  MYSQL_RES *res;	// the results
  char query[256];
  MYSQL_ROW dbRow;	// the results row (line by line)
  int fileId;
  sprintf(query,  "SELECT idOniFile FROM oniFile WHERE name='%s'", fileName);
  res = conn->mysql_perform_query(query);
  if (res==0) {
    std::cerr << "There is no oniFile with name:  "<< fileName << std::endl;
    fileId=-1;
  } else {
    int fieldNumbers = mysql_num_fields(res);
    while ((dbRow = mysql_fetch_row(res))) {
      fileId= char2int(dbRow[0]);
    }
  }
      return fileId;
}

OniFileDataSet::~OniFileDataSet(void)
{
}
