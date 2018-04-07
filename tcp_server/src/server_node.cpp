#include "server_node.h"
#include "MySql_conn.h"
#include <typeinfo>
command_unit master_commands, mysql_commands, tcp_commands;
videoSumm videoSummData;
profileImage profileImageData;
log_list *start, *newptr, *current, *ptr;
faq_list *faq_start, *newfaq_ptr, *faq_current, *faq_ptr;
bool exit_server;

int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_faq = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_speech_status = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_printedData_update = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_printedData_update = PTHREAD_COND_INITIALIZER;

time_t epoch,user_timer;

std::string previous_name;
bool speech_status_listen_flag;

char current_time[15];
char current_date_time[25];
char speech_status_listen_time[15];
boost::array<unsigned int,NUM_EMOTIONS> emotions_count;
char* visit_id;
char* check_in;
string checkin_status;
float batteryState;
std::vector<float> AuxPower_state;

void wizo_info(std::string text)
{
std::cout<<text<<std::endl;


}



int set_timer(struct timeval &tv, time_t sec)
{
    gettimeofday(&tv,NULL);
    tv.tv_sec+=sec;

    return 1;
}


int check_timer(struct timeval &tv, time_t sec){
    struct timeval ctv;
    gettimeofday(&ctv,NULL);

    if( (ctv.tv_sec >= tv.tv_sec) )
    {
        gettimeofday(&tv,NULL);
        tv.tv_sec+=sec;
        return 1;
    }
    else
        return 0;
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
            "visitee_name,%s,appointment_date,%s,appointment_time,%s,appointment_location,%s,speech_request,%i,face_detect,%i,latitude,%f,longitude,%f,altitude,%f,frame,%s,obj_x,%f,obj_y,%f,obj_z,%f,obj_height,%f,obj_width,%f,battery_level,%i,speech_status,%i,<EOC>",
            master_commands.is_base_busy,master_commands.base_position_x,master_commands.base_position_y,master_commands.base_destination_x,master_commands.base_destination_y,master_commands.base_forward_step,
            master_commands.base_backward_step,master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,master_commands.is_right_arm_busy,master_commands.left_arm_move,
            master_commands.right_arm_move,master_commands.arm_actions,master_commands.emotion_status,master_commands.light_status,master_commands.print_status,master_commands.face_cognition_status,master_commands.card_reader_status,
            master_commands.face_cognition_outcome,master_commands.id_card_name,master_commands.visitee_name,master_commands.appointment_date,master_commands.appointment_time,master_commands.appointment_location,
            master_commands.speech_request,master_commands.face_detect,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude,master_commands.obj_frame,master_commands.obj_x,master_commands.obj_y,master_commands.obj_z,master_commands.obj_height,master_commands.obj_width,master_commands.battery_level,master_commands.speech_status);

    return tcp_write(newsockfd,update_buffer);
}


int send_welcome_msg(int newsockfd)
{
    char msg_buffer[WELCOME_BUFFER_LENGTH];

    //time_stamp(master_commands.updated);
    sprintf(msg_buffer,"base_busy,%i,base_pos_x,%f,base_pos_y,%f,base_dest_x,%f,base_dest_y,%f,forward_step,%i,backward_step,%i,rotate_left,%i,rotate_right,%i,left_busy,%i,right_busy,%i,"
            "left_arm_move,%i,right_arm_move,%i,arm_action,%s,emotion_status,%i,light_status,%i,print_status,%i,face_cognition_status,%i,card_reader_status,%i,face_cognition_outcome,%s,id_card_name,%s,"
            "visitee_name,%s,appointment_date,%s,appointment_time,%s,appointment_location,%s,speech_request,%i,face_detect,%i,latitude,%f,longitude,%f,altitude,%f,frame,%s,obj_x,%f,obj_y,%f,obj_z,%f,obj_height,%f,obj_width,%f,"
            "welcome_msg,Welcome your Highness Sheik Mansour bin Muhammad bin Rashid, we are honoured by your attendance to the UAE Drones for good award. We hope you enjoy!,<EOC>",
            master_commands.is_base_busy,master_commands.base_position_x,master_commands.base_position_y,master_commands.base_destination_x,master_commands.base_destination_y,master_commands.base_forward_step,
            master_commands.base_backward_step,master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,master_commands.is_right_arm_busy,master_commands.left_arm_move,
            master_commands.right_arm_move,master_commands.arm_actions,master_commands.emotion_status,master_commands.light_status,master_commands.print_status,master_commands.face_cognition_status,master_commands.card_reader_status,
            master_commands.face_cognition_outcome,master_commands.id_card_name,master_commands.visitee_name,master_commands.appointment_date,master_commands.appointment_time,master_commands.appointment_location,
            master_commands.speech_request,master_commands.face_detect,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude,master_commands.obj_frame,master_commands.obj_x,master_commands.obj_y,master_commands.obj_z,master_commands.obj_height,master_commands.obj_width);


    return tcp_write(newsockfd,msg_buffer);
}

int send_msg_json(int newsockfd)
{
    char msg_buffer[WRITE_BUFFER_LENGTH ];

    //time_stamp(master_commands.updated);
    sprintf(msg_buffer,"{\"control_unit\": {\n\"base_busy\": %i,\n\"base_pos_x\": %f,\n\"base_pos_y\": %f,\n\"base_dest_x\": %f,\n\"base_dest_y\": %f,\n\"forward_step\": %i,\n\"backward_step\": %i,\n\"rotate_left\": %i,\n\"rotate_right\": %i,\n\"left_busy\": %i,\n\"right_busy\": %i,\n\""
            "left_arm_move\": %i,\n\"right_arm_move\": %i,\n\"arm_action\": \"%s\",\n\"emotion_status\": %i,\n\"light_status\": %i,\n\"print_status\": %i,\n\"face_cognition_status\": %i,\n\"card_reader_status\": %i,\n\"face_cognition_outcome\": \"%s\",\n\"id_card_name\": \"%s\",\n\""
            "visitee_name\": \"%s\",\n\"appointment_date\": \"%s\",\n\"appointment_time\": \"%s\",\n\"appointment_location\": \"%s\",\n\"speech_request\": %i,\n\"face_detect\": %i,\n\"latitude\": %f,\n\"longitude\": %f,\n\"altitude\": %f,\n\"frame\": \"%s\",\n\"obj_x\": %f,\n\"obj_y\": %f,\n\"obj_z\": %f,\n\"obj_height\": %f,\n\"obj_width\": %f,\n\"battery_level\": %i,\n\"speech_status\": %i}}",
            master_commands.is_base_busy,master_commands.base_position_x,master_commands.base_position_y,master_commands.base_destination_x,master_commands.base_destination_y,master_commands.base_forward_step,
            master_commands.base_backward_step,master_commands.base_rotate_left,master_commands.base_rotate_right,master_commands.is_left_arm_busy,master_commands.is_right_arm_busy,master_commands.left_arm_move,
            master_commands.right_arm_move,master_commands.arm_actions,master_commands.emotion_status,master_commands.light_status,master_commands.print_status,master_commands.face_cognition_status,master_commands.card_reader_status,
            master_commands.face_cognition_outcome,master_commands.id_card_name,master_commands.visitee_name,master_commands.appointment_date,master_commands.appointment_time,master_commands.appointment_location,
            master_commands.speech_request,master_commands.face_detect,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude,master_commands.obj_frame,master_commands.obj_x,master_commands.obj_y,master_commands.obj_z,master_commands.obj_height,master_commands.obj_width,master_commands.battery_level,master_commands.speech_status);


    return tcp_write(newsockfd,msg_buffer);
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

//    for(int l=0; l<j; ++l)
//    {
//        if( str_tok_buf_tmp[l].tok == NULL )
//            break;
//        else
//            printf("str_tok_buf_tmp[%i]: %s\n",l,str_tok_buf_tmp[l].tok);
//    }

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

//
//    MysqlConn c;
//    while(!exit_server)
//    {
//        std::string query_out_string="";
//        pthread_mutex_lock( &mutex_faq );
//        c.connect_MySql();
//        query_out_string = c.SELECT("SELECT * FROM Wizo.arm_position_config");
//        c.save_to_file(ARMS_FILE_NAME,query_out_string);
//        query_out_string = c.SELECT("select waypoints_data.waypoints from waypoints_data,location,call_wizo where waypoints_data.locationid = call_wizo.locationid and location.locationid=waypoints_data.locationid order by waypintid");
//        c.save_to_file(WAYPOINTS_FILE_NAME,query_out_string);
//        pthread_mutex_unlock( & mutex_faq );
//
//        sleep(DEFAULT_VALUE_RETRIEVE_RATE);
//    }
//    c.~MysqlConn();
//

//read the faq list from datbase
string retrive_faq_from_db()
{
//    MysqlConn c;
//    std::string query_out_string="";
//    c.connect_MySql();
//    query_out_string = c.SELECT("SELECT FAQid,question,answer FROM Wizo.FAQ");
//    c.save_to_file(FAQ_FILE_NAME,query_out_string);

    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    //delete_faq(faq_start);
    faq_start = NULL;
    faq_list *faq_from_db;
    std::string s="";

    connection = mysql_init(NULL);

    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        //exit(1);
        return "";
    }

    char* query = new char[512];
    sprintf(query,"select count(FAQid) from Wizo.FAQ");
    printf("%s\n",query);


    if(mysql_query(connection,query ))
    {
        ROS_INFO("Query Error: %s", mysql_error(connection));
        //exit(1);
    }

    result = mysql_use_result(connection);
    row = mysql_fetch_row(result);
    int row_count = atoi(row[0]);
    printf("%i\n",row_count);

    mysql_free_result(result);

    sprintf(query,"SELECT FAQid,question,answer FROM Wizo.FAQ");

    if(mysql_query(connection,query ))
    {
        ROS_INFO("Query Error: %s", mysql_error(connection));
        //exit(1);
    }

    result = mysql_use_result(connection);

    s="";
    string row1, row2,row3;

    for(int i=0; i <= row_count; ++i)
    {
        //std::stringstream ss;
        row = mysql_fetch_row(result);


        if(row <= 0)
        {
            printf("No row returned, table not updated.\n");
            if(s!="")
            {
                s.erase(s.end()-1,s.end());
                s=s+"]}";
            }
            return s;
        }

        row1 = row[1];
        row2 = row[2];
        row3 = row[3];
        if(s=="") s="{\"FAQ\" : [";
        if(i==row_count)
            //s=s+"{\"id\": \""+row[0]+"\",\"answer\": \""+row[2]+"\",\"question\": \""+row[1]+"\"}";
            s=s+"{\"id\": \""+row[0]+"\",\"answer\": \""+row[2]+"\",\"question\": \""+row[1]+"\"}";
        else
            s=s+"{\"id\": \""+row[0]+"\",\"answer\": \""+row[2]+"\",\"question\": \""+row[1]+"\"},";
    }
    s=s+"]}";
    mysql_free_result(result);
    mysql_close(connection);
    return s;
}

//read the fields from the database and return as a string.
string read_from_mysql_db(string query_type, char* name,struct command_unit& main_command)
{
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    std::string s="";

    connection = mysql_init(NULL);

printf("connecting to sql here\n");
    if (mysql_real_connect(connection, "107.180.100.50", "wizoApp", "321Wizo..","Wizo", 0, NULL, 0) == NULL)
    {
    printf("Sql connection error\n");
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        //exit(1);
        return "";
    }


 printf("Sql connected\n");
 int i;
 //cin>>i;
    if(query_type=="visit")
    {
        //and user_visit.start_datetime >= now() and user_visit.start_datetime < curdate()+1
    printf("querying 1 \n");
        printf("%s\n",name);
        char* query = new char[1024];
        sprintf(query,"SELECT first_name, start_datetime,pickup_locationid, visitid, checkedin FROM Wizo.user_visit, Wizo.user_data WHERE user_visit.visitor_name = '%s' and user_data.userid = user_visit.host_userid and user_visit.start_datetime >= now() and user_visit.start_datetime < curdate()+1 and user_data.isdeleted=0 ORDER BY user_visit.start_datetime desc",name);

        if(mysql_query(connection,query ))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }
    }
   else
    {
        printf("querying 2 \n");
        if(mysql_query(connection, "SELECT * FROM Wizo.control_unit ORDER BY control_unit.controlid desc limit 1"))
        {
            ROS_INFO("Query Error: %s", mysql_error(connection));
            //exit(1);
        }
    }

    result = mysql_use_result(connection);
    printf("querying 2 completed \n");
//    for(int i=0; i < mysql_field_count(connection); ++i)
//    {
        std::stringstream ss;
        row = mysql_fetch_row(result);
    printf("fetching complete \n");
        if(row <= 0)
        {
            printf("No row returned, table not updated.\n");
            mysql_close(connection);
            return s;
        }
        printf("here");

        for(int j=0; j < mysql_num_fields(result); ++j)
        {
        string field_s;
            //ss << row[j];
           // cout<<ss<<endl;
           if (row[j] == NULL) field_s="NULL";
           else field_s = row[j];


            if( j == mysql_num_fields(result)-1 )
            { s = s + field_s;}
            else s = s + field_s + ",";

            //cout<<s<<endl;
        }
        //update_mysql_db();

mysql_free_result(result);


        char query_str[2048];
    sprintf(query_str, "UPDATE Wizo.control_unit SET"
            " control_unit.is_base_busy=%i,"
            " control_unit.base_position_x=%f,"
            " control_unit.base_position_y=%f,"
            " control_unit.base_forward_step=%i,"
            " control_unit.base_backward_step=%i,"
            " control_unit.base_rotate_left=%i,"
            " control_unit.base_rotate_right=%i,"
            " control_unit.is_left_arm_busy=%i,"
            " control_unit.is_right_arm_busy=%i,"
            " control_unit.left_arm_move=%i,"
            " control_unit.right_arm_move=%i,"
            " control_unit.emotion_status=%i,"
            " control_unit.light_status=%i,"
            " control_unit.print_status=%i,"
            " control_unit.face_cognition_status=%i,"
            " control_unit.face_cognition_outcome='%s',"
            " control_unit.card_reader_status=%i,"
            " control_unit.battery_level=%i,"
            " control_unit.id_card_name='%s',"
            " control_unit.face_detect=%i,"
            " control_unit.arm_action='%s',"
            " control_unit.obj_frame='%s',"
            " control_unit.obj_x=%f,"
            " control_unit.obj_y=%f,"
            " control_unit.obj_z=%f,"
            " control_unit.obj_height=%f,"
            " control_unit.obj_width=%f,"
            " control_unit.speech_status=%i,"
            " control_unit.updated='%s'"
            " WHERE control_unit.controlid=1",main_command.is_base_busy,main_command.base_position_x,
            main_command.base_position_y,main_command.base_forward_step,main_command.base_backward_step,
            main_command.base_rotate_left,main_command.base_rotate_right,main_command.is_left_arm_busy,
            main_command.is_right_arm_busy,main_command.left_arm_move,main_command.right_arm_move,
            main_command.emotion_status,main_command.light_status,main_command.print_status,
            main_command.face_cognition_status,main_command.face_cognition_outcome,main_command.card_reader_status,
            main_command.battery_level,main_command.id_card_name,main_command.face_detect,main_command.arm_actions,
            main_command.obj_frame ,main_command.obj_x,main_command.obj_y,main_command.obj_z,main_command.obj_height,
            main_command.obj_width,main_command.speech_status,
            main_command.updated);

    printf("SQL Query formed:\n%s\n",query_str);
//    int i;
//    std::cin>> i;
    if(mysql_query(connection, query_str))
    {
        fprintf(stderr,"Query Error: %s\n", mysql_error(connection));
        mysql_close(connection);
        return s;
    }


 result = mysql_use_result(connection);

        mysql_free_result(result);
        mysql_close(connection);
        //std::cout<<s<<endl;
        return s;
    //}

}


//update Mysql database once any client request update--- or update on a set interval.. store values until then.
void mysql_cmd_update(str_tok_buffer& str_tok_buf_tmp,struct command_unit& main_command)
{
    mysql_commands = command_unit();
    char is_updated[COMMAND_UNIT_LENGTH];
    char* name =new char[1];
    name[1]= '\0';


    string query= "all";
    string str = read_from_mysql_db(query,name,main_command);
    std::cout<<str;
    if( str.empty() )
        return;
    char* data_buffer = new char[str.size()+1];
    std::copy(str.begin(), str.end(), data_buffer);
    data_buffer[str.size()] = '\0';

    mysql_str_to_array(data_buffer, str_tok_buf_tmp,query,is_updated,mysql_commands);

    delete(data_buffer);
    str.clear();
//    mysql_commands=master_commands;
//    update_mysql_db();
}

void mysql_visit_update(str_tok_buffer& str_tok_buf_tmp, char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& command_update, int source,struct command_unit& main_command) //source: 1-scanner 2-cognition
{
    string query= "visit";
    string str="";
    //printf("%s\n",command_update.id_card_name);
    if(source == 1)
        str = read_from_mysql_db(query,command_update.id_card_name,main_command);

    else if(source == 2)
        str = read_from_mysql_db(query,command_update.face_cognition_outcome,main_command);

    //std::cout<<str;
    if( str.empty() || str=="" )
    {
        command_update.visitee_name= new char;
        strcpy(command_update.visitee_name,"NULL");
        command_update.appointment_date= new char;
        strcpy(command_update.appointment_date,"NULL");
        command_update.appointment_time= new char;
        strcpy(command_update.appointment_time,"NULL");
        command_update.appointment_location= new char;
        strcpy(command_update.appointment_location,"NULL");
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
        if(master_commands.speech_status == 1 )
        {
            if(epoch == 2){
                master_commands.speech_status = 2;
                epoch = 0;
            }
            user_timer = 0; //reset the user timer
           // printf("\n%ld \n",(long)epoch);
            epoch++;
        }
        else if(master_commands.speech_status == 2 )
        {
            if(user_timer == 2){
                master_commands.speech_status = 0;
//                free(master_commands.face_cognition_outcome);
//                master_commands.face_cognition_outcome = new char[5];
//                master_commands.face_cognition_outcome[4]='\0';
//                strcpy(master_commands.face_cognition_outcome,"NULL");
//                free(master_commands.id_card_name);
//                master_commands.id_card_name = new char[5];
//                master_commands.id_card_name[4]='\0';
//                strcpy(master_commands.id_card_name,"NULL");
//
//                free(master_commands.visitee_name);
//                master_commands.visitee_name = new char[5];
//                master_commands.visitee_name[4]='\0';
//                strcpy(master_commands.visitee_name,"NULL");
//                free(master_commands.appointment_date);
//                master_commands.appointment_date = new char[5];
//                master_commands.appointment_date[4]='\0';
//                strcpy(master_commands.appointment_date,"NULL");
//                free(master_commands.appointment_location);
//                master_commands.appointment_location = new char[5];
//                master_commands.appointment_location[4]='\0';
//                strcpy(master_commands.appointment_location,"NULL");
//                free(master_commands.appointment_time);
//                master_commands.appointment_time = new char[5];
//                master_commands.appointment_time[4]='\0';
//                strcpy(master_commands.appointment_time,"NULL");
//
//                master_commands.face_cognition_status = 0;
//                master_commands.card_reader_status =0;

                user_timer=0;
                epoch = 0;
            }
            user_timer++;
        }



//        if(master_commands.speech_status == 1 )
//        {
//        //master_commands.speech_status == 1;
//        epoch = 1;
//        }

        printf("                                                         \033[1;31m%s \033[0m\n\033[1A",current_time);
        sleep(1);
    }
}

void *thread_mysql_function(void *arg)
{
    str_tok_buffer sql_str_tok_buf = {NULL};
    while(1)
    {
        pthread_mutex_lock( & mutex_faq );
        printf("\033[1;31mMysql retrieval will be here\033[0m\n");
        mysql_cmd_update(sql_str_tok_buf,master_commands);
        pthread_mutex_unlock( & mutex_faq );
        sleep(20);
    }
}

void *exit_thread_function(void *arg)
{
    char text[100];
    while(1)
    {
        scanf("%s",text);
        if (text[0] == 'q'&& text[1]=='q')
        {
            exit_server=true;
            sleep(1);
            exit(1);
        }


    }
}


void *exitYAML_thread_function(void *arg)
{
    int exitMode;
    int toggle;
    bool logFile;
    int counter=0;
    while(1)
    {
        YAML::Node config = YAML::LoadFile("exit_config.yaml");
        exitMode = config["exit"].as<int>();
        toggle = config["toggle"].as<int>();
        logFile = config["logFile"].as<int>();
        if (exitMode ==1)
        {
            exit_server=true;
            //config["exit"]="0";
            if(toggle==1)
            {
                std::ofstream YAMLout("exit_config.yaml");
                YAMLout << "exit: 0\r\ntoggle: 1";
                YAMLout.close();
            }
            exit(1);
        }

        if(logFile==1 && counter == 0)
        {
            std::string str(current_time);
            str = "./src/logs/" + str + "1.log";
            freopen(str.c_str(),"a",stdout);
            counter++;
        }
    }
}


void *vs_client_thread_function(void *arg)
{
fd_set masterfd;
videoSumm summdata;
summdata = *((videoSumm * )arg);
static std::ofstream ofs_vs;
static std::string filename_vs="/home/mahi/Dropbox/Digirobotics/all_workspace/segway_sim_ws/transfer.sh";

//ssh server@192.168.1.17 "sshpass scp /home/server/DPWorldRoads-Video1.mp4* mahi@192.168.1.16:/home/mahi/Music/"

    ofs_vs.open(filename_vs.c_str(),std::ios::out);
    if (!ofs_vs)
    {
      std::cerr << "Could not open " << filename_vs << "." << std::endl;
    }
    ofs_vs<<"ssh "<<summdata.username_rc<<"@"<<summdata.remoteClient_vs<<" \"sshpass scp "<<summdata.filePath<<summdata.fileName<<"* " <<summdata.username_rs<<"@"<<summdata.remoteServer<<":"<<summdata.filePath_db<<"\"" ;
    ofs_vs.close();

    system("gnome-terminal -x sh -c 'cd /home/mahi/Dropbox/Digirobotics/all_workspace/segway_sim_ws/; ./transfer.sh'");

    std::string insert_string="";
    MysqlConn vsumm_mysql;
    //vsumm_mysql.connect_MySql();
    insert_string = "insert into Wizo.media_data ( wizoid , filename ,updated) values (1, '"+ summdata.filePath_db+ summdata.fileName+"', '"+ current_time +"')";

        if(vsumm_mysql.connect_MySql()){
        vsumm_mysql.INSERT(insert_string);
        vsumm_mysql.close_MySql();
        }

}

void *profileImg_client_thread_function(void *arg)
{
fd_set masterfd;
profileImage profImgdata;
profImgdata = *((profileImage * )arg);
static std::ofstream ofs_profImg;
static std::string filename_profImg="/home/mahi/Dropbox/Digirobotics/all_workspace/segway_sim_ws/profImg_transfer.sh";
static std::string img_localPath="/home/mahi/Dropbox/Digirobotics/all_workspace/segway_sim_ws/";
std::vector<std::vector<std::string> > mysqlRes;
//resultArray resultArray[6];
strcpy(profImgdata.lastUpdatedTime,"2016-10-07 10:10:50") ;

    std::string profImg_query="";
    std::string profImg_res="";
    MysqlConn profImg_mysql;
    //vsumm_mysql.connect_MySql();
    profImg_query = "SELECT first_name,last_name,profileimage FROM Wizo.user_data where (updated >'" +(string)profImgdata.lastUpdatedTime +"' and profileimage != '')";
    printf("query formed here\n");
    if(profImg_mysql.connect_MySql()){
    printf("connected here\n");
        profImg_res =profImg_mysql.SELECT(profImg_query);
           printf("retrived here\n");
        mysqlRes = profImg_mysql.get_mysqlRes();
        //result = mysql_use_result(connection);

        //mysql_free_result(result);
        profImg_mysql.close_MySql();

        ofs_profImg.open(filename_profImg.c_str(),std::ios::out);
        if (!ofs_profImg)
        {
          std::cerr << "Could not open " << filename_profImg << "." << std::endl;
          //return -1;
        }
        ofs_profImg<<"scp -r"<<img_localPath<<" " <<profImgdata.username_rc<<"@"<<profImgdata.remoteClient_ip<<":"<<profImgdata.destFilePath<<"\"" ;
        ofs_profImg.close();

        system("gnome-terminal -x sh -c 'cd /home/mahi/Dropbox/Digirobotics/all_workspace/segway_sim_ws/; ./profImg_transfer.sh'");

    }
    //else
        //return -2;




}

void *faq_client_thread_function(void *arg)
{
fd_set masterfd;
int faq_sockfd= *((int * )arg);
char* faq_buffer;
faq_start = NULL;
string faq=retrive_faq_from_db();


if (faq =="")
{
ifstream faq_saved_file("db_faq.txt");
//faq_saved_file.open("db_faq.txt");
string str;
// while (std::getline(faq_saved_file, str))
//  {
//    //std::getline(faq_saved_file,str);  // delim defaults to '\n'
//   // if(faq_saved_file.is_open())
//    faq = faq + str;
//   cout << faq << "++++++" << endl;
//
//  }

std::string contents((std::istreambuf_iterator<char>(faq_saved_file)),std::istreambuf_iterator<char>());
faq = contents;
faq_saved_file.close();

}


faq_buffer = new char[strlen(faq.c_str())+1];
strcpy(faq_buffer,faq.c_str());

printf("faq buffer is:%s",faq_buffer);
tcp_write(faq_sockfd,faq_buffer);

ofstream faq_file("db_faq.txt");
faq_file << faq_buffer;
faq_file.close();

free(faq_buffer);

//shutdown(faq_sockfd,SHUT_RDWR);
close(faq_sockfd);

}

//**************RETRIEVE DEFAULT VALUES AT THE BEGENING OF PROGRAM*******************************************
void *mysql_default_retrieve_function(void *arg)
{
    MysqlConn c;
    while(!exit_server)
    {
        std::string query_out_string="";
        pthread_mutex_lock( &mutex_faq );
        if(c.connect_MySql()){
        query_out_string = c.SELECT("SELECT * FROM Wizo.arm_position_config");
        c.save_to_file(ARMS_FILE_NAME,query_out_string);
        query_out_string = c.SELECT("select waypoints_data.waypoints from waypoints_data,location,call_wizo where waypoints_data.locationid = call_wizo.locationid and location.locationid=waypoints_data.locationid order by waypintid");
        c.save_to_file(WAYPOINTS_FILE_NAME,query_out_string);
        c.close_MySql();
        }
        pthread_mutex_unlock( & mutex_faq );

        sleep(DEFAULT_VALUE_RETRIEVE_RATE);
    }
    c.~MysqlConn();
}
//*************************************************************************************************************

void *thread_printed_data_update_function(void *arg)
{

    MysqlConn c;
//     visit_id = 2;
     std::string query_update_string="";
     std::string visit_id_str="";

    while(1)
    {
    //    visit_id += 1;
        //printf("waiting for condition#####################################################################\n");
        pthread_mutex_lock( & mutex_printedData_update );
        pthread_cond_wait(&cond_printedData_update, &mutex_printedData_update);
        std::string visit_id_str(visit_id);
        query_update_string="update Wizo.user_visit set user_visit.checkedin="+checkin_status+" where user_visit.visitid="+ visit_id_str;
        pthread_mutex_unlock( & mutex_printedData_update );
        //printf("got the condition signal************************************\n");
        if(c.connect_MySql()){
        std::cout<<query_update_string<<std::endl;
        c.UPDATE(query_update_string);
        //c.close_MySql();
        }

    }

    c.~MysqlConn();
}


void initialize_master_cmd(ros::NodeHandle nh)
{
 int param_int;
 std::string param;
    nh.getParam("/controlid",param_int);
    master_commands.controlid = param_int;
    nh.getParam("/is_base_busy",param_int);
    master_commands.is_base_busy = param_int;
    nh.getParam("/base_position_x",master_commands.base_position_x);
    nh.getParam("/base_position_y",master_commands.base_position_y);
    nh.getParam("/base_destination_x",master_commands.base_destination_x);
    nh.getParam("/base_destination_y",master_commands.base_destination_y);
    nh.getParam("/base_forward_step",param_int);
    master_commands.base_forward_step = param_int;
    nh.getParam("/base_backward_step",param_int);
    master_commands.base_backward_step = param_int;
    nh.getParam("/base_rotate_left",param_int);
    master_commands.base_rotate_left = param_int;
    nh.getParam("/base_rotate_right",param_int);
    master_commands.base_rotate_right = param_int;
    nh.getParam("/is_left_arm_busy",param_int);
    master_commands.is_left_arm_busy = param_int;
    nh.getParam("/is_right_arm_busy",param_int);
    master_commands.is_right_arm_busy = param_int;
    nh.getParam("/left_arm_move",param_int);
    master_commands.left_arm_move = param_int;
    nh.getParam("/right_arm_move",param_int);
    master_commands.right_arm_move = param_int;
    nh.getParam("/emotion_status",param_int);
    master_commands.emotion_status = param_int;
    nh.getParam("/light_status",param_int);
    master_commands.light_status = param_int;
    nh.getParam("/print_status",param_int);
    master_commands.print_status = param_int;
    nh.getParam("/face_cognition_status",param_int);
    master_commands.face_cognition_status = param_int;
    nh.getParam("/card_reader_status",param_int);
    master_commands.card_reader_status = param_int;
    master_commands.face_cognition_outcome = new char;
    nh.getParam("/face_cognition_outcome",param);
    strcpy(master_commands.face_cognition_outcome,param.c_str());
    master_commands.id_card_name = new char;
    nh.getParam("/id_card_name",param);
    strcpy(master_commands.id_card_name,param.c_str());
    master_commands.visitee_name = new char;
    nh.getParam("/visitee_name",param);
    strcpy(master_commands.visitee_name,param.c_str());
    master_commands.appointment_date = new char;
    nh.getParam("/appointment_date",param);
    strcpy(master_commands.appointment_date,param.c_str());
    master_commands.appointment_time = new char;
    nh.getParam("/appointment_time",param);
    strcpy(master_commands.appointment_time,param.c_str());
    master_commands.appointment_location = new char;
    nh.getParam("/appointment_location",param);
    strcpy(master_commands.appointment_location,param.c_str());
    nh.getParam("/speech_request",param_int);
    nh.getParam("/speech_request",param_int);
    nh.getParam("/face_detect",param_int);
    nh.getParam("/face_detect",param_int);
    master_commands.arm_actions = new char;
    nh.getParam("/arm_actions",param);
    strcpy(master_commands.arm_actions,param.c_str());
    nh.getParam("/current_latitude",master_commands.current_latitude);
    nh.getParam("/current_longitude",master_commands.current_longitude);
    nh.getParam("/current_altitude", master_commands.current_altitude);
    master_commands.obj_frame  = new char;
    nh.getParam("/obj_frame",param);
    strcpy(master_commands.obj_frame,param.c_str());
    nh.getParam("/obj_x",master_commands.obj_x);
    nh.getParam("/obj_y", master_commands.obj_y );
    nh.getParam("/obj_z",master_commands.obj_z);
    nh.getParam("/obj_height",master_commands.obj_height);
    nh.getParam("/obj_width",master_commands.obj_width);
    nh.getParam("/battery_level", master_commands.battery_level);
    nh.getParam("/speech_status",master_commands.speech_status);
    nh.getParam("/username_rs",videoSummData.username_rs);
    nh.getParam("/remoteServer",videoSummData.remoteServer);
    nh.getParam("/filePath_db",videoSummData.filePath_db);
}
void currentPose_cb(const move_base_msgs::MoveBaseActionFeedback& msg )
{
printf("saving here\n");
ofstream currentPosefile("currentPose.txt");
currentPosefile<<msg.feedback.base_position.pose.position.x;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.position.y;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.position.z;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.orientation.x;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.orientation.y;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.orientation.z;
currentPosefile<<"\r\n";
currentPosefile<<msg.feedback.base_position.pose.orientation.w;
currentPosefile.close();
}

void _handle_propulsion_power_cb(const segway_msgs::Propulsion& msg )
{
    batteryState = msg.min_propulsion_battery_soc;
    printf("%f",batteryState);
}

void _handle_aux_power_cb(const segway_msgs::AuxPower& msg )
{
    AuxPower_state = msg.aux_soc;
}



int main (int argc, char** argv)
{
freopen("Error.log","a",stderr);
///Initialise Ros & Node Handler
    ros::init(argc, argv, "server_node");
    ros::NodeHandle nh;
///Initialise Server Exit status
    exit_server = false;
///Initialise Master command with initial values
    initialize_master_cmd(nh);

    ros::Subscriber currentPose_sub = nh.subscribe("/segway_move_base/feedback",1,currentPose_cb);
    ros::Subscriber batteryProplsion_sub = nh.subscribe("/segway/feedback/propulsion", 1, _handle_propulsion_power_cb);
    ros::Subscriber batteryAux_sub = nh.subscribe("/segway/feedback/aux_power", 1, _handle_aux_power_cb);



    char log_buff[20];
    start = NULL;
    int sockfd, newsockfd, maxfd, portno; //Socket file descriptors and port number
    char* read_buffer;
    char write_buffer[512]; // buffer array of size 256 for sending messages
    int n, update_success=0;
    str_tok_buffer str_tok_buf = {NULL};
    ros::Duration d(0.001); // 1000Hz
    struct timeval tv;
    tv.tv_sec = 2;

///******************************************************check port configuration****************************************************************************
  ////////  portno = check_config();
  nh.getParam("/port",portno );
///************************************************************PORT CONFIG END*******************************************************************************



///***************************************Create and initialise Threads for different functions**************************************************************
    pthread_t timer_thread, mysql_thread, log_mysl_update_thread,exit_thread,exitYAML_thread,local_db_thread, faq_client_thread[25], vs_client_thread[25], profileImg_client_thread[25], mysql_default_retrieve_thread;
    pthread_t printed_data_update_thread;
    int ret_log =pthread_create(&log_mysl_update_thread,NULL,&log_mysql_update_function,NULL);
    int ret_exit = pthread_create(&exit_thread,NULL,&exit_thread_function,NULL);
   // int ret_exitYAML = pthread_create(&exitYAML_thread,NULL,&exitYAML_thread_function,NULL);
    int ret_mysql_default = pthread_create(&mysql_default_retrieve_thread,NULL,&mysql_default_retrieve_function,NULL);
    int ret=pthread_create(&timer_thread,NULL,&thread_timer_function,NULL);
    int ret_msql=pthread_create(&mysql_thread,NULL,&thread_mysql_function,NULL);
    int ret_print_update_db= pthread_create(&printed_data_update_thread,NULL,&thread_printed_data_update_function,NULL);

///**********************************************************************END*********************************************************************************
    //char * p = &current_date_time;

    sprintf(log_buff,"port nummber: %i",portno);
    wizo_log(0,current_time,LOG_OK,"Server started",log_buff,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude);


    sockfd = create_tcp_socket(portno);
    listen(sockfd,10);
    newsockfd = listen_for_client(sockfd);

    if (-1 == (fcntl(sockfd, F_SETFD, O_NONBLOCK)))
    {
        printf("ERROR Failed to set nonblock flag\n");
        exit(1);
    }


    while(ros::ok())
    {
        fd_set sockset, masterfd;
        FD_ZERO(&sockset);
        FD_ZERO(&masterfd);
        FD_SET(sockfd, &masterfd);
        FD_SET(newsockfd, &masterfd);

        maxfd = newsockfd;

        pthread_mutex_lock( &mutex );

        memcpy(&sockset,&masterfd,sizeof(masterfd));
        int fail_count = 0;
        videoSummData.filePath = "";

        while( update_success < 1 )
        {
            if( fail_count > 2 )
            {
                close_tcp_socket(newsockfd,masterfd);
                break;
            }


            printf("Waiting for message from client on socket %i\n",newsockfd);
            int result = select(newsockfd + 1, &sockset, NULL, NULL, &tv);

            if (result < 0)
            {
                printf("\nWARNING: No Read Event on Any Socket\n");
            }
            else if (FD_ISSET(newsockfd, &sockset)) //if (result == 1) {
            {
                printf("Read Event on Socket %i, I/O in progress...\n",newsockfd);
                read_buffer = new char[CLIENT_READ_BUFFER_SIZE];
                read_buffer = tcp_read(newsockfd);

                printf("\n\nMSG RECEIVED: \n\"%s\"\n\n",read_buffer);

                wizo_log(0,current_time,LOG_OK,"client msg received",read_buffer,master_commands.current_latitude,master_commands.current_longitude,master_commands.current_altitude);

                // Split received msg and update cmd structures.
                if(tcp_str_to_array(read_buffer,str_tok_buf)>0 && read_buffer!=NULL)
                {
                    switch(str_tok_buf[0].tok[0])
                    {
                    case 'a': ///Arms
                        update_success = arms_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 'b': ///Base
                        update_success = base_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 'd': ///Detect
                        update_success = detect_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 'f': ///Face
                        update_success = face_cmd_update(tcp_commands, str_tok_buf);
                        if (update_success==1)
                            update_success=UPDATE_JSON_MSG;
                        break;
                    case 'p': ///Printer
                        update_success = (str_tok_buf[0].tok[2] == 'i' ) ? (printer_cmd_update(tcp_commands, str_tok_buf)) : UPDATE_PROFILE_REQUEST;
                        //update_success = printer_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 's': ///Scanner
                        pthread_cond_signal( &cond );
                        update_success = scanner_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 'l': ///Lights
                        update_success = lights_cmd_update(tcp_commands,str_tok_buf);
                        break;
                    case 'o': ///Object_detect
                        update_success =obj_detect_cmd_update(tcp_commands, str_tok_buf);
                        break;
                    case 'w': ///Welcome
                        update_success =UPDATE_WELCOME_MSG;
                        break;
                    case 'm': ///Master_request
                        if(strcmp(str_tok_buf[1].tok,"face")==0)
                        update_success=UPDATE_JSON_MSG;
                        else
                        update_success = UPDATE_SUCCESS;
                        printf("Values Requested\n");
                        break;
                    case 'q': ///QA - Question and Answers
                        update_success = UPDATE_FAQ_REQUEST;
                        printf("Values Requested\n");
                        break;
                    case 'v': ///video_s - Video summarization

                        videoSummData.remoteClient_vs = str_tok_buf[1].tok;
                        videoSummData.username_rc = str_tok_buf[2].tok;
                        videoSummData.filePath = str_tok_buf[3].tok;
                        videoSummData.fileName = str_tok_buf[4].tok;
                        videoSummData.sock = newsockfd;
                        update_success = UPDATE_VSUMM_REQUEST;
                        printf("Values Requested\n");
                        break;
                    default: ///default
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

                        if (update_success == UPDATE_JSON_MSG)
                        {
                            send_msg_json(newsockfd);
                            close_tcp_socket(newsockfd,masterfd);

                        }
                        else if (update_success == UPDATE_FAQ_REQUEST)
                        {
//                        int *arg = &newsockfd;
                            int ret_faq = pthread_create(&faq_client_thread[newsockfd],NULL,&faq_client_thread_function,&newsockfd);


                        }
                        else if (update_success == UPDATE_VSUMM_REQUEST)
                        {
//                        int *arg = &newsockfd;
                            //int ret_vs = pthread_create(&vs_client_thread[newsockfd],NULL,&vs_client_thread_function,&newsockfd);
                            close(newsockfd);
                             int ret_vs = pthread_create(&vs_client_thread[newsockfd],NULL,&vs_client_thread_function,&videoSummData);

                        }
                        else if (update_success == UPDATE_PROFILE_REQUEST)
                        {
                            profileImageData.remoteClient_ip = str_tok_buf[2].tok;
                            profileImageData.username_rc = str_tok_buf[5].tok;
                            profileImageData.destFilePath = str_tok_buf[7].tok;
                            profileImageData.sock = newsockfd;
                            int ret_profImg = pthread_create(&profileImg_client_thread[newsockfd],NULL,&profileImg_client_thread_function,&profileImageData);

//                            char write_buffer[] = "WARN: Message received not comma seperated";
//                            if( tcp_write(newsockfd,write_buffer) < 0 )
//                            {
//                                close(newsockfd);
//                                break;
//                            }
//                        int *arg = &newsockfd;
                            //int ret_vs = pthread_create(&vs_client_thread[newsockfd],NULL,&vs_client_thread_function,&newsockfd);


                        }
                        else
                        {
                            printf("sending");
                            send_master_commands_update(newsockfd);
                            printf("sent");
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
