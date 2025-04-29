#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class USBCameraNode : public rclcpp::Node
{
public:
    USBCameraNode();

private:
    void timerCallback();

    std::string mode_;
    rclcpp::TimerBase::SharedPtr timer_;
    cv::VideoCapture cap_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_raw_;
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher_compressed_;
};