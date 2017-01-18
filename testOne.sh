#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

if [ $# -lt 1 ]; then
	exit 1
fi
file=$1
file=`basename -s .cfg $file`

#if [ -d "./result/"$file ]; then
#	mv "./result/"$file "./result/"$file"_old_"`date +%m%d%H%M`
#fi
mkdir -p result/$file

total=3
if [ $# -ge 2 ]; then
	total=$2
fi

qas=0
if [ $# -ge 3 ]; then
	qas=$3
fi

i=1
while [ $i -le $total ]; do
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
#date >> result/statistics
	echo -e $yellow"> Processing >>>>> ["$file"] ------ "$i" -------------------------------------------------------------------"$normal

	rm -rf tmp/$file".cnt"
	rm -rf tmp/$file".ds"
	rm -rf tmp/$file"_init"
	rm -rf tmp/$file"_klee1"
	rm -rf tmp/$file"_klee2"
	rm -rf tmp/$file"_klee3"
	echo    $file>> result/statistics
	echo    0  $qas >> result/statistics
	echo    "    |----SELECTIVE---------[$qas]------------------------------------------"
	{ time -p timeout 120 ./run.sh $file 0 $qas ; } 1> result/$file/$i.$qas.selective.out.txt 2>> result/statistics
	grep -nR "finish proving" result/$file/$i.$qas.selective.out.txt >/dev/null
	res=$?
	echo -n "Pass?Fail?  " >> result/statistics
	if [ $res -eq 0 ]; then
		echo -n -e "      --->> "$bold$green
		cat		"tmp/"$file".inv"
		echo -e	""$normal 
	fi
	echo $((1-$res)) >> result/statistics


	rm -rf tmp/$file".cnt"
	rm -rf tmp/$file".ds"
	rm -rf tmp/$file"_init"
	rm -rf tmp/$file"_klee1"
	rm -rf tmp/$file"_klee2"
	rm -rf tmp/$file"_klee3"
	echo    "" >> result/statistics
	echo    $file>> result/statistics
	echo    1  $qas >> result/statistics
	echo    "    |----unSELECTIVE-------[$qas]------------------------------------------"
	{ time -p timeout 120 ./run.sh $file 1 $qas ; } 1> result/$file/$i.$qas.unselective.out.txt 2>> result/statistics
	grep -nR "finish proving" result/$file/$i.$qas.unselective.out.txt >/dev/null
	res=$?
	echo -n "Pass?Fail?  " >> result/statistics
	if [ $res -eq 0 ]; then
		echo -n -e "      --->> "$bold$green
		cat		"tmp/"$file".inv"
		echo -e	""$normal 
	fi
	echo $((1-$res)) >> result/statistics
	#echo $res >> result/statistics

	echo    "" >> result/statistics
	echo    "" >> result/statistics
	i=$(($i+1))
done

exit 0
