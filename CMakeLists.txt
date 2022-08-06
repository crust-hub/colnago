cmake_minimum_required(VERSION 3.10.0)

project(colnago)

# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -O2")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)

FILE(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} server router request dao utils restbed curl sqlite3)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
