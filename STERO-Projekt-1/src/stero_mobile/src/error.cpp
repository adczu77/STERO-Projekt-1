#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/ModelStates.h>
#include <nav_msgs/Odometry.h>
#include <cmath>
#include <fstream>
#include <iostream>

float odometryX = 0.0;
float odometryY = 0.0;

float gazeboX = 0.0;
float gazeboY = 0.0;

float vel = 0;
float vel_ang = 0;

int phase = 0;

void odometryCallback(const nav_msgs::Odometry& msg) {
	odometryX = msg.pose.pose.position.x;
    odometryY = msg.pose.pose.position.y;
};

void velocityCallback(const geometry_msgs::Twist& msg) {
	vel = msg.linear.x;
    vel_ang = msg.angular.z;
};
void gazeboCallback(const gazebo_msgs::ModelStates& msg) {

    gazeboX = msg.pose[1].position.x;
    gazeboY = msg.pose[1].position.y;
};

float distance_odometry = 0.0;
float distance_gazebo = 0.0;

float caly_blad = 0.0;
float bok_blad = 0.0;
float bok1_blad=0.0;
float bok2_blad=0.0;
float bok3_blad=0.0;
float bok4_blad=0.0;

int velocity = 0;
int velocity_angular = 0;

int main(int argc, char* argv[]) {
    ros::init(argc, argv, "error");
    ros::NodeHandle nh;
    ros::Subscriber odometrySubscriber = nh.subscribe("/mobile_base_controller/odom", 1, odometryCallback);
    ros::Subscriber velocitySubscriber = nh.subscribe("/mobile_base_controller/cmd_vel", 1, velocityCallback); 
    ros::Subscriber gazeboSubscriber = nh.subscribe("/gazebo/model_states", 1, gazeboCallback);
    ros::Rate loopRate(10);
    std::ofstream file;
    file.open ("/home/adamc/Projekt1/src/stero_mobile/a1.csv");
    file << "x,y,vel,vel_ang,gazebox,gazeboy,blad,blad_bok,caly_blad\n";
    file.close();
    while(ros::ok()){
        if(vel!=0)
        {
            velocity = 1;
        }
        if(vel_ang!=0)
        {
            velocity_angular = 1;
        }
        distance_odometry = std::sqrt(odometryY*odometryY + odometryX*odometryX);
        distance_gazebo = std::sqrt(gazeboY*gazeboY + gazeboX*gazeboX);
        file.open ("/home/adamc/Projekt1/src/stero_mobile/a1.csv", std::ios::app);
        bok_blad += abs(distance_gazebo - distance_odometry);
        caly_blad += abs(distance_gazebo - distance_odometry);
        std::cout << "X: " << odometryX << "\n";
        std::cout << "Y: " << odometryY << "\n";
        file << odometryX<< ",";
        file << odometryY<< ",";
        file << vel<< ",";
        file << vel_ang<<",";
        file << gazeboX<< ",";
        file << gazeboY<< ",";
        file << abs(distance_gazebo - distance_odometry)<< ",";
        file << bok_blad<<",";
        file << caly_blad<< "\n";
        file.close();
        if (velocity==1 && velocity_angular==1 && vel==0 && vel_ang==0)
        {
            phase++;
            velocity=0;
            velocity_angular=0;
            if(phase==1)
            {
                bok1_blad=bok_blad;
            }
            else if (phase==2)
            {
                bok2_blad=bok_blad;
            }
            else if(phase==3)
            {
                bok3_blad=bok_blad;
            }
            else if (phase==4)
            {
                bok4_blad=bok_blad;
            }
            
            bok_blad=0.0;
        }
        ros::spinOnce();
        loopRate.sleep();
        if (phase==4)
        {
            break;
        }
    }
}
