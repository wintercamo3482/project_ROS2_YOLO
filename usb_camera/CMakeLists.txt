cmake_minimum_required(VERSION 3.5)
project(usb_camera)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(usb_camera_node src/usb_camera_node.cpp)

ament_target_dependencies(usb_camera_node
  rclcpp
  sensor_msgs
  image_transport
  cv_bridge
)

target_link_libraries(usb_camera_node
  ${OpenCV_LIBS}
)

install(TARGETS
  usb_camera_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()