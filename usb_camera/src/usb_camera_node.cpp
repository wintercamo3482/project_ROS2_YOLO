#include "usb_camera_node.h"

// Constructor: Registers a ROS node with the name "usb_camera_node".
USBCameraNode::USBCameraNode() : Node("usb_camera_node")
{
    this->cap_.open("/dev/video0");

    if (!this->cap_.isOpened())
    {
        RCLCPP_ERROR(this->get_logger(), "[ERROR] Failed to open webcam!");
        rclcpp::shutdown();
        return;
    }

    // Timer that calls the timerCallback() function every 30 fps.
    this->timer_ = this->create_wall_timer(
        std::chrono::milliseconds(33),
        std::bind(&USBCameraNode::timerCallback, this)
    );
}

void USBCameraNode::setPublisher(image_transport::Publisher pub)
{
    this->publisher_ = pub;
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

    // Convert OpenCV cv::Mat to ROS2 message type (sensor_msgs/msg/Image).
    // bgr8: OpenCV's default BGR 8bit color format
    auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();

    msg->header.stamp = this->now();
    msg->header.frame_id = "camera_frame";


    // Converted image is published to the ROS2 topic (camera/image_raw).
    // image_transport is activated and /camera/image_raw/compressed is also created.
    this->publisher_.publish(*msg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<USBCameraNode>();

    // By creating an image_transport object based on node, it is possible to support various transmission methods such as compressed.
    image_transport::ImageTransport it(node);

    // Create a topic "camera/image_raw" and create a publisher.
    auto pub = it.advertise("camera/image_raw", 10);

    // Pass the publisher created above to the USBCameraNode class.
    node->setPublisher(pub);

    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}