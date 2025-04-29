#include "camera_subscriber_node.h"

CameraSubscriberNode::CameraSubscriberNode() : Node("camera_subscriber_node")
{
    // Set the value to be changeable depending on the argument passed when running. (Default: RAW)
    this->mode_ = this->declare_parameter<std::string>("mode", "raw");

    if (this->mode_ == "raw")
    {
        this->raw_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "camera/image_raw", 10,
            std::bind(&CameraSubscriberNode::rawImageCallback, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "Subscribed to RAW image topic");
    }

    else if (this->mode_ == "compressed")
    {
        this->compressed_sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
            "camera/image_raw/compressed", 10,
            std::bind(&CameraSubscriberNode::compressedImageCallback, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "Subscribed to COMPRESSED image topic");
    }
}

void CameraSubscriberNode::rawImageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    try
    {
        cv::Mat frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
        cv::imshow("Camera Subscriber Viewer", frame);
        cv::waitKey(1);
    }
    catch (cv_bridge::Exception & e)
    {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
}

void CameraSubscriberNode::compressedImageCallback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    try
    {
        // Decode msg into an OpenCV image.
        cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);

        cv::imshow("Camera Subscriber Viewer", frame);
        cv::waitKey(1);
    }
    catch (cv::Exception & e)
    {
        RCLCPP_ERROR(this->get_logger(), "OpenCV decode exception: %s", e.what());
    }
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraSubscriberNode>());
    rclcpp::shutdown();
    return 0;
}