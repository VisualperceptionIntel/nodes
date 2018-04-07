#include "MySql_conn.h"
using namespace std;

///A constructor is created here for the MySQL connection class
MysqlConn::MysqlConn(void)
{
    printf("Constructor for MySQL here\n");
}

///Destructor for the created MySQL class.
MysqlConn::~MysqlConn(void)
{
    printf("MySQL class killed here\n");
}

///Connect to MySql db here with a registered credentials
bool MysqlConn::connect_MySql()
{
    connection = mysql_init(NULL);
    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        return false;
    }
    return true;
}

///Free any results and close the connection
void MysqlConn::close_MySql()
{
    //mysql_free_result(result);
    mysql_close(connection);

}

int MysqlConn::save_to_file(std::string _file_name,std::string _context)
{
 ofstream   default_values_file(_file_name.c_str());
            default_values_file << _context;
            default_values_file.close();
}




string MysqlConn::SELECT(string query)
{
    Mysql_string="";
    mysqlRes.clear();
    std::cout<<query<<std::endl;
    if(mysql_query(connection, query.c_str()))
    {
        printf("Query Error: %s", mysql_error(connection));
        return Mysql_string;
    }
    //mysql_free_result(result);
    result = mysql_use_result(connection);
    for(int i=0; i<2; ++i)
    {
        //if(mysql_fetch_row(result)!=NULL)
        if( row =mysql_fetch_row(result))
        {
            std::vector<std::string> resRow;
            for(int j=0; j < mysql_num_fields(result); ++j)
            {

                if( j == mysql_num_fields(result)-1 )
                {
                    Mysql_string = Mysql_string + row[j] + "\n";
                    resRow.push_back(row[j]);

                }
                else Mysql_string = Mysql_string + row[j] + ",";
            }
         mysqlRes.push_back(resRow);
            i=0;
        }
        else
        {
            continue;
        }

    }
    printf("%s \n",Mysql_string.c_str());
    mysql_free_result(result);
    return Mysql_string;
}


int MysqlConn::INSERT(string query)
{
 if(mysql_query(connection, query.c_str()))
    {
        printf("Query Error: %s", mysql_error(connection));
        return -1;
    }

    return 1;
}

int MysqlConn::UPDATE(string query)
{

    printf("\033[1m\033[32m -- UPDATING HERE --\033[0m\n");
    if(mysql_query(connection, query.c_str()))
    {
        printf("Query Error: %s", mysql_error(connection));
        return -1;
    }


    result = mysql_use_result(connection);

    mysql_free_result(result);
    mysql_close(connection);




    return 1;
}

    std::vector<std::vector<std::string> > MysqlConn::get_mysqlRes()
    {
        return mysqlRes;
    }

