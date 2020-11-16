cmake_minimum_required(VERSION 3.9)
set (CMAKE_CXX_STANDARD 11)

project(typescript_with_addon VERSION 0.0.1)
include_directories(${CMAKE_JS_INC})
string(APPEND CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wendif-labels")

file(GLOB SOURCE_FILES *.cpp *.h *.cc *.c)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )

# need to pay attention to this part
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# Define NAPI_VERSION
add_definitions(-DNAPI_VERSION=3)

