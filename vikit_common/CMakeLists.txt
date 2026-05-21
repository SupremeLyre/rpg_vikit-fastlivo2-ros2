cmake_minimum_required(VERSION 3.8)
project(vikit_common)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall
  -D_LINUX
  -D_REENTRANT
  -Wno-unused-variable
  -Wno-unused-but-set-variable
  -Wno-unknown-pragmas
)

message(STATUS "Current CPU architecture: ${CMAKE_SYSTEM_PROCESSOR}")
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  add_compile_options(-mmmx -msse -msse2 -msse3 -mssse3)
else()
  add_compile_options(-march=armv8-a)
endif()

find_package(ament_cmake REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Sophus REQUIRED)

set(SOURCEFILES
  src/atan_camera.cpp
  src/omni_camera.cpp
  src/math_utils.cpp
  src/vision.cpp
  src/performance_monitor.cpp
  src/robust_cost.cpp
  src/user_input_thread.cpp
  src/pinhole_camera.cpp
  src/equidistant_camera.cpp
  src/polynomial_camera.cpp
  src/homography.cpp
  src/img_align.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCEFILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${EIGEN3_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Sophus_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBRARIES}
  ${Sophus_LIBRARIES}
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(OpenCV Eigen3 Sophus)
ament_package()
