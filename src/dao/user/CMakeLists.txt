SET(user_source_files user.cpp)
SET(LIBRARY_OUTPUT_PATH lib)
add_library(user ${user_source_files})
target_link_libraries(user util)