cmake_minimum_required(VERSION 3.6)
project(tgapi)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules )

include(FindJansson)
include(FindCURL)

# Include header files
include_directories(src)

# Find source files
file(GLOB SOURCES src/*.c)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

find_package ( CURL )
find_package ( JANSSON )

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries (tgapi ${CURL_LIBRARIES})
target_link_libraries (tgapi ${JANSSON_LIBRARIES})


message(STATUS "********************************************")
message(STATUS "********** ${PROJECT_NAME} build options : **********")
message(STATUS "CURL support:    ${CURL_FOUND}")
message(STATUS "JANSSON support: ${JANSSON_FOUND}")
message(STATUS "********************************************")
