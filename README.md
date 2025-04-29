### publish
---
raw

ros2 run usb_camera usb_camera_node --ros-args -p mode:=raw

compressed

ros2 run usb_camera usb_camera_node --ros-args -p mode:=compressed

### subscribe
---
raw

ros2 run camera_subscriber camera_subscriber_node --ros-args -p mode:=raw

compressed

ros2 run camera_subscriber camera_subscriber_node --ros-args -p mode:=compressed