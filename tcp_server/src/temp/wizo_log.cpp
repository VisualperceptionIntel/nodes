#include "server_node.h"

void display(log_list* log_node,int type){
    while(log_node!=NULL){
        if(type==LOG_OK){
            cout<<BOLDGREEN<<log_node->log_updated<<","<<log_node->log_parent_type<<","<<log_node->log_info<<"->"<<RESET<<endl;
        }else if(type==LOG_ERROR){
            cout<<BOLDRED<<log_node->log_updated<<","<<log_node->log_parent_type<<","<<log_node->log_info<<"->"<<RESET<<endl;
            }
        log_node=log_node->next;
    }
}

void insertEnd(log_list* log_node){
    if(start == NULL){
        start = log_node;
        current = log_node;
        }else{
        current->next = log_node;
        current = log_node;
        }
}

void wizo_log(int display_log,string date_time, unsigned int type,string source, string log_line, float log_lat){
    string seperator= ",";
    ptr = new log_list;
    ptr->log_updated = date_time;
    ptr->log_type = type;
    ptr->log_parent_type = source;
    ptr->log_info =log_line;
    ptr->log_lat= log_lat;
    ptr->next = NULL;
    insertEnd(ptr);
    if(display_log==1)
        display(start,type);
}

//void wizo_faq(int display_faq,unsigned int faq_id,string question, string answer){
//    string seperator= ",";
//    faq_ptr = new faq_list;
//    ptr->log_updated = date_time;
//    ptr->log_type = type;
//    ptr->log_parent_type = source;
//    ptr->log_info =log_line;
//    ptr->log_lat= log_lat;
//    ptr->next = NULL;
//    insertEnd(ptr);
//    if(display_log==1)
//        display(start,type);
//}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *log_mysql_update_function(void *arg)
{
    log_list* log_node;
    log_list* temp_node;
    char query_str[256];
    int connection_status;

    while(1)
    {
        pthread_mutex_lock( &mutex );
        pthread_cond_wait( & cond, & mutex );
        pthread_mutex_unlock( &mutex );

        MYSQL *connection;
        MYSQL_RES *result;
        MYSQL_ROW row;
        MYSQL_FIELD *field;

        connection = mysql_init(NULL);

        if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(connection));
            //mysql_close(connection);
            connection_status = -1;
            //return -1;
        }
        else
            connection_status = 1;


        log_node = start;
        printf("\033[1;31mLog updated to mysql here\033[0m\n");
        #pragma omp for
        for(;;)
        {
            if(log_node==NULL || connection_status == -1)
                break;
            else
            {
                cout<<BOLDGREEN<<log_node->log_updated<<","<<log_node->log_parent_type<<","<<log_node->log_info<<"->"<<RESET<<endl;
                sprintf(query_str, "insert Wizo.transactions set transactions.info='%s',transactions.parent_type='%s', transactions.created='%s', transactions.updated='%s';",log_node->log_info.c_str(),log_node->log_parent_type.c_str(),log_node->log_updated.c_str(),log_node->log_updated.c_str());

                if(connection_status == 1)
                {
                    if(mysql_query(connection, query_str))
                    {
                        fprintf(stderr,"Query Error: %s\n", mysql_error(connection));
                        connection_status == -1;
                    }
                }
                temp_node=log_node->next;
                log_node=NULL;
                log_node=temp_node;
            }
        }
        mysql_close(connection);
        start=log_node;
        pthread_mutex_unlock( &mutex );
        sleep(LOG_UPDATE_RATE);
    }
}

