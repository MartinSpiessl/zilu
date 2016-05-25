#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
bold="\e[1m"
normal="\e[0m"

if [ $# -lt 1 ]
then
	echo "./build_projectsh needs more parameters"
	echo "./build_project.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_cfg="cfg/"
dir_test="test/"
dir_temp="tmp/"
dir_tool="tools/bin/"


prefix=$1
mkdir -p tmp
file_cfg=$prefix".cfg"
file_cpp=$prefix".cpp"
file_var=$prefix".var"
file_inv=$prefix".inv"
path_cfg=$dir_cfg""$file_cfg
path_cpp=$dir_test""$file_cpp
path_var=$dir_temp""$file_var
path_inv=$dir_temp""$file_inv
prefix_path_inv=$dir_temp""$prefix


#**********************************************************************************************
# Learning phase
#**********************************************************************************************
##########################################################################
# Prepare the target loop program
##########################################################################
echo -n -e $blue"Converting the given config file to a valid cplusplus file..."$normal
if [ $# -ge 2 ]; then
	if [ $# -ge 3 ]; then
		$dir_tool"cfg2test" $path_cfg $path_cpp $path_var $prefix_path_inv $2 $3
	else
		$dir_tool"cfg2test" $path_cfg $path_cpp $path_var $prefix_path_inv $2
	fi
else
	$dir_tool"cfg2test" $path_cfg $path_cpp $path_var $prefix_path_inv
fi
Nv=$?
echo -e $green$bold"[DONE]"$normal


##########################################################################
# Generate CMakeLists from cmake.base and Nv value
##########################################################################
echo -n -e $blue"Generating CMakeLists file for further construction..."$normal
cmakefile="./CMakeLists.txt"
echo "cmake_minimum_required (VERSION 2.8)" > $cmakefile
echo "set(Nv "$Nv")" >> $cmakefile
if [ $# -ge 4 ]; then
	if [ $4 -eq 1 ]; then
		echo "add_definitions(-D__QAS_POSITIVE)" >> $cmakefile
	fi
	if [ $4 -eq -1 ]; then
		echo "add_definitions(-D__QAS_NEGATIVE)" >> $cmakefile
	fi
	if [ $4 -eq 0 ]; then
		echo "add_definitions(-D__SELECTIVE_SAMPLING_ENABLED)" >> $cmakefile
	fi
fi
cat ./cmake.in >> $cmakefile
echo "add_executable("$prefix" "$path_cpp" \${DIR_SRCS} \${HEADER})" >> $cmakefile
echo "target_link_libraries("$prefix" \${Z3_LIBRARY})" >> $cmakefile
echo "target_link_libraries("$prefix" \${GSL_LIBRARIES})" >> $cmakefile
echo -e $green$bold"[DONE]"$normal



##########################################################################
# Build the project
##########################################################################
echo -e $blue"Build the project..."$normal
cd build
#rm -rf *
cmake .. > /dev/null
make $prefix
if [ $? -ne 0 ]; then
	echo -e $red$bold"[FAIL]make error, contact developer to fix project source code first..."$normal
	cd ..
	exit 1
fi
#echo -e $green$bold"[DONE]"$normal

cd ..
exit 0
