project (clock)
include_directories(${CMAKE_JS_INC} node_modules/node-addon-api/)
cmake_minimum_required(VERSION 3.18)

# gcc-11.1 fails with c++20 but succeeds with c++17
# Apple clang version 12.0.0 (clang-1200.0.32.29) succeeds with both c++20 and c++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SOURCE_FILES "*.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
