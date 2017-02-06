#!/bin/bash
red="\e[31m"
green="\e[32m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"


if [ $# -lt 1 ]; then
	echo "./run.sh needs more parameters"
	echo "./run.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)

dir_cfg=$dir_project"/cfg"

prefix=`basename -s .cfg $1`
if [ ! -f $dir_cfg"/"$prefix".cfg" ]; then
	if [ ! -f $1 ]; then
		echo -e $red"The argument is invalid, can not find a config file with name $1"
		exit 1
	fi
	cp $1 $dir_cfg
fi

grep "disjunctive" $dir_cfg"/"$prefix".cfg" 1>/dev/null 2>&1
if [ $? -eq 0 ]; then
	cp $dir_cfg"/"$prefix".cfg" $dir_project"/disj/cfg/"
	cd $dir_project"/disj"
	./run.sh $prefix
	ret=$?
	cd $dir_project
	exit $ret
fi

dir_test=$dir_project"/test"
dir_temp=$dir_project"/tmp"
mkdir -p tmp
mkdir -p build
mkdir -p test
file_cfg=$prefix".cfg"
path_cfg=$dir_cfg"/"$file_cfg
file_cpp=$prefix".cpp"
path_cpp=$dir_test"/"$file_cpp
file_var=$prefix".var"
path_var=$dir_temp"/"$file_var
file_cnt=$prefix".cnt"
path_cnt=$dir_temp"/"$file_cnt

file_dataset=$prefix".ds"
path_dataset=$dir_temp"/"$file_dataset

#rm -f $path_cnt
rm -f $path_dataset

##########################################################################
# BEGINNING 
##########################################################################

#echo "build....build...build"
if [ $# -lt 2 ]; then
	./scripts/build.sh $prefix $path_cnt $path_dataset 0 >/dev/null 2>&1
else
	if [ $# -lt 3 ]; then
		./scripts/build.sh $prefix $path_cnt $path_dataset $2  >/dev/null 2>&1
	else
		./scripts/build.sh $prefix $path_cnt $path_dataset $2 $3  >/dev/null 2>&1
	fi
fi

initseed=$RANDOM
if [ $# -ge 4 ]; then
	initseed=$4
fi

##echo "-----------------------"$prefix"--------------------------" >> tmp/statistics
echo -e $green"GEN INIT"$normal
./scripts/gen_init.sh $prefix
echo -e $green"DONE"$normal

#file_inv=$prefix".inv"
#path_inv=$dir_temp"/"$file_inv
#if [ $# -ge 3 ]; then
#	echo -e $blue"Using precondition as the invariant candidiate..."$normal
#	grep '^precondition=*' $path_cfg > $path_inv
#	sed -i 's/precondition=*//g' $path_inv
#	./scripts/verify.sh $prefix
#	if [ $? -eq 0 ]; then
#		exit 0
#	fi
#fi


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
	./$prefix $iteration $initseed
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
		#echo "=====================time========================="
		#echo -n -e $green$bold"------------------------------------------------------------- Iteration "
		#echo -e " Done -------------------------------------------------------------------"$normal$normal
		echo "SUCCEED. with iteration= "$iteration >> tmp/statistics
		exit 0
	else
		iteration=$(($iteration+1))
	fi
done
echo "FAILED. iteration= "$iteration >> tmp/statistics
exit $?

