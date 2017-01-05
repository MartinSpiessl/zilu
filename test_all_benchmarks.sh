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

#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for file in `ls $folder/*.cfg`
do
	date >> result/statistics
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	#echo -e $yellow""$i" --> Processing $file >> "$filename" --> "$cfgfile""$normal
	echo -e $yellow$bold$i$normal$yellow" --> Processing $file >> "$filename" --> "$cfgfile$normal

	echo    "No.="$i >> result/statistics
	echo    "testcase="$cfgfile >> result/statistics
	echo    "testmode=selective" >> result/statistics
	echo    "   ||----SELECTIVE---------------------------------------------------"
	{ time -p timeout 120 ./run.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective.out.txt 2>> result/statistics
	echo    "return_code="$? >> result/statistics
	echo -n -e "     ---->>"$bold$green
	cat		"tmp/"$cfgfile".inv"
	echo -e	""$normal 

	echo    "   ||----unSELECTIVE-------------------------------------------------"
	echo    "testmode=unselective" >> result/statistics
	{ time -p timeout 120 ./run.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective.out.txt 2>> result/statistics
	echo    "return_code="$? >> result/statistics
	echo -n -e "     ---->>"$bold$green
	cat		"tmp/"$cfgfile".inv"
	echo -e	""$normal 


	echo    "" >> result/statistics
	echo    "" >> result/statistics

done
exit 0
