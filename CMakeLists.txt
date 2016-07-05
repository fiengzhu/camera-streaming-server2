cmake_minimum_required (VERSION 2.8.1)

########+ Project settings ########
PROJECT(ov_video_capture)
# SET(LICENSE "MIT")

######## Build and include settings ########
include_directories(
    $ENV{SDKTARGETSYSROOT}/usr/src/kernel/include/uapi
    $ENV{SDKTARGETSYSROOT}/usr/src/kernel/include
    $ENV{SDKTARGETSYSROOT}/usr/include
	/usr/local/include
)

link_directories(
    ${LINK_DIRECTORIES}
	/usr/local/lib
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable(
    caserver2
	        
	${LOCAL_SRCS}
)

target_link_libraries(
    caserver2
	
	PocoFoundation
	PocoNet
	PocoUtil
    opencv_core
    opencv_highgui    
    opencv_imgproc
)
