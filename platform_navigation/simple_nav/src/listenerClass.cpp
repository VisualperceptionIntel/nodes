#include "listenerClass.h"


void wp_clear()
{
    wp_vector.clear();
}
/*
void Listener::callback_Pose(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    double temp;
    //prevPosition = currentPose;

    tf::Quaternion q(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, msg->pose.orientation.w);
    tf::Matrix3x3(q).getRPY(currentPose.roll, currentPose.pitch, currentPose.yaw);

    currentPose.x = msg->pose.position.x;
    currentPose.y = msg->pose.position.y;
    currentPose.z = msg->pose.position.z;
    isCalled = true;


}*/
void Listener::callback_Pose(const geometry_msgs::Pose::ConstPtr &msg)
{
    double temp;

    currentPose.x = msg->position.x;
    currentPose.y = msg->position.y;
    currentPose.z = msg->position.z;
    currentPose.roll = msg->orientation.x;
    currentPose.pitch = msg->orientation.y;
    currentPose.yaw = msg->orientation.z;

    isCalled = true;


}
void Listener::callback_Twist(const geometry_msgs::Twist::ConstPtr &msg)
{

    joyTwist.x = msg->linear.x;
    joyTwist.yaw = msg->angular.z;


}



void Listener::callback_int(const std_msgs::Int16::ConstPtr &msg)
{
    dataInt = msg->data;
    
    //wp_msg.data="";
}

void Listener::callback_wp(const std_msgs::Int16::ConstPtr &msg)
{
    dataInt = msg->data;
    wp_msg.data = (dataInt == WP_AVAILABLE)? readfile_wp() : "";
    (dataInt == WP_AVAILABLE)? (pub_wp.publish(wp_msg)) : (wp_clear());
    //wp_msg.data="";
}

void Listener::callback_float(const std_msgs::Float32::ConstPtr &msg)
{
    dataFloat = msg->data;
}

/*void callback_vector(const std_msgs::Float32MultiArray::ConstPtr &vector_msg)
{
    int i = 0;
	for(std::vector<float>::const_iterator it = vector_msg->data.begin(); it != vector_msg->data.end(); ++it)
	{
		wp_array[i] = *it;
		i++;
    }
}*/
 ///------------------implement speed on another node-----------------
diffPosition_t Listener::calcdiff_moved()
{
    diffPosition_t moved_diff;
    moved_diff.linearX = sqrt(pow((currentPose.y - prevPosition.y),2 )+ pow((currentPose.x - prevPosition.x),2));
    moved_diff.angularZ = currentPose.yaw - prevPosition.yaw;
    moved_diff.angularZ = ((moved_diff.angularZ > PI) ? (moved_diff.angularZ - 2*PI) : (( moved_diff.angularZ < -PI) ? (2*PI + moved_diff.angularZ ):moved_diff.angularZ));
    return moved_diff;
}


double Listener::get_Float_MessageValue()
{
    return dataFloat;
}

int Listener::get_Int_MessageValue()
{
    return dataInt;
}

Pose Listener::get_Pose()
{
    return currentPose;
}

Pose Listener::get_Twist()
{
    return joyTwist;
}


