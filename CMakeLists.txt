cmake_minimum_required(VERSION 3.0.2)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(CsvTex) #CsvTex
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
include_directories(${PROJECT_SOURCE_DIR}/include/CsvTex)

find_package(PythonLibs 2.7)
find_package(Eigen3 REQUIRED)
IF (Eigen3_FOUND)
    message(STATUS "[CSV###] Eigen3 found. Version: ${Eigen3_VERSION}")
    include_directories(BEFORE SYSTEM ${Eigen3_INCLUDE_DIRS})
    add_definitions(${Eigen3_CFLAGS})
    link_directories(${Eigen3_LIBRARY_DIRS})
ENDIF()
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})



#target_include_directories(${PROJECT_NAME} PRIVATE ${PYTHON_INCLUDE_DIRS})


add_executable(${PROJECT_NAME}_node src/PublisherCsv.cpp)
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${PYTHON_LIBRARIES}
  Eigen3::Eigen)
