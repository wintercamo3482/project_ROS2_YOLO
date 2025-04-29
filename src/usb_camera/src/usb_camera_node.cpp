#include "usb_camera_node.h"

USBCameraNode::USBCameraNode() : Node("camera_publisher_node")
{
    this->mode_ = this->declare_parameter<std::string>("mode", "raw");

    if (this->mode_ == "raw")
    {
        this->publisher_raw_ = this->create_publisher<sensor_msgs::msg::Image>("camera/image_raw", 10);
        RCLCPP_INFO(this->get_logger(), "Publishing in RAW mode");
    }
    else if (this->mode_ == "compressed")
    {
        this->publisher_compressed_ = this->create_publisher<sensor_msgs::msg::CompressedImage>("camera/image_raw/compressed", 10);
        RCLCPP_INFO(this->get_logger(), "Publishing in COMPRESSED mode");
    }
    
    this->cap_.open(0);
    
    if (!cap_.isOpened())
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to open camera");
        rclcpp::shutdown();
        return;
    }
    
    this->timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33),
            std::bind(&USBCameraNode::timerCallback, this)
    );
}

// Function called periodically by a timer.
void USBCameraNode::timerCallback()
{
    cv::Mat frame;
    this->cap_ >> frame;
        
    if (frame.empty())
    {
        RCLCPP_WARN(this->get_logger(), "[WARN] Receive empty frame...");
        return;
    }

    if (this->mode_ == "raw")
    {
        // Convert OpenCV cv::Mat to ROS2 message type (sensor_msgs/msg/Image).
        // bgr8: OpenCV's default BGR 8bit color format
        auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();

        // Converted image is published to the ROS2 topic (camera/image_raw).
        // image_transport is activated and /camera/image_raw/compressed is also created.
        this->publisher_raw_->publish(*msg);
    }
           
    else if (mode_ == "compressed")
    {
            std::vector<uchar> buf;

            // Encode cv::Mat to jpg format and save the result to buf.
            cv::imencode(".jpg", frame, buf);

            // Create a new message of type CompressedImage to be managed by a smart pointer.
            auto compressed_msg = std::make_shared<sensor_msgs::msg::CompressedImage>();

            compressed_msg->format = "jpeg";
            compressed_msg->data = buf;

            publisher_compressed_->publish(*compressed_msg);
    }
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<USBCameraNode>());
    rclcpp::shutdown();

    return 0;
}