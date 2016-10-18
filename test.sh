#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

i=0
if [ $# -ge 1 ] && [ $1 !=  "all" ]; then
	echo    "--"$1"------------------------------------------------------------------------------"
	echo    "   ||----SELECTIVE---------------------------------------------------"
	echo    "\n--"$1"------------------------------------------------------------------------------" >> result/statistics
	echo -n "   ||----SELECTIVE---------------------------------------------------" >> result/statistics
	echo    "" >> result/statistics
	{ time -p timeout 120 ./run_iterative.sh $1 0 ; } 1> result/"$1".selective.out.txt 2>> result/statistics
	echo    "   ||----unSELECTIVE-------------------------------------------------"
	echo    "   ||----unSELECTIVE-------------------------------------------------" >> result/statistics
	{ time -p timeout 120 ./run_iterative.sh $1 1 ; } 1> result/"$1".unselective.out.txt 2>> result/statistics
	date >> result/statistics
	exit 0
fi


#date >> result/statistics
#for file in `find cfg/oopsla13-cfg -maxdepth 1 -name '*.cfg'`
#for file in `ls cfg/oopsla13-cfg/*.cfg`
#do
#	i=$(($i+1))
#	cfgfile=`basename -s .cfg $file`
#	echo $i" --> Processing $file --> $cfgfile ..." 
#	echo    "\n"$i"--"$file"------------------------------------------------------------------------------" >> result/statistics
#	echo    "" >> result/statistics
#	{ time -p timeout 120 ./run_iterative.sh $file 0 ; } 1> result/"$cfgfile".out.txt 2>> result/statistics
#	date >> result/statistics
#done
#exit 0

date >> result/statistics
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
do
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	#cfgfile=$file
	#echo -e $yellow""$i" --> Processing $file >> "$filename" --> "$cfgfile""$normal
	echo -e $yellow$bold$i$normal$yellow" --> Processing $file >> "$filename" --> "$cfgfile$normal

	echo    $i"--"$cfgfile"------------------------------------------------------------------------------" >> result/statistics
	echo    "   ||----SELECTIVE---------------------------------------------------"
	echo    "   ||----SELECTIVE---------------------------------------------------" >> result/statistics
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective.out.txt 2>> result/statistics
	echo -n -e "     ---->>"$bold$green
	cat		"tmp/"$cfgfile".inv"
	echo -e	""$normal 

	echo    "   ||----unSELECTIVE-------------------------------------------------"
	echo    "   ||----unSELECTIVE-------------------------------------------------" >> result/statistics
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective.out.txt 2>> result/statistics
	echo -n -e "     ---->>"$bold$green
	cat		"tmp/"$cfgfile".inv"
	echo -e	""$normal 

	echo    "" >> result/statistics
	echo    "" >> result/statistics
	echo    "" >> result/statistics
	echo    "" >> result/statistics
	echo    "##########################################################################################" >> result/statistics
	date >> result/statistics
done
exit 0


date >> result/statistics
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
do
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	#cfgfile=$file
	#echo -e $yellow""$i" --> Processing $file >> "$filename" --> "$cfgfile""$normal
	echo $i" --> Processing $file >> "$filename" --> "$cfgfile

	echo    "\n"$i"--"$cfgfile"------------------------------------------------------------------------------" >> result/statistics
	echo    "   ||----SELECTIVE@1---------------------------------------------------" >> result/statistics
	echo    "   ||----SELECTIVE@1---------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective@1.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 
	
	echo    "   ||----SELECTIVE@2---------------------------------------------------" >> result/statistics
	echo    "   ||----SELECTIVE@2---------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective@2.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 

	echo    "   ||----SELECTIVE@3---------------------------------------------------" >> result/statistics
	echo    "   ||----SELECTIVE@3---------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective@3.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 


	echo    "   ||----unSELECTIVE@1-------------------------------------------------" >> result/statistics
	echo    "   ||----unSELECTIVE@1-------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective@1.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 

	echo    "   ||----unSELECTIVE@2-------------------------------------------------" >> result/statistics
	echo    "   ||----unSELECTIVE@2-------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective@2.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 

	echo    "   ||----unSELECTIVE@3-------------------------------------------------" >> result/statistics
	echo    "   ||----unSELECTIVE@3-------------------------------------------------"
	{ time -p timeout 120 ./run_iterative.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective@3.out.txt 2>> result/statistics
	echo -n "      ****"
	cat		"tmp/"$cfgfile".inv"
	echo	"" 

	echo    "##########################################################################################" >> result/statistics
	date >> result/statistics
done
