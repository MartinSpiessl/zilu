#!/bin/sh
i=0
if [ $# -ge 1 ] && [ $1 !=  "all" ]; then
	echo    "--"$1"------------------------------------------------------------------------------"
	echo    "   ||----SELECTIVE---------------------------------------------------"
	echo    "\n--"$1"------------------------------------------------------------------------------" >> tmp/statistics
	echo -n "   ||----SELECTIVE---------------------------------------------------" >> tmp/statistics
	echo    "" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $1 0 ; } 1> result/"$1".selective.out.txt 2>> tmp/statistics
	echo    "   ||----unSELECTIVE-------------------------------------------------"
	echo    "   ||----unSELECTIVE-------------------------------------------------" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $1 1 ; } 1> result/"$1".unselective.out.txt 2>> tmp/statistics
	date >> tmp/statistics
	exit 0
fi


date >> tmp/statistics
#for file in `find cfg/oopsla13-cfg -maxdepth 1 -name '*.cfg'`
for file in `ls cfg/oopsla13-cfg/*.cfg`
do
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	echo $i" --> Processing $file --> $cfgfile ..." 
	echo    "\n"$i"--"$file"------------------------------------------------------------------------------" >> tmp/statistics
	echo    "" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $file 0 ; } 1> result/"$cfgfile".out.txt 2>> tmp/statistics
	date >> tmp/statistics
done
exit 0


date >> tmp/statistics
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for file in `find cfg/oopsla13-cfg -maxdepth 1 -name '*.cfg'`
do
	i=$(($i+1))
	#cfgfile=`basename -s .cfg $file`
	cfgfile=$file
	#echo -e $yellow""$i" --> Processing $file >> "$filename" --> "$cfgfile""$normal
	echo $i" --> Processing $file >> "$filename" --> "$cfgfile

	echo    "\n"$i"--"$cfgfile"------------------------------------------------------------------------------" >> tmp/statistics
	echo -n "   ||----SELECTIVE---------------------------------------------------" >> tmp/statistics
	echo    "   ||----SELECTIVE---------------------------------------------------"
	echo    "" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective.out.txt 2>> tmp/statistics
	echo    "   ||----unSELECTIVE-------------------------------------------------" >> tmp/statistics
	echo    "   ||----unSELECTIVE-------------------------------------------------"
	{ time -p timeout 600 ./run_iterative.sh $cfgfile 1 ; } 1> result/"$cfgfile".unselective.out.txt 2>> tmp/statistics
	date >> tmp/statistics
done
