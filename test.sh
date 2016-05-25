#!/bin/sh
i=0
if [ $# -ge 1 ]; then
	echo "--"$1"------------------------------------------------------------------------------"
	echo "   ||----SELECTIVE---------------------------------------------------"
	echo "\n--"$1"------------------------------------------------------------------------------" >> tmp/statistics
	echo -n "   ||----SELECTIVE---------------------------------------------------" >> tmp/statistics
	echo "" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $1 0 ; } 1> result/"$1".selective.out.txt 2>> tmp/statistics
	echo "   ||----unSELECTIVE-------------------------------------------------"
	echo "   ||----unSELECTIVE-------------------------------------------------" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $1 ; } 1> result/"$1".unselective.out.txt 2>> tmp/statistics
	date >> tmp/statistics
	exit 0
fi


date >> tmp/statistics
for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
do
	i=$(($i+1))
	length=$((${#file}-4))
	filename=${file#cfg/}
	cfgfile=${filename%?cfg}
	#echo -e $yellow""$i" --> Processing $file >> "$filename" --> "$cfgfile""$normal
	echo $i" --> Processing $file >> "$filename" --> "$cfgfile

	#mv $file interproc_`basename -s .txt $file`.cfg
	# { timeout 60 ./interproc/interproc.opt $file ; } > results/`basename $file`.out.txt # 2> result/`basename $file`.time.txt
	# time timeout 360 ./run_iterative.sh $cfgfile 0 # > result/`$cfgfile`.selective.out.txt 2> tmp/statistics
	echo "\n"$i"--"$cfgfile"------------------------------------------------------------------------------" >> tmp/statistics
	echo -n "   ||----SELECTIVE---------------------------------------------------" >> tmp/statistics
	echo "   ||----SELECTIVE---------------------------------------------------"
	echo "" >> tmp/statistics
	{ time -p timeout 600 ./run_iterative.sh $cfgfile 0 ; } 1> result/"$cfgfile".selective.out.txt 2>> tmp/statistics
	echo "   ||----unSELECTIVE-------------------------------------------------" >> tmp/statistics
	echo "   ||----unSELECTIVE-------------------------------------------------"
	{ time -p timeout 600 ./run_iterative.sh $cfgfile ; } 1> result/"$cfgfile".unselective.out.txt 2>> tmp/statistics
	#if [ $i -ge 2 ]; then
	#	exit 0
	#fi
	date >> tmp/statistics
done
