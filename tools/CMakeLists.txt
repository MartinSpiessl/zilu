cmake_minimum_required (VERSION 2.8)

project(smt2solver)
set(PRECISION 3)
set(filename "smt2solver")


if(UNIX)
	set(CMAKE_C_COMPILER             "clang")
	set(CMAKE_C_FLAGS                "-Wall -std=c99")
	set(CMAKE_C_FLAGS_DEBUG          "-g")
	set(CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_CXX_COMPILER             "clang++")
	#set(CMAKE_CXX_FLAGS                "-Wall -g")
	set(CMAKE_CXX_FLAGS                "-Wall -g -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG          "-g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_AR      "/usr/bin/llvm-ar")
	set(CMAKE_LINKER  "/usr/bin/llvm-ld")
	set(CMAKE_NM      "/usr/bin/llvm-nm")
	set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
	set(CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

# config for Z3 solver...
	set(Z3_ROOT "/usr" CACHE PATH "Root of Z3 distribution.")
	#find_library(Z3_LIBRARY NAMES z3 PATHS ${Z3_ROOT}/lib NO_DEFAULT_PATH)
	find_library(Z3_LIBRARY NAMES z3 PATHS ${Z3_ROOT}/lib /usr/local/lib)
endif(UNIX)

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



add_executable(${filename} ./src/${filename}.cpp)
target_link_libraries(${filename} ${Z3_LIBRARY})
