
#  _____   ____   _____ _______ _____  _____ _   _            ______ _   _  _____ _____ _   _ ______ 
# |  __ \ / __ \ / ____|__   __|  __ \|_   _| \ | |   /\     |  ____| \ | |/ ____|_   _| \ | |  ____|
# | |  | | |  | | |       | |  | |__) | | | |  \| |  /  \    | |__  |  \| | |  __  | | |  \| | |__   
# | |  | | |  | | |       | |  |  _  /  | | | . ` | / /\ \   |  __| | . ` | | |_ | | | | . ` |  __|  
# | |__| | |__| | |____   | |  | | \ \ _| |_| |\  |/ ____ \  | |____| |\  | |__| |_| |_| |\  | |____ 
# |_____/ \____/ \_____|  |_|  |_|  \_\_____|_| \_/_/    \_\ |______|_| \_|\_____|_____|_| \_|______|
                                                                                                    
                                                                                
add_library(DoctrinaEngine STATIC  "../DoctrinaEngine.h" "Application/Private/deApplication.cpp" "Application/Public/deApplication.h" "Core/Public/deVersion.h")

target_include_directories(DoctrinaEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DoctrinaEngine PROPERTY CXX_STANDARD 20)
endif()
               