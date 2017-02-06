#!/bin/bash
dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_include=$dir_project"/scripts"
. $dir_include"/include.sh"

if [ $# -lt 1 ]; then
	echo "./run.sh needs more parameters"
	echo "./run.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi


prefix=`basename -s .cfg $1`
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

mkdir -p $dir_temp
mkdir -p $dir_build
mkdir -p $dir_test
#rm -f $path_cnt
rm -f $path_dataset
rm -f $path_cnt_lib
rm -f $dir_temp"/candidate.h"

##########################################################################
# BEGINNING 
##########################################################################
#./build_project.sh $prefix $path_cnt $path_dataset $2
#$dir_script"/build.sh" $prefix $path_cnt $path_dataset 0 #0 means selective sampling
##echo "-----------------------"$prefix"--------------------------" >> tmp/statistics
#$dir_script"/gen_init.sh" $prefix

#cp $dir_project"/../config.h.in" $dir_project
if [ $# -lt 2 ]; then
	./scripts/build.sh $prefix $path_cnt $path_dataset 0
else
	if [ $# -lt 3 ]; then
		./scripts/build.sh $prefix $path_cnt $path_dataset $2  >/dev/null 2>&1
	else
		./scripts/build.sh $prefix $path_cnt $path_dataset $2 $3  >/dev/null 2>&1
	fi
fi



cd $dir_build 
echo -e $blue"Running the project to generate invariant candidiate..."$normal
./$prefix
ret=$?
cd $dir_project

if [ $ret -ne 0 ]; then
	echo -e $red$bold"can not get an invariant candidate, read log file to find out more."$normal$normal
fi
echo "end........................"
exit $ret
