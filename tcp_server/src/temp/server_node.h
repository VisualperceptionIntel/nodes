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

#include <assert.h>

//#####################################*
/**Index Mappings for Command Unit**///#
#define IS_BASE_BUSY            1    //#
#define BASE_POSITION_X         2    //#
#define BASE_POSITION_Y         3    //#
#define BASE_DESTINATION_X      4    //#
#define BASE_DESTINATION_Y      5    //#
#define BASE_FORWARD_STEP       6    //#
#define BASE_BACKWARD_STEP      7    //#
#define BASE_ROTATE_LEFT        8    //#
#define BASE_ROTATE_RIGHT       9    //#
#define IS_LEFT_ARM_BUSY        10   //#
#define IS_RIGHT_ARM_BUSY       11   //#
#define LEFT_ARM_MOVE           12   //#
#define RIGHT_ARM_MOVE          13   //#
#define EMOTION_STATUS          14   //#
#define LIGHT_STATUS            15   //#
#define PRINT_STATUS            16   //#
#define FACE_COGNITION_STATUS   17   //#
#define CARD_READER_STATUS      18   //#
#define UPDATED                 20   //#
#define FACE_COGNITION_OUTCOME  21   //#
#define ID_CARD_NAME            22   //#
#define VISITEE_NAME            23   //#
#define APPOINTMENT_DATE        24   //#
#define APPOINTMENT_TIME        25   //#
#define APPOINTMENT_LOCATION    26   //#
#define SPEECH_REQUEST          27   //#
#define SPEECH_STATUS           28   //#
#define ARM_ACTIONS             29   //#
#define CURRENT_LATITUDE        30   //#
#define CURRENT_LONGITUDE       31   //#
#define CURRENT_ALTITUDE        32   //#
#define OBJ_FRAME               33   //#
#define OBJ_X                   34   //#
#define OBJ_Y                   35   //#
#define OBJ_Z                   36   //#
#define OBJ_HEIGHT              37   //#
#define OBJ_WIDTH               38   //#
#define WELCOME_MSG             39   //#
/*********** Buffer sizes***********///#
#define COMMAND_UNIT_LENGTH     40   //#
#define WRITE_BUFFER_LENGTH     1024 //#
#define WELCOME_BUFFER_LENGTH   1024 //#
//####################################/

//################################/
// Update frequency for mysql    //
#define MYSQL_UPDATE_TIME    60  //
//################################/

//###########################################*
#define NUM_EMOTIONS            7          //#
#define DB_USER_VISIT           4          //#
#define DB_HOST                 0          //#
#define DB_APPOINTMENT_DATETIME 1          //#
#define DB_APPOINTMENT_LOCATION 2          //#
//##########################################*/

//  Arm status:
/*#####################
  ##  UNKNOWN -   0  ##
  ##  HOME    -   1  ##
  ##  FREE    -   2  ##
  ##  WAVING  -   3  ##
  ##  WALKING -   4  ##
  ##  PICKING -   5  ##
  ##  HOLDING -   6  ##
  ##  PLACING -   7  ##
  #####################*/

//Define arm actions:
//###########################//
#define WALK    4           //#
#define PICK    5           //#
#define PLACE   7           //#
#define WAVE    3           //#
#define HOME    1           //#
/*###########################*/

/*####################*/
#define LOG_OK 0     //#
#define LOG_WARN 1   //#
#define LOG_ERROR 2  //#
/*####################*/

/*#######################################*/
#define RESET       "\033[0m"           //#
#define BLACK       "\033[30m"          //#
#define RED         "\033[31m"          //#
#define GREEN       "\033[32m"          //#
#define YELLOW      "\033[33m"          //#
#define BLUE        "\033[34m"          //#
#define MAGENTA     "\033[35m"          //#
#define CYAN        "\033[36m"          //#
#define WHITE       "\033[37m"          //#
#define BOLDBLACK   "\033[1m\033[30m"   //#
#define BOLDRED     "\033[1m\033[31m"   //#
#define BOLDGREEN   "\033[1m\033[32m"   //#
#define BOLDYELLOW  "\033[1m\033[33m"   //#
#define BOLDBLUE    "\033[1m\033[34m"   //#
#define BOLDMAGENTA "\033[1m\033[35m"   //#
#define BOLDCYAN    "\033[1m\033[36m"   //#
#define BOLDWHITE   "\033[1m\033[37m"   //#
/*#######################################*/

#define UPDATE_SUCCESS 1
#define UPDATE_WELCOME_MSG 2
#define UPDATE_FAQ_REQUEST 3



#define LOG_UPDATE_RATE 10
using namespace std;

struct command_unit              // Current Indexes:
{
    unsigned int controlid;//--------------//  0
    unsigned int is_base_busy;//-----------//  1
    float base_position_x;//---------------//  2
    float base_position_y;//---------------//  3
    float base_destination_x;//------------//  4
    float base_destination_y;//------------//  5
    unsigned int  base_forward_step;//-----//  6
    unsigned int  base_backward_step;//----//  7
    unsigned int  base_rotate_left;//------//  8
    unsigned int  base_rotate_right;//-----//  9
    unsigned int  is_left_arm_busy;//------// 10
    unsigned int  is_right_arm_busy;//-----// 11
    unsigned int  left_arm_move;//---------// 12
    unsigned int  right_arm_move;//--------// 13
    unsigned int  emotion_status;//--------// 14
    unsigned int  light_status;//----------// 15
    unsigned int  print_status;//----------// 16
    unsigned int  face_cognition_status;//-// 17
    unsigned int  card_reader_status;//----// 18
    char  created[40];//-------------------// 19
    char  updated[40];//-------------------// 20
    char*  face_cognition_outcome;//-------// 21
    char*  id_card_name;//-----------------// 22
    char*  visitee_name;//-----------------// 23
    char*  appointment_date;//-------------// 24
    char*  appointment_time;//-------------// 25
    char*  appointment_location;//---------// 26
    unsigned int speech_request;//---------// 27
    unsigned int speech_status; //---------// 28
    char*  arm_actions; //-----------------// 29
    float current_latitude;//--------------// 30
    float current_longitude;//-------------// 31
    float current_altitude;//--------------// 32
    char* obj_frame;//---------------------// 33
    float obj_x;//-------------------------// 34
    float obj_y;//-------------------------// 35
    float obj_z;//-------------------------// 36
    float obj_height;//--------------------// 37
    float obj_width;//---------------------// 38
    char* welcome_msg;//-------------------// 39
};

extern command_unit master_commands, mysql_commands, tcp_commands; // 3 instances of object




struct string_token
{
    char * tok;
};

extern boost::array<unsigned int,NUM_EMOTIONS> emotions_count;
extern char current_time[15];
extern char current_date_time[25];
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
//string log_string;

 struct log_list{
        string          log_updated;    //date and time of update
        unsigned int    log_type;       //type of log 0-OK, 1-WARNING, 2-ERROR
        string          log_status;     //
        string          log_info;       //LOG INFORMATION
        float           log_lat;        //current lattitude
        float           log_long;       //current longitude
        float           log_alt;        //current altitude
        string          log_parent_type;//source of log
        log_list*       next;           //pointer to next
};


struct faq_list{
        unsigned int    faq_id;
        string          question;
        string          answer;
        faq_list*        next;
};

extern log_list *start, *newptr, *current, *ptr;
extern faq_list *faq_start, *newfaq_ptr, *faq_current, *faq_ptr;
// defines array of string_token objects
typedef string_token str_tok_buffer[COMMAND_UNIT_LENGTH];

log_list* createNewNode(string);
void wizo_log(int,string date_time, unsigned int type,string source, string log_line, float);
void insertEnd(log_list*);
void display(log_list*,int);
void insertEnd(log_list*);
void *log_mysql_update_function(void *arg);
void wizo_log(int display_log,string date_time, unsigned int type,string source, string log_line);
void *exit_thread_function(void*);
void error(const char *msg);
int create_tcp_socket(int portno);
void close_tcp_socket(int newsockfd, fd_set& masterfd);
int listen_for_client(int sockfd);
char *tcp_read(int newsockfd);
int tcp_write(int newsockfd, char msg[WRITE_BUFFER_LENGTH]);
int  tcp_str_to_array(char* rcv_data, str_tok_buffer str_tok_buf_tmp);




void mysql_visit_update(str_tok_buffer& str_tok_buf_tmp, char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& command_update,int);
void time_stamp(char* time_buffer);

int base_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);
int arms_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);

int detect_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);

void update_master_commands(struct command_unit updated_values ,char is_updated[COMMAND_UNIT_LENGTH]);
int scanner_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);
int printer_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);
int face_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);
int lights_cmd_update(str_tok_buffer& str_tok_buf_tmp);
int obj_detect_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp);
