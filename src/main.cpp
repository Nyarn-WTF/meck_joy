#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joy.hpp"

using std::placeholders::_1;

class Meck : public rclcpp::Node{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub;

public:
    explicit Meck():Node("teleop_twist_joy_mech"){
        auto callback = [this](const sensor_msgs::msg::Joy::SharedPtr smsg)->void{
            geometry_msgs::msg::Twist pmsg;

            RCLCPP_INFO(this->get_logger(),"");

            pmsg.linear.x  = (double)smsg->axes[3];
            pmsg.linear.y  = (double)smsg->axes[4];
            pmsg.angular.z = (double)smsg->axes[0];

            if(smsg->buttons[13]){
                pmsg.linear.x  = 0.0;
                pmsg.linear.y  = 0.0;
                pmsg.angular.z = 0.0;
            }

            pub->publish(pmsg);
        };
        this->pub = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        this->sub = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, callback);
    }
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);

    auto node = std::make_shared<Meck>();
    RCLCPP_INFO(node->get_logger(),"Meck_Joy");
    
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}