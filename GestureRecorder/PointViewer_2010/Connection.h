#pragma once

#include <XnOpenNI.h>
#include <string>
#include <vector>
#include <winsock2.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
 

/**
* @author Pascal Mueller, Florian Gref
* @email pascal_mueller@gmx.net, florian.gref@gmail.com
*
* This class establishes the connection to the Database and handles all the queries.
* Please set environment variables as described: $MYSQL_LIB to the mysql and 
* $MYSQL_IN to your mysql installation mysql/include.
* Also add libmysql.lib to Properties->Linker->Input additional dependencies!!!!
* 
*/

struct fileDate{
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;
};
using namespace std;
class Connection
{
private:
	MYSQL *conn;		// the connection
	MYSQL_RES *res;	// the results
	MYSQL_ROW row;	// the results row (line by line)

public:
	Connection(void);
	MYSQL* mysql_connection_setup(struct connection_details mysql_details);
	MYSQL_RES* mysql_perform_query( char *sql_query);
  ~Connection(void);

};

