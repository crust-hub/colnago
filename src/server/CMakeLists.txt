FILE(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")
SET(server_source_files ${SOURCE_FILES})
SET(LIBRARY_OUTPUT_PATH lib)
add_library(server ${server_source_files})