#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"


if [ $# -lt 1 ]; then
	echo "./run_iterative.sh needs more parameters"
	echo "./run_iterative.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_cfg="cfg"
dir_test="test"
dir_temp="tmp"
dir_tool="tools/bin"

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

#rm -f $path_cnt
rm -f $path_dataset
rm -f $path_cnt_lib

##########################################################################
# BEGINNING 
##########################################################################
cd tools
./make_tools.sh
cd ..

if [ $# -lt 2 ]; then
	./build_project.sh $prefix $path_cnt $path_dataset 0
else
	./build_project.sh $prefix $path_cnt $path_dataset $2
fi

##echo "-----------------------"$prefix"--------------------------" >> tmp/statistics
./gen_init.sh $prefix

if [ $# -ge 3 ]; then
	echo -e $blue"Using precondition as the invariant candidiate..."$normal
	grep '^precondition=*' $path_cfg > $path_inv
	sed -i 's/precondition=*//g' $path_inv
	./scripts/verify.sh $prefix
	if [ $? -eq 0 ]; then
		exit 0
	fi
fi


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
	./$prefix
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
