#include "car_control.h"

CarControl::CarControl()
    : run{ false }
    , speed{ 0 }
    , angle{ 0 }
{
    in_drive_param = nh_.subscribe< drive_msgs::drive_param >(
        TOPIC_DRIVE_PARAM, 1, &CarControl::drive_param_callback, this);

    out_speed = nh_.advertise< std_msgs::Float64 >(TOPIC_FOCBOX_SPEED, 1);
    out_angle = nh_.advertise< std_msgs::Float64 >(TOPIC_FOCBOX_ANGLE, 1);
}

void CarControl::drive_param_callback(
    const drive_msgs::drive_param::ConstPtr& param)
{
    adjustDriveParam(param->velocity, param->angle);
}

void CarControl::adjustDriveParam(double raw_speed, double raw_angle)
{
    speed = raw_speed * MAX_SPEED;
    if (speed < MIN_SPEED)
    {
        speed = 0;
    }
    angle = (raw_angle * MAX_ANGLE + 1) / 2;
    std::cout << "speed: " << speed << " | angle: " << angle << std::endl;
    if (run)
    {
        std_msgs::Float64 msg_speed;
        msg_speed.data = speed;
        out_speed.publish(msg_speed);
        std_msgs::Float64 msg_angle;
        msg_angle.data = angle;
        out_angle.publish(msg_angle);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "car_control");
    CarControl car_control;
    ros::spin();
    return 0;
}