SET(router_source_files index.cpp index.h)
SET(LIBRARY_OUTPUT_PATH lib)
add_library(router ${router_source_files})
target_link_libraries(router)