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
#include "std_msgs/Float32.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float32MultiArray.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <tf/transform_listener.h>
#include <tf_conversions/tf_eigen.h>


#include <string.h>     //strlen
#include <string>       //string

#define OP_MODE_JOYSTICK    1
#define OP_MODE_AUTO        2
#define OP_MODE_UI          3
#define OP_MODE_IDLE        4

extern const unsigned int WP_AVAILABLE;
extern const unsigned int WP_UNAVAILABLE;

extern const unsigned int GOAL_PENDING;
extern const unsigned int GOAL_ACTIVE;
extern const unsigned int GOAL_PREEMPTED;
extern const unsigned int GOAL_SUCCEEDED;
extern const unsigned int GOAL_ABORTED;

#define PI 3.14159

using namespace std;
extern float wp_array[100];
extern string readfile_wp();
extern std_msgs::String wp_msg;
extern void ignore();
extern ros::Publisher pub_wp;
extern bool _wp_available;
extern vector<vector<double> >wp_vector;



struct Pose{
    double x;
    double y;
    double z;
    double roll;
    double pitch;
    double yaw;
    };

struct cmd_struct{
    double linearX;
    double angularZ;
    };
typedef cmd_struct diffPosition_t;

class Listener
{
private:
    int dataInt;
    double dataFloat;


public:
    void callback_Pose(const geometry_msgs::Pose::ConstPtr &msg);
    void callback_Twist(const geometry_msgs::Twist::ConstPtr &msg);
    void callback_float(const std_msgs::Float32::ConstPtr &float_msg);
    void callback_int(const std_msgs::Int16::ConstPtr &int_msg);
    void callback_wp(const std_msgs::Int16::ConstPtr &int_msg);
    void callback_vector(const std_msgs::Float32MultiArray::ConstPtr &vector_msg);
    Pose currentPose; /// struct for pose
    //Obstacle closesetObstacle;
    void Publish(geometry_msgs::Twist msg_);
    int get_Int_MessageValue();
    double get_Float_MessageValue();
    Pose get_Pose();
    Pose get_Twist();
    diffPosition_t calcdiff_moved();
    Pose currPosition;
    Pose prevPosition;
    Pose joyTwist;
    bool isCalled;
};
