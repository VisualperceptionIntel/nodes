#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
//#include "wizo_msgs/array1d.h"
#include "wizo_msgs/array2d.h"
#include "wizo_msgs/array1d.h"
#include "wizo_msgs/basedata.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "colours.h"
//#include "segway_msgs/Propulsion.h"
//#include "segway_msgs/AuxPower.h"
//#include "segway_msgs/ConfigCmd.h"
#include <cctype>
//#include "jsoncpp/json.h"
//#include <jsoncpp/json/reader.h>
//#include <jsoncpp/json/writer.h>
//#include <jsoncpp/json/value.h>

//#include <pcl.h>

#include "listenerClass.h"

#define OP_MODE_JOYSTICK    1
#define OP_MODE_AUTO        2
#define OP_MODE_UI          3
#define OP_MODE_IDLE        4

const unsigned int WP_AVAILABLE   = 1;
const unsigned int WP_UNAVAILABLE = 0;

const unsigned int GOAL_PENDING   = 1;
const unsigned int GOAL_ACTIVE    = 2;
const unsigned int GOAL_PREEMPTED = 3;
const unsigned int GOAL_SUCCEEDED = 4;
const unsigned int GOAL_ABORTED   = 5;

#define MAX_OBSTACLE_DIST   7
#define STOP_OBSTACLE_DIST  0.9
#define MAX_TURN_ANGLE   3.14
#define MIN_TURN_ANGLE  0.1
#define MAX_LINEAR_X  0.3
#define PI 3.14159


using namespace std;


///----------------------------------------Structs----------------------------------------------------


struct Obstacle{
    double front_obs;
    double back_obs;
    double left_obs;
    double right_obs;
    };


///------------------------------------------Class-----------------------------------------------------



///---------------------------------------Ros Params---------------------------------------------------
static int _mode;
    //private_nh.getParam("waypoint_file" , _waypoint_file);
    //private_nh.getParam("retryCount"    , _retryCount );
static float _angularSpeedRadPS=1;
static float _linearSpeedMPS=1;
static float _pubRate=7;
static float _collisionBoxWidth=3;
static float _collisionBoxDist=3; //FROM VELODYNE
static float _poseTolerance = 0.5;
static float _destinationTolerance =1;
static std::string _waypoint_file="_waypoints.txt";
static std::string _obsFrontTopic ="/obstacle/front" ;
static std::string  _obsBackTopic="/obstacle/back";
static std::string  _obsLeftTopic ="/obstacle/left";
static std::string  _obsRightTopic ="/obstacle/right";
static std::string  _waypointTopic="/waypointTopic";
static std::string  _navModeTopic ="/wp_request_status";

///----------------------------------------Listeners----------------------------------------------------
    ///Define the subscribers here
    Listener listener_localizer;
    Listener listener_obstacleFront;
    Listener listener_obstacleBack;
    Listener listener_obstacleLeft;
    Listener listener_obstacleRight;
    Listener listener_waypoint;
    Listener listener_navMode;
    Listener listener_joyStick;
    Listener listener_wpMode;

///---------------------------------------Subscribers---------------------------------------------------
    ros::Subscriber localizer_sub;
    ros::Subscriber obstacleFront_sub;
    ros::Subscriber obstacleBack_sub;
    ros::Subscriber obstacleLeft_sub;
    ros::Subscriber obstacleRight_sub;
    ros::Subscriber waypoint_sub;
    ros::Subscriber navMode_sub;
    ros::Subscriber wpLoad_sub;

    ros::Publisher pub_cmd;
    ros::Publisher pub_base_status;
    ros::Publisher pub_wp;
    ros::Publisher pub_patrol_status;
    ros::Publisher pub_tfndtpose;
    ros::Publisher pub_obstacle_Status;

///----------------------------------------Variables----------------------------------------------------

    Obstacle currentObstacle;
    Pose currentPose;
    //float wp_array[100];
    int currentNavMode = OP_MODE_IDLE;
    vector <double>  nextPoint ;//= new vector <d> (2);
    vector<vector<double> >wp_vector;
    string globalWp;
    int navMode;
    bool _wp_available;
    cmd_struct currentSpeed;
    int waypointCount;
    bool wp_eof;
    float reqYaw;
    float nxtX;
    float nxtY;

///--------------------------------------Ros Variables--------------------------------------------------
    wizo_msgs::array2d wpoints;
    std_msgs::String wp_msg;

///----------------------------------------Functions----------------------------------------------------
 string readfile_wp();
 void ignore();


