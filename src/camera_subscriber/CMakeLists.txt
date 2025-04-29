cmake_minimum_required(VERSION 3.5)
project(camera_subscriber)

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

add_executable(camera_subscriber_node src/camera_subscriber_node.cpp)

ament_target_dependencies(camera_subscriber_node
  rclcpp
  sensor_msgs
  cv_bridge
  image_transport
)

target_link_libraries(camera_subscriber_node ${OpenCV_LIBS})

install(TARGETS
  camera_subscriber_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()