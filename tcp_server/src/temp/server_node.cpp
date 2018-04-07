#include "server_node.h"
command_unit master_commands, mysql_commands, tcp_commands;
log_list *start, *newptr, *current, *ptr;
faq_list *faq_start, *newfaq_ptr, *faq_current, *faq_ptr;
int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
char current_time[15];
char current_date_time[25];
boost::array<unsigned int,NUM_EMOTIONS> emotions_count;


int set_timer(struct timeval &tv, time_t sec)
{
    gettimeofday(&tv,NULL);
    tv.tv_sec+=sec;

    return 1;
}

void time_stamp(char* time_buffer)
{
    time_t rawtime;
    struct tm * timeinfo;
    //char time_buffer [40];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer,40,"%F %X",timeinfo);
    strftime(current_time,15,"%X",timeinfo);
}


int send_master_commands_update(int newsockfd)
{
    char update_buffer[WRITE_BUFFER_LENGTH];

    //time_stamp(master_commands.updated);

    sprintf(update_buffer,"base_busy,%i,base_pos_x,%f,base_pos_y,%f,base_dest_x,%f,base_dest_y,%f,forward_step,%i,backward_step,%i,rotate_left,%i,rotate_right,%i,left_busy,%i,right_busy,%i,"
            "left_arm_move,%i,right_arm_move,%i,arm_action,%s,emotion_status,%i,light_status,%i,print_status,%i,face_cognition_status,%i,card_reader_status,%i,face_cognition_outcome,%s,id_card_name,%s,"
            "visitee_name,%s,appointment_date,%s,appointment_time,%s,appointment_location,%s,speech_request,%i,speech_status,%i,latitude,%f,longitude,%f,altitude,%f,frame,%s,obj_x,%f,obj_y,%f,obj_z,%f,obj_height,%f,obj_width,%f,<EOC>",
            master_commands.is_base_busy,master_commands.base_position_x,master_commands.base_position_y,master_commands.base_destination_x,master_commands.base_destination_y,master_commands.base_forward_step,
            master_commands.base_backward_step,master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,master_commands.is_right_arm_busy,master_commands.left_arm_move,
            master_commands.right_arm_move,master_commands.arm_actions,master_commands.emotion_status,master_commands.light_status,master_commands.print_status,master_commands.face_cognition_status,master_commands.card_reader_status,
            master_commands.face_cognition_outcome,master_commands.id_card_name,master_commands.visitee_name,master_commands.appointment_date,master_commands.appointment_time,master_commands.appointment_location,
            master_commands.speech_request,master_commands.speech_status,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude,master_commands.obj_frame,master_commands.obj_x,master_commands.obj_y,master_commands.obj_z,master_commands.obj_height,master_commands.obj_width);

    return tcp_write(newsockfd,update_buffer);
}


int send_welcome_msg(int newsockfd)
{
    char msg_buffer[WELCOME_BUFFER_LENGTH];

    //time_stamp(master_commands.updated);
    sprintf(msg_buffer,"base_busy,%i,base_pos_x,%f,base_pos_y,%f,base_dest_x,%f,base_dest_y,%f,forward_step,%i,backward_step,%i,rotate_left,%i,rotate_right,%i,left_busy,%i,right_busy,%i,"
            "left_arm_move,%i,right_arm_move,%i,arm_action,%s,emotion_status,%i,light_status,%i,print_status,%i,face_cognition_status,%i,card_reader_status,%i,face_cognition_outcome,%s,id_card_name,%s,"
            "visitee_name,%s,appointment_date,%s,appointment_time,%s,appointment_location,%s,speech_request,%i,speech_status,%i,latitude,%f,longitude,%f,altitude,%f,frame,%s,obj_x,%f,obj_y,%f,obj_z,%f,obj_height,%f,obj_width,%f,"
            "welcome_msg,Welcome your Highness Sheik Mansour bin Muhammad bin Rashid, we are honoured by your attendance to the UAE Drones for good award. We hope you enjoy!,<EOC>",
            master_commands.is_base_busy,master_commands.base_position_x,master_commands.base_position_y,master_commands.base_destination_x,master_commands.base_destination_y,master_commands.base_forward_step,
            master_commands.base_backward_step,master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,master_commands.is_right_arm_busy,master_commands.left_arm_move,
            master_commands.right_arm_move,master_commands.arm_actions,master_commands.emotion_status,master_commands.light_status,master_commands.print_status,master_commands.face_cognition_status,master_commands.card_reader_status,
            master_commands.face_cognition_outcome,master_commands.id_card_name,master_commands.visitee_name,master_commands.appointment_date,master_commands.appointment_time,master_commands.appointment_location,
            master_commands.speech_request,master_commands.speech_status,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude,master_commands.obj_frame,master_commands.obj_x,master_commands.obj_y,master_commands.obj_z,master_commands.obj_height,master_commands.obj_width);


    return tcp_write(newsockfd,msg_buffer);
}

void update_mysql_cmd_unit(str_tok_buffer str_tok_buf_tmp, string query_type,char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& mysql_commands)
{
    if (query_type=="visit")
    {
        if( (str_tok_buf_tmp[DB_HOST].tok!="(NULL)") || (str_tok_buf_tmp[DB_HOST].tok!="(null)") )
        {
            mysql_commands.visitee_name = new char[strlen(str_tok_buf_tmp[DB_HOST].tok)+1];
            strcpy(mysql_commands.visitee_name,str_tok_buf_tmp[DB_HOST].tok);
            is_updated[VISITEE_NAME] = 1;
        }
        if( (str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok!="(NULL)") || (str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok!="(null)") )
        {
            char* pch;
            pch = strtok(str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok," ");
            mysql_commands.appointment_date = new char[strlen(pch)+1];
            strcpy(mysql_commands.appointment_date,pch);
            pch = strtok(NULL," ");
            mysql_commands.appointment_time = new char[strlen(pch)+1];
            strcpy(mysql_commands.appointment_time,pch);
            is_updated[APPOINTMENT_DATE] = 1;
            is_updated[APPOINTMENT_TIME] = 1;
        }
        if( (str_tok_buf_tmp[DB_APPOINTMENT_LOCATION].tok!="(NULL)") || (str_tok_buf_tmp[DB_APPOINTMENT_LOCATION].tok!="(null)") )
        {
            mysql_commands.appointment_location = new char[strlen(str_tok_buf_tmp[2].tok)+1];
            strcpy(mysql_commands.appointment_location,str_tok_buf_tmp[2].tok);
//            mysql_commands.appointment_location = str_tok_buf_tmp[2].tok;
//            cout<<"--------"<<mysql_commands.appointment_location;
            is_updated[APPOINTMENT_LOCATION] = 1;
        }
    }

    else
    {
        if( (str_tok_buf_tmp[BASE_DESTINATION_X].tok!="(NULL)") || (str_tok_buf_tmp[BASE_DESTINATION_X].tok!="(null)") )
        {
            mysql_commands.base_destination_x = atof(str_tok_buf_tmp[BASE_DESTINATION_X].tok);
            is_updated[BASE_DESTINATION_X] = 1;
        }

        if( (str_tok_buf_tmp[BASE_DESTINATION_Y].tok!="(NULL)") || (str_tok_buf_tmp[BASE_DESTINATION_Y].tok!="(null)") )
        {
            mysql_commands.base_destination_y = atof(str_tok_buf_tmp[BASE_DESTINATION_Y].tok);
            is_updated[BASE_DESTINATION_Y] = 1;
        }

        if( (str_tok_buf_tmp[UPDATED].tok!="(NULL)") || (str_tok_buf_tmp[UPDATED].tok!="(null)") )
        {
            strcpy(mysql_commands.updated, str_tok_buf_tmp[UPDATED].tok);
            is_updated[UPDATED] = 1;
        }

        update_master_commands(mysql_commands, is_updated);

    }
}

int mysql_str_to_array(char* sql_data, str_tok_buffer str_tok_buf_tmp, string query_type,char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& command_update)
{
    char* pch;
    int j=1;

    if( sql_data == NULL )
    {
        printf("\nWARNING: cannot split array, sql_data empty.\n");
        return -1;
    }
    pch = strtok(sql_data,",");
    str_tok_buf_tmp[0].tok = pch;
    while(pch != NULL)
    {
        pch = strtok(NULL,",");
        str_tok_buf_tmp[j].tok = pch;
        ++j;
    }

    for(int l=0; l<j; ++l)
    {
        if( str_tok_buf_tmp[l].tok == NULL )
            break;
        else
            printf("str_tok_buf_tmp[%i]: %s\n",l,str_tok_buf_tmp[l].tok);
    }

    update_mysql_cmd_unit(str_tok_buf_tmp, query_type,is_updated,command_update);
}



void faq_insertEnd(faq_list* faq_node){
    if(faq_start == NULL){
        faq_start = faq_node;
        faq_current = faq_node;
        }else{
        faq_current->next = faq_node;
        faq_current = faq_node;
        }
}

//read the faq list from datbase
string retrive_faq_from_db()
{
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    //delete_faq(faq_start);
    faq_start = NULL;
    faq_list *faq_from_db;
    std::string s;

    connection = mysql_init(NULL);

    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        //exit(1);
        return "";
    }

        char* query = new char[512];
        sprintf(query,"SELECT FAQid,question,answer FROM Wizo.FAQ");

        if(mysql_query(connection,query ))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }

          result = mysql_use_result(connection);

    for(int i=0; i <= mysql_field_count(connection); ++i)
    {
        //std::stringstream ss;
        row = mysql_fetch_row(result);

        if(row <= 0)
        {
            printf("No row returned, table not updated.\n");
          return "";
        }


//faq_from_db->faq_id = atoi(row[0]);
//faq_from_db->question = row[1];
//faq_from_db->answer = row[2];
//
//faq_insertEnd(faq_from_db);

s=s+"id,"+row[0]+",question,"+row[1]+",answer,"+row[2]+",";
//     switch (j){
//        case 0:
//
//            break;
//
//        case 1:
//            break;
//
//        case 2:
//            break;
//
//        default:
//        break;
//        }




//        for(int j=0; j < mysql_num_fields(result); ++j)
//        {
//
//
//
//
//            //ss << row[j];
//            if( j == mysql_num_fields(result)-1 && i == mysql_field_count(connection)-1 )
//              s = s + row[j];
//            else s = s + row[j] + ",";
//        }

    }

    s=s+"<EOC>";
mysql_free_result(result);
        mysql_close(connection);

        return s;


}





//read the fields from the database and return as a string.
string read_from_mysql_db(string query_type, char* name)
{
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    std::string s;

    connection = mysql_init(NULL);

    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        //exit(1);
        return "";
    }

    if(query_type=="visit")
    {
        //and user_visit.start_datetime >= now() and user_visit.start_datetime < curdate()+1

        printf("%s\n",name);
        char* query = new char[512];
        sprintf(query,"SELECT first_name, start_datetime,pickup_locationid FROM Wizo.user_visit, Wizo.user_data WHERE user_visit.visitor_name = '%s' and user_data.userid = user_visit.host_userid and user_visit.start_datetime >= now() and user_visit.start_datetime < curdate()+1 ORDER BY user_visit.start_datetime desc",name);


        if(mysql_query(connection,query ))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }
    }
    else if(query_type=="faq")
    {
        //and user_visit.start_datetime >= now() and user_visit.start_datetime < curdate()+1


        char* query = new char[512];
        sprintf(query,"SELECT FAQid,question,answer FROM Wizo.FAQ");


        if(mysql_query(connection,query ))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }
    }

    else
    {
        if(mysql_query(connection, "SELECT * FROM Wizo.control_unit ORDER BY control_unit.controlid desc limit 1"))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }
    }
    result = mysql_use_result(connection);

    for(int i=0; i < mysql_field_count(connection); ++i)
    {
        std::stringstream ss;
        row = mysql_fetch_row(result);

        if(row <= 0)
        {
            printf("No row returned, table not updated.\n");
            return "";
        }

        for(int j=0; j < mysql_num_fields(result); ++j)
        {
            ss << row[j];
            if( j == mysql_num_fields(result)-1 ) s = s + row[j];
            else s = s + row[j] + ",";
        }
        mysql_free_result(result);
        mysql_close(connection);

        return s;
    }
}

void update_mysql_db(char query_str[256])
{
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    connection = mysql_init(NULL);

    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        //return -1;
    }

//    char query_str[256];
//    sprintf(query_str, "UPDATE Wizo.control_unit SET"
//            " control_unit.is_base_busy=%i,"
//            " control_unit.base_position_x=%f,"
//            " control_unit.base_position_y=%f,"
//            " control_unit.base_forward_step=%i,"
//            " control_unit.base_backward_step=%i,"
//            " control_unit.base_rotate_left=%i,"
//            " control_unit.base_rotate_right=%i,"
//            " control_unit.is_left_arm_busy=%i,"
//            " control_unit.is_right_arm_busy=%i,"
//            " control_unit.left_arm_move=%i,"
//            " control_unit.right_arm_move=%i,"
//            " control_unit.emotion_status=%i,"
//            " control_unit.light_status=%i,"
//            " control_unit.print_status=%i,"
//            " control_unit.face_cognition_status=%i,"
//            " control_unit.face_cognition_outcome=%s,"
//            " control_unit.card_reader_status=%i,"
//            " control_unit.updated=%s,"
//            " WHERE control_unit.controlid=1",master_commands.is_base_busy,master_commands.base_position_x,
//            master_commands.base_position_y,master_commands.base_forward_step,master_commands.base_backward_step,
//            master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,
//            master_commands.is_right_arm_busy,master_commands.left_arm_move,master_commands.right_arm_move,
//            master_commands.emotion_status,master_commands.light_status,master_commands.print_status,
//            master_commands.face_cognition_status,master_commands.face_cognition_outcome,master_commands.card_reader_status,
//            master_commands.updated);

    //printf("SQL Query formed:\n%s\n",query_str);

    if(mysql_query(connection, query_str))
    {
        fprintf(stderr,"Query Error: %s\n", mysql_error(connection));
        return;
    }
    mysql_close(connection);
}


void faq_db_request(str_tok_buffer& str_tok_buf_tmp)
{
    mysql_commands = command_unit();
    char is_updated[COMMAND_UNIT_LENGTH];
    char* name =new char[1];
    name[1]= '\0';


    string query= "all";
    string str = read_from_mysql_db(query,name);
    if( str.empty() )

        return;
    char* data_buffer = new char[str.size()+1];
    std::copy(str.begin(), str.end(), data_buffer);
    data_buffer[str.size()] = '\0';

    mysql_str_to_array(data_buffer, str_tok_buf_tmp,query,is_updated,mysql_commands);

    delete(data_buffer);
    str.clear();
}





//update Mysql database once any client request update--- or update on a set interval.. store values until then.
void mysql_cmd_update(str_tok_buffer& str_tok_buf_tmp)
{
    mysql_commands = command_unit();
    char is_updated[COMMAND_UNIT_LENGTH];
    char* name =new char[1];
    name[1]= '\0';


    string query= "all";
    string str = read_from_mysql_db(query,name);
    if( str.empty() )
        return;
    char* data_buffer = new char[str.size()+1];
    std::copy(str.begin(), str.end(), data_buffer);
    data_buffer[str.size()] = '\0';

    mysql_str_to_array(data_buffer, str_tok_buf_tmp,query,is_updated,mysql_commands);

    delete(data_buffer);
    str.clear();
}

void mysql_visit_update(str_tok_buffer& str_tok_buf_tmp, char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& command_update, int source) //source: 1-scanner 2-cognition
{
    string query= "visit";
    string str="";
    //printf("%s\n",command_update.id_card_name);
    if(source == 1)
        str = read_from_mysql_db(query,command_update.id_card_name);
    else if(source == 2)
        str = read_from_mysql_db(query,command_update.face_cognition_outcome);

    if( str.empty() || str=="" )
    {
        command_update.visitee_name= new char;
        strcpy(command_update.visitee_name,"(null)");
        command_update.appointment_date= new char;
        strcpy(command_update.appointment_date,"(null)");
        command_update.appointment_time= new char;
        strcpy(command_update.appointment_time,"(null)");
        command_update.appointment_location= new char;
        strcpy(command_update.appointment_location,"(null)");
        is_updated[VISITEE_NAME] = 1;
        is_updated[APPOINTMENT_DATE] = 1;
        is_updated[APPOINTMENT_TIME] = 1;
        is_updated[APPOINTMENT_LOCATION] = 1;
        return;
    }
    char* data_buffer = new char[str.size()+1];
    std::copy(str.begin(), str.end(), data_buffer);
    data_buffer[str.size()] = '\0';

    mysql_str_to_array(data_buffer, str_tok_buf_tmp,query,is_updated,command_update);

    delete(data_buffer);
    str.clear();
}

void *thread_timer_function(void *arg)
{
    while(1)
    {
        time_t rawtime;
        struct tm * timeinfo;
        //char time_buffer [40];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        const char* reset_time= "18:00:00";
        strftime(current_time,40,"%F %X",timeinfo);
        if(strcmp(current_time,reset_time)==0)
        {
            for(int i=0; i<NUM_EMOTIONS; i++)
            {
                emotions_count[i]=0;
            }
            printf("emotion value reseted\n");
        }

        printf("                                                          \033[1;31m%s \033[0m\n\033[1A",current_time);
        sleep(1);
    }
}

void *thread_mysql_function(void *arg)
{
    str_tok_buffer sql_str_tok_buf = {NULL};
    while(1)
    {
        printf("\033[1;31mMysql retrieval will be here\033[0m\n");
        mysql_cmd_update(sql_str_tok_buf);
        sleep(60);
    }
}

void *exit_thread_function(void *arg)
{
    char text[100];
    char custom;
    while(1)
    {
        scanf("%s",text);
        if (text[0] == 'q'&& text[1]=='q')
            exit(1);
    }
}

void *faq_client_thread_function(void *arg)
{
fd_set masterfd;
int faq_sockfd= *((int * )arg);
char* faq_buffer;
faq_start = NULL;
string faq=retrive_faq_from_db();
faq_buffer = new char[strlen(faq.c_str())+1];
strcpy(faq_buffer,faq.c_str());


tcp_write(faq_sockfd,faq_buffer);

free(faq_buffer);

//shutdown(faq_sockfd,SHUT_RDWR);
close(faq_sockfd);

}


int main (int argc, char** argv)
{

    ros::init(argc, argv, "server_node");
    ros::NodeHandle nh;


    char log_buff[10];
    start = NULL;
    pthread_t timer_thread, mysql_thread, log_mysl_update_thread,exit_thread,local_db_thread, faq_client_thread;
    int ret=pthread_create(&timer_thread,NULL,&thread_timer_function,NULL);
    int ret_msql=pthread_create(&mysql_thread,NULL,&thread_mysql_function,NULL);
    int ret_log =pthread_create(&log_mysl_update_thread,NULL,&log_mysql_update_function,NULL);
    int ret_exit = pthread_create(&exit_thread,NULL,&exit_thread_function,NULL);

    int sockfd, newsockfd, maxfd, portno; //Socket file descriptors and port number
    char* read_buffer;//[256]; // buffer array of size 256
    char write_buffer[256]; // buffer array of size 256 for sending messages
    int n, update_success=0;

    str_tok_buffer str_tok_buf = {NULL};
    ros::Duration d(0.001); // 1000Hz

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        wizo_log(1,current_time,LOG_ERROR,"Server started","No port provided",0.1);

        exit(1);
    }

    sprintf(log_buff,"port nummber: %i",atoi(argv[1]));
    wizo_log(1,current_time,LOG_OK,"Server started",log_buff,0.2);

    portno = atoi(argv[1]);
    sockfd = create_tcp_socket(portno);
    listen(sockfd,10);
    newsockfd = listen_for_client(sockfd);

    if (-1 == (fcntl(sockfd, F_SETFD, O_NONBLOCK)))
    {
        printf("ERROR Failed to set nonblock flag\n");
        exit(1);
    }

    fd_set sockset, masterfd;
    FD_ZERO(&sockset);
    FD_ZERO(&masterfd);
    FD_SET(sockfd, &masterfd);
    FD_SET(newsockfd, &masterfd);

    maxfd = newsockfd;

    struct timeval tv, tv_timer; // timeout struct for socket calls
//    set_timer(tv_timer,MYSQL_UPDATE_TIME);

    int temp_speech = 0;
    master_commands.speech_request = temp_speech;

    while(ros::ok())
    {
        pthread_mutex_lock( &mutex );
        //cout << "just looping" << endl;
        memcpy(&sockset,&masterfd,sizeof(masterfd));


        int fail_count = 0;

        while( update_success < 1 )
        {
            if( fail_count > 2 )
            {
                close_tcp_socket(newsockfd,masterfd);
                break;
            }

            tv.tv_sec = 2;
            printf("Waiting for message from client on socket %i\n",newsockfd);
            int result = select(newsockfd + 1, &sockset, NULL, NULL, &tv);

            printf("Number of SOCK Fd's with read event ready: %i\n",result);

            if (result < 0)
            {
                printf("\nWARNING: No Read Event on Any Socket\n");
            }
            else if (FD_ISSET(newsockfd, &sockset)) //if (result == 1) {
            {
                // The socket has data. For good measure, it's not a bad idea to test further
                printf("Read Event on Socket %i, I/O in progress...\n",newsockfd);

                read_buffer = new char[256];
                read_buffer = tcp_read(newsockfd);
                printf("\n\nMSG RECEIVED: \n\"%s\"\n\n",read_buffer);
                wizo_log(1,current_time,LOG_OK,"client msg received",read_buffer,0.3);

                // Split received msg and update cmd structures.
                if(tcp_str_to_array(read_buffer,str_tok_buf)>0)
                {

                    if(strcmp(str_tok_buf[0].tok,"base")==0){
                        update_success = base_cmd_update(tcp_commands, str_tok_buf);
                        if( update_success > 0 ) {}
//                            update_mysql_db(); // updates base_pos_x/y and is_busy at the moment
                    }else if(strcmp(str_tok_buf[0].tok,"arms")==0){
                        update_success = arms_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"detect")==0){
                        update_success = detect_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"printer")==0){
                        update_success = printer_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"face")==0){
                        update_success = face_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"scanner")==0){
                        pthread_cond_signal( &cond );
                        update_success = scanner_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"lights")==0){
                        update_success = lights_cmd_update(str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"object_detect")==0){
                        update_success =obj_detect_cmd_update(tcp_commands, str_tok_buf);
                    }else if(strcmp(str_tok_buf[0].tok,"welcome")==0){
                        update_success =UPDATE_WELCOME_MSG;
                    }else if(strcmp(str_tok_buf[0].tok,"master_request")==0){
                        update_success = UPDATE_SUCCESS;
                        printf("Values Requested\n");
                    }else if(strcmp(str_tok_buf[0].tok,"faq_request")==0){
                        update_success = UPDATE_FAQ_REQUEST;
                        printf("Values Requested\n");
                    }else{
                        update_success = -1;
                    }







                    if( update_success < 0 )
                    {
                        char write_buffer[] = "WARN: MSG not recognised,<EOC>";
                        if ( tcp_write(newsockfd,write_buffer) < 0)
                        {
                            close_tcp_socket(newsockfd, masterfd);
                            break;
                        }
                        ++fail_count;
                        continue;
                    }

                    if( update_success>0 )
                    {

                        if (update_success == UPDATE_WELCOME_MSG)
                        {
                            send_welcome_msg(newsockfd);
                            close_tcp_socket(newsockfd,masterfd);

                        }else if (update_success == UPDATE_FAQ_REQUEST){
//                        int *arg = &newsockfd;
                        int ret_faq = pthread_create(&faq_client_thread,NULL,&faq_client_thread_function,&newsockfd);


                        }else
                        {
                            send_master_commands_update(newsockfd);
                            close_tcp_socket(newsockfd,masterfd);
                        }

                        std::cout <<"\nHighest emotion index is "<< std::distance(emotions_count.begin(), std::max_element(emotions_count.begin(), emotions_count.end()))<<endl;
                        // close socket and remove from FD set


                        maxfd = sockfd;
                        free(read_buffer);
                        break;
                    }
                }
                else
                {
                    char write_buffer[] = "WARN: Message received not comma seperated";
                    if( tcp_write(newsockfd,write_buffer) < 0 )
                    {
                        close_tcp_socket(newsockfd, masterfd);
                        break;
                    }
                    ++fail_count;
                    continue;
                }
                free(read_buffer);
            }

            {
                char write_buffer[] = "FAIL";
                if ( tcp_write(newsockfd,write_buffer) < 0)
                {
                    close_tcp_socket(newsockfd, masterfd);
                    break;
                }
                ++fail_count;
            }
        }
        pthread_cond_signal( &cond );
        pthread_mutex_unlock( & mutex );
        printf("Waiting for connection..\n");
        newsockfd = listen_for_client(sockfd);
        FD_SET(newsockfd,&masterfd);
        printf("Connection accepted on socket %i\n",newsockfd);
        update_success = 0;
    }
    return 0;
}
