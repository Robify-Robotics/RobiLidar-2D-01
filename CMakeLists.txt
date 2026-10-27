cmake_minimum_required(VERSION 2.8.3)

#Make the generated VS project file structure match the local directory structure
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)
 
function(my_add_executable)
    foreach(_source IN ITEMS ${ARGN})
        assign_source_group(${_source})
    endforeach()
    add_executable(${ARGV})
endfunction(my_add_executable)

#Project name
project(bluesea)
#C++98  C99
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
IF (UNIX)
# Add gdb support
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

ELSEIF (WIN32)
#Multithreading
set(CMAKE_CXX_FLAGS_DEBUG "/MT")
#Force file output
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG       "/FORCE")
ENDIF ()
#Specify generation in debug mode
set(CMAKE_CONFIGURATION_TYPES Debug)
#All header file directories
include_directories ("${PROJECT_SOURCE_DIR}/sdk")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/service")
#Load the file set of the specified format from the specified directory
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/sdk/*.cpp  ${PROJECT_SOURCE_DIR}/sdk/*.c  ${PROJECT_SOURCE_DIR}/example/main.cpp)
file(GLOB_RECURSE HEADER_FILES  ${PROJECT_SOURCE_DIR}/sdk/*.h  ${PROJECT_SOURCE_DIR}/sdk/*.hpp)

IF (UNIX)
  #Load the file set of the specified format to remove from the specified directory
  #file(GLOB_RECURSE REMOVE_SOURCES  "${PROJECT_SOURCE_DIR}/sdk/service/LidarCheckService_win32.cpp")
  #file(GLOB_RECURSE REMOVE_HEADERS  "${PROJECT_SOURCE_DIR}/sdk/win32/*")
  #Get the final file set required for compilation
  #list(REMOVE_ITEM SOURCE_FILES ${REMOVE_SOURCES})
  #list(REMOVE_ITEM HEADER_FILES ${REMOVE_HEADERS})
  
  source_group("Include" FILES ${HEADER_FILES})
  #Compile to generate the demo executable
  my_add_executable(demo ${SOURCE_FILES} ${HEADER_FILES})
  #Add the pthread link library
  target_link_libraries(demo m pthread )
ELSEIF (WIN32)
  #Same as above
  file(GLOB_RECURSE REMOVE_SOURCES  "${PROJECT_SOURCE_DIR}/sdk/uart.c")
  #file(GLOB_RECURSE REMOVE_HEADERS  "${PROJECT_SOURCE_DIR}/sdk/linux/*")
  
  list(REMOVE_ITEM SOURCE_FILES ${REMOVE_SOURCES})
  #list(REMOVE_ITEM HEADER_FILES ${REMOVE_HEADERS})
  
  source_group("Include" FILES ${HEADER_FILES})
  my_add_executable(demo ${SOURCE_FILES} ${HEADER_FILES})
ENDIF ()

#make install   Place the executable in the specified location
#install(TARGETS demo RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/tools/)