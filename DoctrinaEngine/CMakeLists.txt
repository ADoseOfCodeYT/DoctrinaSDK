
#  _____   ____   _____ _______ _____  _____ _   _            ______ _   _  _____ _____ _   _ ______ 
# |  __ \ / __ \ / ____|__   __|  __ \|_   _| \ | |   /\     |  ____| \ | |/ ____|_   _| \ | |  ____|
# | |  | | |  | | |       | |  | |__) | | | |  \| |  /  \    | |__  |  \| | |  __  | | |  \| | |__   
# | |  | | |  | | |       | |  |  _  /  | | | . ` | / /\ \   |  __| | . ` | | |_ | | | | . ` |  __|  
# | |__| | |__| | |____   | |  | | \ \ _| |_| |\  |/ ____ \  | |____| |\  | |__| |_| |_| |\  | |____ 
# |_____/ \____/ \_____|  |_|  |_|  \_\_____|_| \_/_/    \_\ |______|_| \_|\_____|_____|_| \_|______|
                                                                                                    
file(GLOB_RECURSE DOCTRINAENGINE_SOURCE_CPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE DOCTRINAENGINE_SOURCE_H CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h")
                                                                        

add_library(DoctrinaEngine STATIC  ${DOCTRINAENGINE_SOURCE_CPP} ${DOCTRINAENGINE_SOURCE_H} "DoctrinaEngine.h")


target_include_directories(DoctrinaEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DoctrinaEngine PROPERTY CXX_STANDARD 20)
endif()
               