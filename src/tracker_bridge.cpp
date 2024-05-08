#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <px4_msgs/msg/vehicle_odometry.hpp>

using namespace std;
using px4_msgs::msg::VehicleOdometry;
using geometry_msgs::msg::PoseStamped;
using std::placeholders::_1;

/*
  This node receives geometry_msgs::msg::PoseStamped, then converts it to
  px4_msgs::msg::VehicleOdometry and re-publishes on /fmu/in/vehicle_mocap_odometry.
*/ 
class TrackerBridge : public rclcpp::Node
  {
  public:
    TrackerBridge()
    : Node("minimal_subscriber")
    { 

      rclcpp::SensorDataQoS qos;
      sub = this->create_subscription<PoseStamped>(
      "/vrpn_mocap/drone/pose", qos, std::bind(&TrackerBridge::repost_callback, this, _1));

      pub = this->create_publisher<VehicleOdometry>("/fmu/in/vehicle_visual_odometry", 10);


    }

  private:
    void repost_callback(const PoseStamped & in_msg) const
    { 
      // produce position 
      float x = (float)in_msg.pose.position.x;
      float y = (float)in_msg.pose.position.y;
      float z = (float)in_msg.pose.position.z;

      array<float, 3> position = {x,y,z};

      // produce rotation 
      float x_q = (float)in_msg.pose.orientation.x;
      float y_q = (float)in_msg.pose.orientation.y;
      float z_q = (float)in_msg.pose.orientation.z;
      float w_q = (float)in_msg.pose.orientation.w;

      array<float, 4> q = {x_q,y_q,z_q,w_q};
      
      RCLCPP_INFO(this->get_logger(), "stamp: '%i'", in_msg.header.stamp.sec);

      VehicleOdometry out_msg;
      out_msg.timestamp = (uint64_t)in_msg.header.stamp.sec;
      out_msg.timestamp_sample = 0;
      out_msg.pose_frame = 2;
      out_msg.position = position;
      out_msg.q = q;

      // out_msg.position_variance ={0.0,0.0,0.0};
      // out_msg.angular_velocity ={0.0,0.0,0.0};
      // out_msg.velocity_frame = 2;
      
      // out_msg.position_variance ={0.0,0.0,0.0};
      // out_msg.orientation_variance ={0.0,0.0,0.0};
      // out_msg.velocity_variance ={0.0,0.0,0.0};

      // out_msg.reset_counter = 6;
      // out_msg.quality = 0;


      pub->publish(out_msg);
    }
  rclcpp::Publisher<VehicleOdometry>::SharedPtr pub;
  rclcpp::Subscription<PoseStamped>::SharedPtr sub;
};

        // // define the output message
        // VehicleOdometry out_msg{}

        // out_msg.timestamp_sample = this->get_clock()->now().nanoseconds() / 1000;
        // out_msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        // out_msg.position = {in_msg->data.position.x, in_msg->data.position.y, in_msg->data.position.z};
        // out_msg.q = {in_msg->data.orientation.x, in_msg->data.orientation.y, in_msg->data.orientation.z, in_msg->data.orientation.w}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TrackerBridge>());
  rclcpp::shutdown();
  return 0;
}
