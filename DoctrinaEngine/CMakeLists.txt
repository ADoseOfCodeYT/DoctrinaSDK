#  _____   ____   _____ _______ _____  _____ _   _            ______ _   _  _____ _____ _   _ ______ 
# |  __ \ / __ \ / ____|__   __|  __ \|_   _| \ | |   /\     |  ____| \ | |/ ____|_   _| \ | |  ____|
# | |  | | |  | | |       | |  | |__) | | | |  \| |  /  \    | |__  |  \| | |  __  | | |  \| | |__   
# | |  | | |  | | |       | |  |  _  /  | | | . ` | / /\ \   |  __| | . ` | | |_ | | | | . ` |  __|  
# | |__| | |__| | |____   | |  | | \ \ _| |_| |\  |/ ____ \  | |____| |\  | |__| |_| |_| |\  | |____ 
# |_____/ \____/ \_____|  |_|  |_|  \_\_____|_| \_/_/    \_\ |______|_| \_|\_____|_____|_| \_|______|                                                                                                   
                         
cmake_minimum_required (VERSION 4.0.2)

project ("DoctrinaEngine" CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "Source/*.cpp")

add_library(${PROJECT_NAME} STATIC "DoctrinaEngine.h"  ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")

