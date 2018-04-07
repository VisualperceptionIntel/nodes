#include "wp_navigation.h"
#include <fstream>
using namespace std;


string readfile_wp()
{
        fstream infile (_waypoint_file.c_str());
        string wp_str = "";
        if(infile)
        {
        wp_vector.clear();
        string s;
        while(getline (infile,s))
        {
            wp_str = (wp_str != "")? (wp_str + ","):(wp_str);
            stringstream ss(s);
            double i;
            vector<double> vect;
            wizo_msgs::array1d point_n;
            wp_str = wp_str +  "["+ss.str() + "]";
            while (ss >> i)
            {

                vect.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            wp_vector.push_back(vect);
        }
       // std::remove(_waypoint_file.c_str());
        cout<<wp_str<<endl;

        }
	waypointCount = wp_vector.size();
        return wp_str;
}

double calcDistance(double  cx,double  cy,double nx,double ny)
{
    nxtX = nx;
    nxtY = ny;
    double dist;
    printf("cx : %f, cy : %f, nx : %f, ny : %f \n\n",cx,cy,nx,ny);
    dist = sqrt(((nx-cx)*(nx-cx)) + ((ny-cy)*(ny-cy)));
    printf("The distance = %f \n\n",dist);
    return dist;
}

double calcDiffPose(Pose * _currentPose, vector<double> _nextPoint)
{
    double dx = _nextPoint[0] - _currentPose->x;
    double dy = _nextPoint[1] - _currentPose->y;
    reqYaw = atan2(dy, dx);
    double dpose = reqYaw - _currentPose->yaw;
    dpose = ((dpose > PI) ? (dpose - 2*PI) : (( dpose < -PI) ? (2*PI + dpose ):dpose));

return dpose;
}

double adaptiveSpeed(float _dpose, Obstacle * _currentObstacle)
{
    double linAdaptSpeed = 0;
    double linAdaptSpeed_obs = 0;
    double linAdaptSpeed_curve = 0;
   // double turnFactor = 0;
   // turnFactor = PI/2 -( (_dpose*PI/2) / 7);


    linAdaptSpeed_obs = (_currentObstacle->front_obs >= MAX_OBSTACLE_DIST) ? 1 : (_currentObstacle->front_obs <= STOP_OBSTACLE_DIST) ? 0 : _currentObstacle->front_obs / sqrt(9 + pow(_currentObstacle->front_obs,2)) ;

	std_msgs::String msg;

    if(linAdaptSpeed_obs<1){
	msg.data= "1";
	pub_obstacle_Status.publish(msg);
    }else{
	msg.data= "0";
	pub_obstacle_Status.publish(msg);
	}

    linAdaptSpeed_curve = (abs(_dpose) >= MAX_TURN_ANGLE) ? 1 : (abs(_dpose) <= MIN_TURN_ANGLE) ? 0 : 1- (_dpose/ sqrt(3 + pow(_dpose,2))) ;

    linAdaptSpeed = (linAdaptSpeed_obs < linAdaptSpeed_curve) ? linAdaptSpeed_obs : linAdaptSpeed_curve;
    printf(BOLDRED"linAdaptSpeed_obs: %f ------------------------------- linAdaptSpeed_curve: %f\n",linAdaptSpeed_obs,linAdaptSpeed_curve);

    return linAdaptSpeed;
}


cmd_struct navigate(Pose * _currentPose, vector<double> _nextPoint, Obstacle * _currentObstacle)
{
    cmd_struct reqSpeed;
    float _dpose;
    
   _dpose = calcDiffPose(_currentPose,_nextPoint);
    printf("dpose: %f\n",_dpose);

    reqSpeed.linearX =  MAX_LINEAR_X * adaptiveSpeed(_dpose,_currentObstacle);
    reqSpeed.angularZ = ((_dpose)) * (1/ PI);

    return reqSpeed;
}

void ignore()
{
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "wp_navigation");
    ros::NodeHandle node;
    ros::NodeHandle private_nh("~");


    ///get the rosparam here
    private_nh.getParam("mode"          , _mode);
    private_nh.getParam("waypoint_file" , _waypoint_file);
//    private_nh.getParam("retryCount"    , _retryCount );
    private_nh.getParam("angularSpeedRadPS" , _angularSpeedRadPS);  //*****
    private_nh.getParam("linearSpeedMPS"    , _linearSpeedMPS);     //*****
    private_nh.getParam("pubRate"           , _pubRate);
    private_nh.getParam("destinationTolerance", _destinationTolerance);
    private_nh.getParam("obsFrontTopic" , _obsFrontTopic) ;
    private_nh.getParam("obsBackTopic"  , _obsBackTopic);
    private_nh.getParam("obsLeftTopic"  , _obsLeftTopic) ;
    private_nh.getParam("obsRightTopic" , _obsRightTopic);
    private_nh.getParam("waypointTopic" , _waypointTopic);
    private_nh.getParam("navModeTopic"  , _navModeTopic);

    ros::Rate rate(_pubRate);
    ///Timer to get the difference between 2 calls of loaclisation to calculate speed
    ros::Time lasttime=ros::Time::now();
    ros::Time currtime=ros::Time::now();
    ros::Duration diffTime;

    ///Define all the Subscribers here
    //localizer_sub       = node.subscribe("/ndt_pose", 1, &Listener::callback_Pose, &listener_localizer);
    localizer_sub       = node.subscribe("/tf_ndt_pose", 1, &Listener::callback_Pose, &listener_localizer);
    obstacleFront_sub   = node.subscribe(_obsFrontTopic, 1,  &Listener::callback_float, &listener_obstacleFront);
    obstacleBack_sub    = node.subscribe(_obsBackTopic, 1,  &Listener::callback_float, &listener_obstacleBack);
    obstacleLeft_sub    = node.subscribe(_obsLeftTopic, 1,  &Listener::callback_float, &listener_obstacleLeft);
    obstacleRight_sub   = node.subscribe(_obsRightTopic, 1,  &Listener::callback_float, &listener_obstacleRight);
    navMode_sub         = node.subscribe(_navModeTopic, 1,  &Listener::callback_int, &listener_navMode);//subscribes to the nav_mode to check if Auto or Remote
    wpLoad_sub          = node.subscribe(_waypointTopic, 1,  &Listener::callback_wp, &listener_wpMode);//subscribes to the /wp_request_status and if the mode is available then reads _waypoints.txt and
   
    ///Define all the Publishers here
    pub_cmd         = node.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    pub_base_status = node.advertise<wizo_msgs::basedata>("base_status", 1);
    pub_wp          = node.advertise<std_msgs::String>("current_waypoints", 1); //NOT USED AT THE MOMENT
    pub_obstacle_Status = node.advertise<std_msgs::String>("obstacle_status", 1);
    pub_patrol_status = node.advertise<std_msgs::Int16>("patrol_status", 1);

    ///record log to file

    static std::ofstream ofs;
    static std::string filename;
    char buffer[80];
    std::time_t now = std::time(NULL);
    std::tm* pnow = std::localtime(&now);
    std::strftime(buffer, 80, "%Y%m%d_%H%M%S", pnow);
    filename = "logs/wp_" + std::string(buffer) + ".csv";
    ofs.open(filename.c_str(), std::ios::app);

    while(ros::ok())
    {

        currentObstacle.front_obs = listener_obstacleFront.get_Float_MessageValue();
        currentObstacle.back_obs = listener_obstacleBack.get_Float_MessageValue();
        currentNavMode = listener_navMode.get_Int_MessageValue(); //if waypoint loaded.

        printf("Obstacle: %f \n",currentObstacle.front_obs );
        currtime=ros::Time::now();

        ///Variables to reinitialise/reset every cycle - 1) cmd_vel msg 2) required speed
        geometry_msgs::Twist cmdVel_msg;
        cmd_struct reqSpeed;
	   wizo_msgs::basedata base_status_msg;
	   std_msgs::Int16 patrol_status_msg;
        
        cout<<"No. of waypoints in list"<<wp_vector.size()<<endl;
   	   bool loadNextWaypoints = true;
	   while (loadNextWaypoints && wp_vector.size() > 0){
	       //load the first point in the waypoints
	        vector<double> _nextPoint = wp_vector[0];
		   // Calculate the distance to destination
		   float _dist = calcDistance(currentPose.x,currentPose.y, _nextPoint[0], _nextPoint[1]);
		   // Check if reached destination
	        loadNextWaypoints = (_dist<= _destinationTolerance)? true : false;///check if destination reached ? loadNextWaypoints = true : loadNextWaypoints = false
	        // If yes load next point if list available
		   if(loadNextWaypoints){
	              //check if wp_file end reached
			    wp_eof = (wp_vector.size() == 0)? true: false;
			    if (wp_eof)
				{
					cmdVel_msg.linear.x  = 0;  // Dont Do this
				        cmdVel_msg.angular.z = 0;  // Dont Do this
					base_status_msg.base_status = 0;
					patrol_status_msg.data = 0;
					break;
				}
				// Erase the first point to load the next
				wp_vector.erase(wp_vector.begin());
		   }
		   else
	        {
			 ///If current mode is (auto) and waypoint (available), do navigation,  else dont move
			 reqSpeed =((currentNavMode == OP_MODE_AUTO) & (wp_vector.size()>0) )?(navigate(&currentPose,wp_vector[0],&currentObstacle)) : (cmd_struct());
			 base_status_msg.base_status = 1;
		   }
	   }

       
       
     
        printf(BOLDGREEN"CurrX= %f,   CurrY= %f,   CurrYaw= %f,\n"RESET,currentPose.x, currentPose.y,currentPose.yaw);
        printf(BOLDRED"NxtX= %f,   NxtY= %f,   reqYaw = %f \n"RESET,nxtX, nxtY,reqYaw);
        printf("linx= %f,   angZ= %f\n",cmdVel_msg.linear.x, cmdVel_msg.angular.z);
        pub_cmd.publish(cmdVel_msg);
	   pub_base_status.publish(base_status_msg);
	   pub_patrol_status.publish(patrol_status_msg);
          ofs <<"Live_X," << currentPose.x << ","
            <<"Live_Y," << currentPose.y << ","
            <<"LiveYaw,"<< currentPose.yaw << ","
            <<"Recorded_X,"<< nxtX << ","
            <<"Recorded_Y,"<< nxtY << ","
            <<"Recorded_Yaw,"<<  reqYaw << ","
            <<"Applied_Throtle," << cmdVel_msg.linear.x << ","
            <<"Applied_Steering,"<< cmdVel_msg.angular.z << ","
            << std::endl;

        ros::spinOnce();
        ///Rate should be a rosparam
        rate.sleep();
    }

}











