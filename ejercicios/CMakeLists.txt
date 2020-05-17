# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

 find_package(OpenGL REQUIRED)


 if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



 # Compile external dependencies 
add_subdirectory (external)

 # On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

 if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



 include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

 set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

 add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

 # Tutorial 2
add_executable(ejercicio3 
	ejercicio3/main.cpp
	common/shader.cpp
	common/shader.hpp

 	ejercicio3/SimpleFragmentShader.fragmentshader
	ejercicio3/SimpleVertexShader.vertexshader
)
target_link_libraries(ejercicio3
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(ejercicio3 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio3/")
create_target_launcher(ejercicio3 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio3/")
create_default_target_launcher(ejercicio3 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio3/") # tut 1 is not the default or people would complain that tut 2 doesn't work
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

 # Ejercicio04
add_executable(ejercicio04 
	ejercicio04/ejercicio04.cpp
	common/shader.cpp
	common/shader.hpp

 	ejercicio04/SimpleFragmentShader.fragmentshader
	ejercicio04/SimpleVertexShader.vertexshader
)
target_link_libraries(ejercicio04
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(ejercicio04 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio04/")
create_target_launcher(ejercicio04 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio04/")
create_default_target_launcher(ejercicio04 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio04/") # tut 1 is not the default or people would complain that tut 2 doesn't work
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


 # Ejercicio04
add_executable(ejercicio05 
	ejercicio05/ejercicio05.cpp
	common/shader.cpp
	common/shader.hpp

 	ejercicio05/SimpleFragmentShader.fragmentshader
	ejercicio05/SimpleVertexShader.vertexshader
)
target_link_libraries(ejercicio05
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(ejercicio05 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio05/")
create_target_launcher(ejercicio05 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio05/")
create_default_target_launcher(ejercicio05 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio05/") # tut 1 is not the default or people would complain that tut 2 doesn't work
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

 # Ejercicio04
add_executable(control_practico 
	control_practico/control_practico.cpp
	common/shader.cpp
	common/shader.hpp
)
target_link_libraries(control_practico
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(control_practico PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/control_practico/")
create_target_launcher(control_practico WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/control_practico/")
create_default_target_launcher(control_practico WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/control_practico/") # tut 1 is not the default or people would complain that tut 2 doesn't work
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


 if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

 add_custom_command(
   TARGET ejercicio3 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/ejercicio3${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio3/"
)
add_custom_command(
   TARGET ejercicio04 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/ejercicio04${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio04/"
)
add_custom_command(
   TARGET ejercicio05 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/ejercicio05${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/ejercicio05/"
)
add_custom_command(
   TARGET control_practico POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/control_practico${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/control_practico/"
)


 elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

 endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
