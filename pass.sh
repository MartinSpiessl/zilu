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
	folder="result"
fi

#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
#for file in `ls -d $folder`
for dir in `ls -l $folder/ | grep "^d" | awk '{print $9}'`
do
	i=$(($i+1))
	grep -nR "finish proving" $folder/$dir/selective.out.txt >/dev/null
	Sres=$?
	Sres=$((1-$Sres))
	grep -nR "finish proving" $folder/$dir/unselective.out.txt >/dev/null
	Ures=$?
	Ures=$((1-$Ures))
	echo $dir"  ->S "$Sres"   ->U "$Ures
done


exit 0
