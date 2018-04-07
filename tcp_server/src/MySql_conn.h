#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <time.h>
#include <sys/fcntl.h>
#include <mysql/mysql.h>
#include "std_msgs/String.h"
#include <algorithm>  //sort
#include <valarray>   //valarray
#include <pthread.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <omp.h>
#include <vector>



class MysqlConn
{
public:
    MysqlConn();
    ~MysqlConn();
    bool connect_MySql();
    void close_MySql();
    std::string SELECT(std::string query);
    int INSERT(std::string query);
    int UPDATE(std::string query);
    int save_to_file(std::string file_name,std::string context);
    std::vector<std::vector<std::string> > get_mysqlRes();

private:
    MYSQL       *connection;
    std::string  Mysql_string;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    std::vector<std::vector<std::string> > mysqlRes;



};
