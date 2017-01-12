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
	exit 1
fi

#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for dir in `ls -l $folder/ | grep "^d" | awk '{print $9}'`
do
	echo $dir
	i=$(($i+1))
	./est $folder"/"$dir"/statistics" > $folder"/"$dir".txt"
done

exit 0
