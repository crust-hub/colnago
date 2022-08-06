FILE(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")
SET(utils_source_files ${SOURCE_FILES})
SET(LIBRARY_OUTPUT_PATH lib)
add_library(utils ${utils_source_files})