
#  _____   ____   _____ _______ _____  _____ _   _            ______ _   _  _____ _____ _   _ ______ 
# |  __ \ / __ \ / ____|__   __|  __ \|_   _| \ | |   /\     |  ____| \ | |/ ____|_   _| \ | |  ____|
# | |  | | |  | | |       | |  | |__) | | | |  \| |  /  \    | |__  |  \| | |  __  | | |  \| | |__   
# | |  | | |  | | |       | |  |  _  /  | | | . ` | / /\ \   |  __| | . ` | | |_ | | | | . ` |  __|  
# | |__| | |__| | |____   | |  | | \ \ _| |_| |\  |/ ____ \  | |____| |\  | |__| |_| |_| |\  | |____ 
# |_____/ \____/ \_____|  |_|  |_|  \_\_____|_| \_/_/    \_\ |______|_| \_|\_____|_____|_| \_|______|           

cmake_minimum_required (VERSION 3.8)
project ("DoctrinaEngine") 

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
               