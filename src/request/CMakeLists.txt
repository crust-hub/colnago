FILE(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")
SET(request_source_files ${SOURCE_FILES})
SET(LIBRARY_OUTPUT_PATH lib)
add_library(request ${request_source_files})