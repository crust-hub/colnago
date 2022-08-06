FILE(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")
SET(view_source_files ${SOURCE_FILES})
SET(LIBRARY_OUTPUT_PATH lib)
add_library(view ${view_source_files})