
#  ______ _____ _____ _______ ____  _____  
# |  ____|  __ \_   _|__   __/ __ \|  __ \ 
# | |__  | |  | || |    | | | |  | | |__) |
# |  __| | |  | || |    | | | |  | |  _  / 
# | |____| |__| || |_   | | | |__| | | \ \ 
# |______|_____/_____|  |_|  \____/|_|  \_\

cmake_minimum_required (VERSION 3.8)
project ("Editor")

add_executable (${PROJECT_NAME}
"Source/main.cpp" "Source/main.h"  
"Source/framework.h"
"Source/targetver.h"
"Source/Resource/resource.h"
"Source/Resource/Editor.rc"
)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set_target_properties(${PROJECT_NAME}  PROPERTIES
WIN32_EXECUTABLE YES
)

target_link_libraries(${PROJECT_NAME}  PRIVATE DoctrinaEngine)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 20)
endif()