cmake_minimum_required(VERSION 3.10.0)
project(colnago)

add_definitions(-w)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++2a -Wall -O2")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

FILE(GLOB_RECURSE SOURCE_FILES "src/*/*.c*")
ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} restbed curl sqlite3 odb-mysql odb)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
