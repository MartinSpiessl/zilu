#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

i=0
if [ $# -ge 1 ]; then
	folder=$1
else
	folder="cfg"
fi

if [ -d "./result" ]; then
	mv ./result ./result_`date +%m%d%h%M`
fi
mkdir -p result

#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
date >> result/statistics
for file in `ls $folder/*.cfg`
do
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	echo -e $yellow$bold$i$normal$yellow" --> Processing $file >> "$filename" --> "$cfgfile$normal

	#echo    $i >> result/statistics
	echo    $cfgfile >> result/statistics
	echo    "   ||----SELECTIVE---------------------------------------------------"
	{ time -p timeout 120 ./run.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective.out.txt 2>> result/statistics
	#echo -n -e "     ---->>"$bold$green
	#cat		"tmp/"$cfgfile".inv"
	#echo -e	""$normal 

	echo    "" >> result/statistics
	echo    "   ||----unSELECTIVE-------------------------------------------------"
	{ time -p timeout 120 ./run.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective.out.txt 2>> result/statistics
	#echo -n -e "     ---->>"$bold$green
	#cat		"tmp/"$cfgfile".inv"
	#echo -e	""$normal 


	echo    "" >> result/statistics
	echo    "" >> result/statistics

done
exit 0
