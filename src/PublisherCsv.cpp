#include "ros/ros.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <Eigen/Eigen>
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"

using namespace std;


class ReadCsv{

public:

  ReadCsv(){}
  ~ReadCsv(){}


  /**
   * Reads the Columns of the file in a vector of pair containing column name , column values.
   *
   * @param values File name whose values are read.
   * @param values Maximum Column Index till data are read.
   * @return Vector of `pairs <string, vector>`.
   */

  std::vector<std::pair<std::string, std::vector<double>>> readColumns (std::string filename, int MaxIndex){
    /** Reads a CSV file into a vector of <string, vector<double>> pairs where
     each pair represents <column name, column values>*/

    if(MaxIndex <= 0.0) throw std::runtime_error("[CSV] Nr of Columns should be more than Zero !");

    // Create a vector of <string, double vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<double>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    if(!myFile.is_open()) throw std::runtime_error("[CSV] Could not open file");
    std::string line, colname;
    double val;
    int count =0;
    if(myFile.good())
    {
      // Extract the first line in the file
      std::getline(myFile, line);

      // Create a stringstream from line
      std::stringstream ss(line);
      // Extract each column name
      std::cout << "[CSV] Reading " << MaxIndex << " columns." << std::endl;
      while(std::getline(ss, colname, ',') && count<MaxIndex){
        count ++;
        // Initialize and add <colname, double vector> pairs to result
        result.push_back({colname, std::vector<double> {}});
      }

      // Read data, line by line
      while(std::getline(myFile, line)  )
      {
        // Create a stringstream of the current line
        std::stringstream ss(line);
        count=0;
        // Keep track of the current column index
        unsigned int colIdx = 0;

        // Extract each double value till Max colum index
        while(ss >> val && count<MaxIndex ){

          // Add the current integer to the 'colIdx' column's values vector
          result.at(colIdx).second.push_back(val);
          count++;
          // If the next token is a comma, ignore it and move on
          if(ss.peek() == ',') ss.ignore();

          // Increment the column index
          colIdx++;
        }
      }
    }
    myFile.close();
    return result;
  }

};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "PublisherCsv");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::Float64>("timestamp", 1000);
  ros::Publisher joint1_pub = nh.advertise<std_msgs::Float64>("pose_tx", 1000);
  ros::Publisher joint2_pub = nh.advertise<std_msgs::Float64>("pose_ty", 1000);
  ros::Publisher joint3_pub = nh.advertise<std_msgs::Float64>("pose_tz", 1000);
  ros::Publisher joint4_pub = nh.advertise<std_msgs::Float64>("pose_qw", 1000);
  ros::Publisher joint5_pub = nh.advertise<std_msgs::Float64>("pose_qx", 1000);
  ros::Publisher joint6_pub = nh.advertise<std_msgs::Float64>("pose_qy", 1000);
  ros::Rate loop_rate(10);

  ReadCsv Obj;
  std::vector<double> Timestamp;
  std::vector<double> joint_angles_1,joint_angles_2,joint_angles_3,joint_angles_4,joint_angles_5,joint_angles_6;
  std::vector<double> joint_velocity;


  std::vector<std::pair<std::string, std::vector<double>>> output = Obj.readColumns("/home/mujib/Desktop/etc/experiment1.csv", 28);
  std::vector<std::pair<std::string, std::vector<double>>>::iterator it;
  for(it = output.begin(); it != output.end(); it++ )    {

    if(it->first == "Timestamp") { Timestamp=it->second;}
    ///Change the name of the values of the comparison below in order to generate
    ///the results for other outputs also change Max colums to be read.
    else if (it->first == "pose_tx"){joint_angles_1=it->second;}
    else if (it->first == "pose_ty"){joint_angles_2=it->second;}
    else if (it->first == "pose_tz"){joint_angles_3=it->second;}
    else if (it->first == "pose_qw"){joint_angles_4=it->second;}
    else if (it->first == "pose_qx"){joint_angles_5=it->second;}
    else if (it->first == "pose_qy"){joint_angles_6=it->second;}

  }

  while (ros::ok())
  {

    std_msgs::Float64 timeMsg;

    std_msgs::Float64 ang1Msg; std_msgs::Float64 ang2Msg;
    std_msgs::Float64 ang3Msg; std_msgs::Float64 ang4Msg;
    std_msgs::Float64 ang5Msg; std_msgs::Float64 ang6Msg;

    for (unsigned int k=0; k<Timestamp.size() ; k++) {
      timeMsg.data=Timestamp.at(k);

      ang1Msg.data=joint_angles_1.at(k);
      ang2Msg.data=joint_angles_2.at(k);
      ang3Msg.data=joint_angles_3.at(k);
      ang4Msg.data=joint_angles_4.at(k);
      ang5Msg.data=joint_angles_5.at(k);
      ang6Msg.data=joint_angles_6.at(k);

      chatter_pub.publish(timeMsg);
      joint1_pub.publish(ang1Msg);
      joint2_pub.publish(ang2Msg);
      joint3_pub.publish(ang3Msg);
      joint4_pub.publish(ang4Msg);
      joint5_pub.publish(ang5Msg);
      joint6_pub.publish(ang6Msg);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
