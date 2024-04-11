#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/PoseStamped.h>
#include <cmath>
#include <stdlib.h> 


int main(int argc, char* argv[]) {
	int frequency=1000;
	ros::init(argc, argv, "package");
	ros::NodeHandle nh;
	ros::Publisher publisher = nh.advertise<geometry_msgs::Twist>("/key_vel",1);
	ros::Rate loopRate(frequency);
	geometry_msgs::Twist message;
	float bok;
	std::string kierunek = "+";
	std::string  profil = "s";
	float v = 5.0;
	float w = 0.5;
	float a = 0.01;
	message.linear.x = 0;
	message.linear.y = 0;
	message.linear.z = 0;
	message.angular.x = 0;
	message.angular.y = 0;
	message.angular.z = 0;
	float distance = 0.0;
	float angle = 0.0;
	int phase = 0;
	int timer=0;
	nh.getParam("/bok", bok);
	nh.getParam("/kierunek", kierunek);
	nh.getParam("/profil", profil);
	nh.getParam("/v", v);
	nh.getParam("/w", w);
	nh.getParam("/a", a);
	while (ros::ok())
	{
		timer=0;
		if (profil == "s")
		{
			while (distance < bok)
			{
				message.linear.x = v;
				distance += 1.0/frequency*v;
				publisher.publish(message);
				ros::spinOnce();
				loopRate.sleep();
			}
		}
		else if (profil == "t")
		{
			while (distance<bok/3)
			{
				distance += 1.0/frequency * message.linear.x;
				message.linear.x += 1.0/frequency*a;
				publisher.publish(message);
				std::cout << "distance:" << distance << "   velocity:"<<message.linear.x<<"\n";
				ros::spinOnce();
				loopRate.sleep();
			}
			while (distance<=bok*2/3 && distance >=bok/3)
			{
				distance += 1.0/frequency*message.linear.x;
				publisher.publish(message);
				std::cout << "distance:" << distance << "   velocity:"<<message.linear.x<<"\n";;
				ros::spinOnce();
				loopRate.sleep();
			}
			while (distance>bok*2/3 && distance<bok)
			{
				distance += 1.0/frequency * message.linear.x;
				message.linear.x -= 1.0/frequency*a;
				publisher.publish(message);
				std::cout << "distance:" << distance << "   velocity:"<<message.linear.x<<"\n";;
				ros::spinOnce();
				loopRate.sleep();
			}
			
		}
		message.linear.x = 0;
		publisher.publish(message);
		ros::spinOnce();
		loopRate.sleep();
		while(timer<1500)
		{
			message.linear.x = 0;
			angle = 0.0;
			publisher.publish(message);
			ros::spinOnce();
			loopRate.sleep();
			timer++;
		}
		
		while (angle < M_PI/2)
		{
			timer=0;
			if (kierunek=="+")
			{
				message.angular.z = -w;
			}
			else if (kierunek=="-")
			{
				message.angular.z = w;
			}
			angle += 1.0/frequency*w;
			publisher.publish(message);
        	ros::spinOnce();
        	loopRate.sleep();
		}
		message.angular.z = 0.0;
        distance = 0.0;		
		while(timer<1000)
		{
			message.linear.x = 0.0;
			distance = 0.0;
			publisher.publish(message);
			ros::spinOnce();
			loopRate.sleep();
			timer++;
		}	
		publisher.publish(message);
        ros::spinOnce();
        loopRate.sleep();
		phase++;
		if (phase==4)
		{
			break;
		}
	}
	return 0;
}
