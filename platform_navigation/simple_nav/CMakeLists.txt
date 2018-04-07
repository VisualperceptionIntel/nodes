cmake_minimum_required(VERSION 2.8.3)
project(simple_nav)


find_package(PCL REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  wizo_msgs
  cmake_modules
#jsoncpp
#segway_msgs
)
find_package(Eigen REQUIRED)



catkin_package(

)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
${Eigen_INCLUDE_DIRS}
)

add_executable(simple_nav_node src/wp_navigation.cpp src/listenerClass.cpp)

target_link_libraries(simple_nav_node ${catkin_LIBRARIES})

add_dependencies(simple_nav_node  wizo_msgs jsoncpp ${catkin_EXPORTED_TARGETS}) #segway_msgs

