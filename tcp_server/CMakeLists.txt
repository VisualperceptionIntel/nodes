cmake_minimum_required(VERSION 2.8.3)
project(tcp_server)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake-modules)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  segway_msgs
)

find_package(MySqlClient REQUIRED)


option(WITH_OPENMP      "Build with OpenMP support for multithreading"                  ON)
# add OpenMP support
if (WITH_OPENMP)
    find_package(OpenMP QUIET)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    if (NOT OPENMP_FOUND)
        set(WITH_OPENMP OFF)
    endif()
endif()


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIRS}
)

add_definitions(
  ${MYSQL_DEFINITIONS}
)


## Declare a cpp executable
add_executable(server_node src/server_node.cpp src/wizo_log.cpp src/wizo_tcp.cpp src/cmd_update_functions.cpp src/control_unit_table_update.cpp src/MySql_conn.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(server_node
  ${catkin_LIBRARIES}
  ${MYSQL_LIBRARIES}
  yaml-cpp
)

add_dependencies(server_node segway_msgs ${catkin_EXPORTED_TARGETS})
