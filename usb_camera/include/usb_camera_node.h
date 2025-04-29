#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.h>

class USBCameraNode : public rclcpp::Node
{
public:
    USBCameraNode();
    void setPublisher(image_transport::Publisher pub);

private:
    void timerCallback();

    cv::VideoCapture cap_;
    rclcpp::TimerBase::SharedPtr timer_;
    image_transport::Publisher publisher_;
};