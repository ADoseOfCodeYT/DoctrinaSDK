
#  ______ _____ _____ _______ ____  _____  
# |  ____|  __ \_   _|__   __/ __ \|  __ \ 
# | |__  | |  | || |    | | | |  | | |__) |
# |  __| | |  | || |    | | | |  | |  _  / 
# | |____| |__| || |_   | | | |__| | | \ \ 
# |______|_____/_____|  |_|  \____/|_|  \_\
                                                               

add_executable (Editor "Editor.cpp" "Editor.h"  )

target_link_libraries(Editor PRIVATE DoctrinaEngine)

target_include_directories(Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Editor PROPERTY CXX_STANDARD 20)
endif()