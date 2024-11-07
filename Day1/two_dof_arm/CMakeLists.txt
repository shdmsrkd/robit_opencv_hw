cmake_minimum_required(VERSION 3.8)
project(two_dof_arm)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(arm_node src/arm_node.cpp)
target_include_directories(arm_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(arm_node PUBLIC c_std_99 cxx_std_17)
target_link_libraries(arm_node
  ${OpenCV_LIBS}
)
ament_target_dependencies(arm_node
  rclcpp
  OpenCV
)

install(TARGETS arm_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
