#!/bin/bash
dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_project=`dirname $dir_project`
. $dir_project"/scripts/include.sh"

Nv=0
Nb=0

dir_org=`pwd`

prefix=$1
mkdir -p tmp
file_cfg=$prefix".cfg"
file_cpp=$prefix".cpp"
file_var=$prefix".var"
file_inv=$prefix".inv"
path_cfg=$dir_cfg"/"$file_cfg
path_cpp=$dir_test"/"$file_cpp
path_var=$dir_temp"/"$file_var
path_inv=$dir_temp"/"$file_inv
#prefix_path_inv=$dir_temp"/"$prefix



if [ $# -lt 1 ]; then
	echo "./build.sh needs more parameters"
	echo "./build.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi
cd $dir_project

#**********************************************************************************************
# Learning phase
#**********************************************************************************************
##########################################################################
# Prepare the target loop program
##########################################################################
echo -n -e $blue"Converting the given config file to a valid cplusplus file..."$normal

#cd $dir_parser
#make
#make clean
#cd ..

#echo -e $green$bold$path_var$normal
cat $path_cfg | $dir_parser"/parser" -t 2 -o $path_cpp -v $path_var -i $path_inv -c $2 -N $dir_temp"/"$prefix".nvnb" -m $dir_test"/"$prefix".pc" -V
read -r Nv Nb < $dir_temp"/"$prefix".nvnb"

cp $dir_test"/"$prefix".cnt.init" $dir_temp"/"$prefix".cnt"

i=0
while [ $i -lt $Nb ]; do
	cat $path_cfg | $dir_parser"/parser" -t 3 -o $dir_temp"/"$prefix"_b"$i".cpp" -b $i
	i=$(($i+1))
done
#cat $path_cfg | $dir_tool"/parser/parser" -t 3 -o $dir_tmp"/"$prefix"_b0.cpp" -b 0 
#cat $path_cfg | $dir_tool"/parser/parser" -t 3 -o $dir_tmp"/"$prefix"_b1.cpp" -b 1 
#cat test/new2.cfg| ./parser -t 2 -c new2.ce -o new2.cpp -v new2.var -i new2.inv
#Nv=$?
echo -e $green$bold"[DONE]"$normal


##########################################################################
# Generate CMakeLists from cmake.base and Nv value
##########################################################################
echo -n -e $blue"Generating CMakeLists file for further construction..."$normal
cmakefile="$dir_project/CMakeLists.txt"
#cmakefile="./CMakeLists.txt"
echo "cmake_minimum_required (VERSION 2.8)" > $cmakefile
#echo "set(Nv "$Nv")" >> $cmakefile
echo "set(Nv "$Nv")" >> $cmakefile
echo "set(Nb "$Nb")" >> $cmakefile
echo "set(Project "$prefix")" >> $cmakefile
echo "set(ProjectHome "$dir_project")" >> $cmakefile
		

if [ $# -ge 4 ]; then
	if [ $4 -eq 0 ]; then
		echo "add_definitions(-D__SELECTIVE_SAMPLING_ENABLED)" >> $cmakefile
		echo "selective enabled."
	else
		echo "selective NOTenabled."
	fi
fi

if [ $# -ge 5 ]; then
	if [ $5 -eq 1 ]; then
		echo "add_definitions(-D__QAS_POSITIVE)" >> $cmakefile
	fi
	if [ $5 -eq -1 ]; then
		echo "add_definitions(-D__QAS_NEGATIVE)" >> $cmakefile
	fi
fi
cat ../cmake.in >> $cmakefile
echo "add_executable("$prefix" "$path_cpp" \${DIR_SRCS} \${HEADER})" >> $cmakefile
echo "target_link_libraries("$prefix" \${Z3_LIBRARY})" >> $cmakefile
echo "target_link_libraries("$prefix" \${GSL_LIBRARIES})" >> $cmakefile
echo -e $green$bold"[DONE]"$normal


##########################################################################
# Build the project
##########################################################################
echo -e $blue"Build the project..."$normal
cd $dir_build
#rm -rf *
cmake .. > /dev/null
make $prefix
if [ $? -ne 0 ]; then
	echo -e $red$bold"[FAIL]make error, contact developer to fix project source code first..."$normal
	cd ..
	exit 1
fi
cd $dir_project
rm $cmakefile
#echo -e $green$bold"[DONE]"$normal

cd $dir_org
exit 0
