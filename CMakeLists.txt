
#  __  __           _____ _______ ______ _____  
# |  \/  |   /\    / ____|__   __|  ____|  __ \ 
# | \  / |  /  \  | (___    | |  | |__  | |__) |
# | |\/| | / /\ \  \___ \   | |  |  __| |  _  / 
# | |  | |/ ____ \ ____) |  | |  | |____| | \ \ 
# |_|  |_/_/    \_\_____/   |_|  |______|_|  \_\
                                                                                           
cmake_minimum_required (VERSION 3.8)

project ("DoctrinaSDK")

#add_compile_definitions("NDEBUG=$<CONFIG:Release>")
#add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory ("DoctrinaEngine")
add_subdirectory ("Editor")

#DELETE THE OUT FOLDER AFTER CHANGING THIS VARIABLE
option(PRODUCTION_BUILD "Remove debug signatures" off)

if(PRODUCTION_BUILD)
	if(MSVC)

	# Remove debug signatures

		target_compile_definitions(DoctrinaEngine PUBLIC _CRT_SECURE_NO_WARNINGS)
		target_compile_definitions(Editor PUBLIC _CRT_SECURE_NO_WARNINGS)

	endif()
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()