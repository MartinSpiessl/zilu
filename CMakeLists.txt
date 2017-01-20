cmake_minimum_required (VERSION 2.8)
set(Nv 2)
project(InvariantInferenceFramework)
set(PRECISION 3)

add_definitions (-D__DS_ENABLED)
#add_definitions (-D__QUESTION_TRACE_CHECK_ENABLED)
#add_definitions (-D__TRAINSET_SIZE_RESTRICTED)

add_definitions (-D__PRT)
#add_definitions (-D__PRT_DEBUG)
#add_definitions (-D__PRT_TRACE)
add_definitions (-D__PRT_TESTCASE)
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


if(UNIX)
	set(CMAKE_C_COMPILER             "clang")
	set(CMAKE_C_FLAGS                "-Wall -std=c99")
	set(CMAKE_C_FLAGS_DEBUG          "-g")
	set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_CXX_COMPILER             "clang++")
	set(CMAKE_CXX_FLAGS                "-Wall -g")
	#set(CMAKE_CXX_FLAGS                "-Wall -g -lgsl -lgslcblas")
	#set(CMAKE_CXX_FLAGS                "-Wall -g -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG          "-g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_AR      "/usr/bin/llvm-ar")
	set(CMAKE_LINKER  "/usr/bin/llvm-ld")
	set(CMAKE_NM      "/usr/bin/llvm-nm")
	set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
	set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
endif(UNIX)

# config for Z3 solver...
if(UNIX)
	set(Z3_ROOT "/usr" CACHE PATH "Root of Z3 distribution.")
	#find_library(Z3_LIBRARY NAMES z3 PATHS ${Z3_ROOT}/lib NO_DEFAULT_PATH)
	find_library(Z3_LIBRARY NAMES z3 PATHS ${Z3_ROOT}/lib /usr/local/lib)
endif(UNIX)
if(WIN32)
	set(Z3_ROOT "C:/Program Files/z3" CACHE PATH "Root of Z3 distribution.")
	find_library(Z3_LIBRARY NAMES libz3.lib PATHS ${Z3_ROOT}/bin NO_DEFAULT_PATH)
endif(WIN32)

#find_path(Z3_INCLUDE_DIR NAMES z3.h z3++.h PATHS ${Z3_ROOT}/include NO_DEFAULT_PATH)
find_path(Z3_INCLUDE_DIR NAMES z3.h z3++.h PATHS ${Z3_ROOT}/include /usr/local/include)
mark_as_advanced(Z3_EXECUTABLE Z3_INCLUDE_DIR Z3_LIBRARY)
find_program (Z3_EXECUTABLE
	NAMES z3 PATHS ${Z3_ROOT} PATH_SUFFIXES bin 
	#DOC "z3 command line executable" NO_DEFAULT_PATH)
	DOC "z3 command line executable" /usr/local/bin)
mark_as_advanced(Z3_EXECUTABLE)

if (Z3_EXECUTABLE)
	execute_process (COMMAND ${Z3_EXECUTABLE} -version
		OUTPUT_VARIABLE z3_version
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	if (z3_version MATCHES "^Z3 version [0-9]")
		string (REPLACE "Z3 version " "" Z3_VERSION_STRING ${z3_version})
	endif()
endif()
mark_as_advanced(Z3_VERSION_STRING)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(Z3
	REQUIRED_VARS Z3_LIBRARY Z3_INCLUDE_DIR Z3_EXECUTABLE
	VERSION_VAR Z3_VERSION_STRING)
include_directories(${Z3_INCLUDE_DIR})
#endif(UNIX)

IF(UNIX) 
	SET(GSL_CONFIG_PREFER_PATH 
		"$ENV{GSL_DIR}/bin"
		"$ENV{GSL_DIR}"
		"$ENV{GSL_HOME}/bin" 
		"$ENV{GSL_HOME}" 
		CACHE STRING "preferred path to GSL (gsl-config)")
	FIND_PROGRAM(GSL_CONFIG gsl-config
		${GSL_CONFIG_PREFER_PATH}
		/usr/bin/
		)
	# MESSAGE("DBG GSL_CONFIG ${GSL_CONFIG}")

	IF (GSL_CONFIG) 
		# set CXXFLAGS to be fed into CXX_FLAGS by the user:
		SET(GSL_CXX_FLAGS "`${GSL_CONFIG} --cflags`")

		# set INCLUDE_DIRS to prefix+include
		EXEC_PROGRAM(${GSL_CONFIG}
			ARGS --prefix
			OUTPUT_VARIABLE GSL_PREFIX)
		SET(GSL_INCLUDE_DIR ${GSL_PREFIX}/include CACHE STRING INTERNAL)

		# set link libraries and link flags
		#SET(GSL_LIBRARIES "`${GSL_CONFIG} --libs`")
		EXEC_PROGRAM(${GSL_CONFIG}
			ARGS --libs
			OUTPUT_VARIABLE GSL_LIBRARIES )

		# extract link dirs for rpath  
		EXEC_PROGRAM(${GSL_CONFIG}
			ARGS --libs
			OUTPUT_VARIABLE GSL_CONFIG_LIBS )

		# extract version
		EXEC_PROGRAM(${GSL_CONFIG}
			ARGS --version
			OUTPUT_VARIABLE GSL_FULL_VERSION )

		# split version as major/minor
		STRING(REGEX MATCH "(.)\\..*" GSL_VERSION_MAJOR_ "${GSL_FULL_VERSION}")
		SET(GSL_VERSION_MAJOR ${CMAKE_MATCH_1})
		STRING(REGEX MATCH ".\\.(.*)" GSL_VERSION_MINOR_ "${GSL_FULL_VERSION}")
		SET(GSL_VERSION_MINOR ${CMAKE_MATCH_1})

		# split off the link dirs (for rpath)
		# use regular expression to match wildcard equivalent "-L*<endchar>"
		# with <endchar> is a space or a semicolon
		STRING(REGEX MATCHALL "[-][L]([^ ;])+" 
			GSL_LINK_DIRECTORIES_WITH_PREFIX 
			"${GSL_CONFIG_LIBS}" )
		#      MESSAGE("DBG  GSL_LINK_DIRECTORIES_WITH_PREFIX=${GSL_LINK_DIRECTORIES_WITH_PREFIX}")

		# remove prefix -L because we need the pure directory for LINK_DIRECTORIES

		IF (GSL_LINK_DIRECTORIES_WITH_PREFIX)
			STRING(REGEX REPLACE "[-][L]" "" GSL_LINK_DIRECTORIES ${GSL_LINK_DIRECTORIES_WITH_PREFIX} )
		ENDIF (GSL_LINK_DIRECTORIES_WITH_PREFIX)
		SET(GSL_EXE_LINKER_FLAGS "-Wl,-rpath,${GSL_LINK_DIRECTORIES}" CACHE STRING INTERNAL)
		#      MESSAGE("DBG  GSL_LINK_DIRECTORIES=${GSL_LINK_DIRECTORIES}")
		#      MESSAGE("DBG  GSL_EXE_LINKER_FLAGS=${GSL_EXE_LINKER_FLAGS}")

		#      ADD_DEFINITIONS("-DHAVE_GSL")
		#      SET(GSL_DEFINITIONS "-DHAVE_GSL")
		MARK_AS_ADVANCED(
			GSL_CXX_FLAGS
			GSL_INCLUDE_DIR
			GSL_LIBRARIES
			GSL_LINK_DIRECTORIES
			GSL_DEFINITIONS
			)
		MESSAGE(STATUS "Using GSL from ${GSL_PREFIX}")

	ELSE(GSL_CONFIG)
		MESSAGE("FindGSL.cmake: gsl-config not found. Please set it manually. GSL_CONFIG=${GSL_CONFIG}")
	ENDIF(GSL_CONFIG)

ENDIF(UNIX)


file(GLOB HEADER "include/*.h")
source_group("Header Files" FILES ${HEADERS}) 

file(GLOB TEST "test/*.cpp")
source_group("Test" FILES ${TEST}) 

# add include dir and source dir into project
configure_file(config.h.in ../include/config.h)
include_directories (include)
include_directories (frontend)
include_directories (learning)
include_directories (learning/mlAlgo)
include_directories (learning/learners)
include_directories (checking)
#include_directories(.)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(frontend DIR_SRCS)
AUX_SOURCE_DIRECTORY(learning DIR_SRCS)
AUX_SOURCE_DIRECTORY(learning/mlAlgo DIR_SRCS)
AUX_SOURCE_DIRECTORY(learning/learners DIR_SRCS)
AUX_SOURCE_DIRECTORY(checking DIR_SRCS)
#AUX_SOURCE_DIRECTORY(test DIR_TEST)

add_executable(zilu_conj1 /home/lijiaying/Research/GitHub/zilu/test/zilu_conj1.cpp ${DIR_SRCS} ${HEADER})
target_link_libraries(zilu_conj1 ${Z3_LIBRARY})
target_link_libraries(zilu_conj1 ${GSL_LIBRARIES})
