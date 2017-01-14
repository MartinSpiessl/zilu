#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"


if [ $# -lt 1 ]; then
	echo "./tob.sh needs more parameters"
	echo "./tob.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
#dir_project=`dirname $dir_project`

dir_cfg=$dir_project"/cfg"
dir_test=$dir_project"/test"
dir_temp=$dir_project"/tmp"
dir_tool=$dir_project"/tools/bin"

# dir_cfg=$1

prefix=`basename -s .cfg $1`
if [ ! -f $dir_cfg"/"$prefix".cfg" ]; then
	if [ ! -f $1 ]; then
		echo -e $red"The argument is invalid, can not find a config file with name $1"
		exit 1
	fi
	prefix=$prefix"_copy"
	cp $1 $dir_cfg"/"$prefix".cfg"
fi

mkdir -p tmp
mkdir -p build
mkdir -p test
file_cfg=$prefix".cfg"
path_cfg=$dir_cfg"/"$file_cfg
file_cpp=$prefix".cpp"
path_cpp=$dir_test"/"$file_cpp
file_var=$prefix".var"
path_var=$dir_temp"/"$file_var
file_inv=$prefix".inv"
path_inv=$dir_temp"/"$file_inv
file_cnt=$prefix".cnt"
path_cnt=$dir_temp"/"$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp"/"$file_cnt_lib

file_dataset=$prefix".ds"
path_dataset=$dir_temp"/"$file_dataset

target=$prefix
if [ $# -ge 2 ]; then
	if [ $2 -eq 0 ]; then
		target=$prefix"_selective"
	else
		target=$prefix"_unselective"
	fi
fi

##echo "-----------------------"$prefix"--------------------------" >> tmp/statistics
echo -e $green"GEN INIT"$normal
./scripts/gen_init.sh $prefix


iteration=1
echo -e $blue"Running the project to generate invariant candidiate..."$normal
while [ $iteration -le 128 ]; do
	echo -n -e $green$bold"--------------------------------------------- Iteration "
	echo -n -e $iteration
	echo -e " --------------------------------------------------------"$normal$normal
	##########################################################################
	# Run the target to get Invariant Candidates
	##########################################################################
	cd build
	./$target
	ret=$?
	if [ $ret -ne 0 ]; then
		echo -e $red$bold"can not get an invariant candidate, read log file to find out more."$normal$normal
		exit 1
	fi
	cd ..


	#**********************************************************************************************
	# verification phase
	#**********************************************************************************************
	./scripts/verify.sh $prefix
	if [ $? -eq 0 ]; then
		echo ""
		echo "=====================time========================="
		#echo -n -e $green$bold"------------------------------------------------------------- Iteration "
		#echo -e " Done -------------------------------------------------------------------"$normal$normal
		echo "SUCCEED. with iteration= "$iteration >> tmp/statistics
		exit 0
	else
		iteration=$(($iteration+1))
	fi
	#cat $dir_temp"svm.ds" > $path_dataset
done
echo "FAILED. iteration= "$iteration >> tmp/statistics
exit $?
