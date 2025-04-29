#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class CameraSubscriberNode : public rclcpp::Node
{
public:
    CameraSubscriberNode();

private:
    void rawImageCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void compressedImageCallback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);

    std::string mode_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr raw_sub_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr compressed_sub_;
};