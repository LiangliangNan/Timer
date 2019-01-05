cmake_minimum_required(VERSION 3.1)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


################################################################################

set (CMAKE_CXX_STANDARD 11)

################################################################################


add_executable(${PROJECT_NAME}
		timer.h
                example.cpp
)
