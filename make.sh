#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"


#if [ $# -lt 1 ]; then
#	echo "./make.sh needs more parameters"
#	echo "./make.sh cofig_file_folder"
#	echo "try it again..."
#	exit 1
#fi

dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
#dir_project=`dirname $dir_project`

#dir_cfg=$1
dir_cfg=$dir_project"/cfg"
dir_temp=$dir_project"/tmp"


mkdir -p tmp
rm -rf tmp/*
mkdir -p build
mkdir -p test


cd tools
./make_tools.sh
cd ..


i=1
for file in `ls $dir_cfg/*.cfg`
do
	prefix=`basename -s .cfg $file`
	file_cnt=$prefix".cnt"
	path_cnt=$dir_temp"/"$file_cnt
	file_dataset=$prefix".ds"
	path_dataset=$dir_temp"/"$file_dataset
	echo -e $i">"$yellow"building $prefix selective & unselective..."
	./scripts/build.sh $prefix $path_cnt $path_dataset 0 $prefix"_selective">/dev/null 2>&1
	echo -e "  |--"$blue"selective   "$green"[done]"$normal
	./scripts/build.sh $prefix $path_cnt $path_dataset 1 $prefix"_unselective">/dev/null 2>&1
	echo -e "  |--"$blue"unselective "$green"[done]"$normal
	i=$(($i+1))
done
