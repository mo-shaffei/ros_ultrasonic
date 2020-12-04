//
// Created by shaffei on 12/4/20.
//
#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/String.h>

int main(int argc, char **argv) {

    // initialize this ros node and call it us_publisher
    ros::init(argc, argv, "us_publisher");

    // create a NodeHandle object
    ros::NodeHandle node_handle;
    // create a Range message object
    sensor_msgs::Range sonar_msg;
    // advertise a topic called rangeSonar whose message is of type Range
    ros::Publisher pub_range = node_handle.advertise<sensor_msgs::Range>("rangeSonar", 1000);
    // set the loop frequency to 1Hz, so the readings are updated every 1 second
    ros::Rate loop_rate(1);

    // our radiation type is ultrasonic
    sonar_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
    // call the us frame sonar_frame
    sonar_msg.header.frame_id = "sonar_frame";
    // field of view of the HC-SR04 is 30 degrees(0.523599 rad) from datasheet
    sonar_msg.field_of_view = 0.523599;
    //min and max range are 0 m and 1 m
    sonar_msg.min_range = 0.0;
    sonar_msg.max_range = 1.0;

    float sensor_reading = 0;
    while (ros::ok()) {
        // update the range value with the new reading
        sonar_msg.range = sensor_reading / 100;
        // store the current time in the stamp of the message
        sonar_msg.header.stamp = ros::Time::now();
        // publish the new reading to the rangeSonar topic
        pub_range.publish(sonar_msg);
        ros::spinOnce();
        // sleep 1 second
        loop_rate.sleep();

        // update the reading and reset if it exceeds 50
        sensor_reading += 5;
        if (sensor_reading > 50)
            sensor_reading = 0;
    }
    return 0;
}