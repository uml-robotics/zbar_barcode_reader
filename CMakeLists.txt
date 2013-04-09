cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=c++0x")        ## Enable C++0x 
endif()

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

include($ENV{ROS_ROOT}/core/rosbuild/FindPkgConfig.cmake)
pkg_search_module(zbar REQUIRED zbar)

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})

#common commands for building c++ executables and libraries
rosbuild_add_library(${PROJECT_NAME} src/barcode_reader.cpp)
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES} ${zbar_LIBRARIES})
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
rosbuild_add_executable(barcode_reader_node src/barcode_reader_node.cpp)
target_link_libraries(barcode_reader_node ${PROJECT_NAME})
