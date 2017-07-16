cmake_minimum_required (VERSION 2.8)
set(Nv 2)
add_definitions(-D__SELECTIVE_SAMPLING_ENABLED)
project(zilu CXX)
set(PRECISION 3)

###############################################################################################################################
add_definitions (-D__DS_ENABLED)
#add_definitions (-D__QUESTION_TRACE_CHECK_ENABLED)
#add_definitions (-D__TRAINSET_SIZE_RESTRICTED)

add_definitions (-D__PRT)
#add_definitions (-D__PRT_DEBUG)
#add_definitions (-D__PRT_TRACE)
#add_definitions (-D__PRT_TESTCASE)
#add_definitions (-D__PRT_GSLSOLVE)
#add_definitions (-D__PRT_Z3SOLVE)
#add_definitions (-D__PRT_POLYNOMIAL)
#add_definitions (-D__PRT_SVM_I)
#add_definitions (-D__PRT_POLYSVM)
#add_definitions (-D__PRT_INFER)
#add_definitions (-D__PRT_QUERY)
add_definitions (-D__PRT_STATISTICS)
add_definitions (-DSCRIPT)

#option(PRINT_ALL "Print All Message" ON)
option(PRINT_ALL "Print All Message" OFF)
if(PRINT_ALL)
	add_definitions (-D__PRT)
	add_definitions (-D__PRT_TESTCASE)
	add_definitions (-D__PRT_GSLSOLVE)
	add_definitions (-D__PRT_Z3SOLVE)
	add_definitions (-D__PRT_TRACE)
	add_definitions (-D__PRT_POLYNOMIAL)
	add_definitions (-D__PRT_SVM_I)
	add_definitions (-D__PRT_POLYSVM)
	add_definitions (-D__PRT_INFER)
	add_definitions (-D__PRT_QUERY)
endif(PRINT_ALL)
###############################################################################################################################


###############################################################################################################################
#set(CMAKE_C_COMPILER             "clang")
#set(CMAKE_C_FLAGS                "-Wall -std=c99")
#set(CMAKE_C_FLAGS_DEBUG          "-g")
#set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#set(CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_CXX_COMPILER             "clang++")
set(CMAKE_CXX_FLAGS                "-Wall -g")
#set(CMAKE_CXX_FLAGS                "-Wall -g -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
###############################################################################################################################
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
find_package(Z3 REQUIRED)
find_package(GSL REQUIRED)

add_subdirectory(parser)

configure_file(config.h.in ../include/config.h)
# using for windows
#file(GLOB HEADER "include/*.h")
#source_group("Header Files" FILES ${HEADERS}) 
#file(GLOB TEST "test/*.cpp")
#source_group("Test" FILES ${TEST}) 

# add include dir and source dir into project
include_directories (include)
aux_source_directory(src DIR_SRCS)
add_executable(01 /home/lijiaying/Research/GitHub/zilu/test/01.cpp ${DIR_SRCS})
target_link_libraries(01 ${Z3_LIBRARY})
target_link_libraries(01 ${GSL_LIBRARIES})
