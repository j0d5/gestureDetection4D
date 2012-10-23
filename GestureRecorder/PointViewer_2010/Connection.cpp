
#include "Connection.h"


// just going to input the general details and not the port numbers
struct connection_details
{
  char *server;
  char *user;
  char *password;
  char *database;
  int port;
};

Connection::Connection(void)
{
  struct connection_details mysqlD;
  mysqlD.server = "localhost";  // where the mysql database is
  mysqlD.port= 3306;			  // the port on which the database runs
  mysqlD.user = "root";		  // the root user of mysql	
  mysqlD.password = "01mase01";   // the password of the root user in mysql
  mysqlD.database = "kinect";	  // the databse to pick

  // connect to the mysql database
  this->conn = this->mysql_connection_setup(mysqlD);
  
}


MYSQL* Connection::mysql_connection_setup(struct connection_details mysql_details)
{
  // first of all create a mysql instance and initialize the variables within
  MYSQL *connection = mysql_init(NULL);

  // connect to the database with the details attached.
  if (!mysql_real_connect(connection,mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, mysql_details.port, NULL, 0)) {
    printf("Conection error : %s\n", mysql_error(connection));
    exit(1);
  }
  return connection;
}

MYSQL_RES* Connection::mysql_perform_query(char *sql_query)
{
  // send the query to the database
  if (mysql_query(this->conn, sql_query))
  {

    printf("MySQL query error : %s\n", mysql_error(this->conn));
    exit(1);
  }

  return  mysql_store_result(this->conn);
}




/**
* Closes the DB Connection. And frees the result.
*
*
*/
Connection::~Connection()
{
  /* clean up the database link */
  mysql_close(this->conn);
}
