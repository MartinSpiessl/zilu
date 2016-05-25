#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
bold="\e[1m"
normal="\e[0m"

if [ $# -lt 1 ]
then
	echo "./run_once.sh needs more parameters"
	echo "./run_once.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_cfg="cfg/"
dir_test="test/"
dir_temp="tmp/"
dir_tool="tools/bin/"


prefix=$1
mkdir -p tmp
mkdir -p build
file_cfg=$prefix".cfg"
path_cfg=$dir_cfg""$file_cfg
file_cpp=$prefix".cpp"
path_cpp=$dir_test""$file_cpp
file_var=$prefix".var"
path_var=$dir_temp""$file_var
file_inv=$prefix".inv"
path_inv=$dir_temp""$file_inv
file_cnt=$prefix".cnt"
path_cnt=$dir_temp""$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp""$file_cnt_lib


file_dataset=$prefix".ds"
path_dataset=$dir_temp""$file_dataset

#rm -f $path_cnt
rm -f $path_dataset
rm -f $path_cnt_lib
##########################################################################
# BEGINNING 
##########################################################################
cd tools
./make_tools.sh
cd ..

#./build_project.sh $prefix
./build_project.sh $prefix $path_cnt $path_dataset $2

if [ $# -ge 3 ]; then
	echo -e $blue"Using precondition as the invariant candidiate..."$normal
	grep '^precondition=*' $path_cfg > $path_inv
	sed -i 's/precondition=*//g' $path_inv
	./verify.sh $prefix
	if [ $? -eq 0 ]; then
		exit 0
	fi
fi
##########################################################################
# Run the target to get Invariant Candidates
##########################################################################
cd build
echo -e $blue"Running the project to generate invariant candidiate..."$normal
./$prefix
ret=$?
if [ $ret -ne 0 ]; then
	echo -e $red$bold"can not get an invariant candidate, read log file to find out more."$normal$normal
	#echo "can not separate using default paramater"
	#echo "try more parameters to get a perfect classifier"
	exit 1
fi
cd ..


#**********************************************************************************************
# verification phase
#**********************************************************************************************
./verify.sh $prefix

exit $?
