#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

zilu_path="./zilu-test"
cpa_path="./cpa-test"
interproc_path="./interproc-test"
mkdir -p $cpa_path
mkdir -p $interproc_path


option1=0
option2=0
if [ $# -lt 1 ]; then
	option1=1
	option2=1
else 
	if [ $1 -eq 1 ]; then
		option1=1;
	else 
		if [ $1 -eq 2 ]; then
			option2=1;
		fi
	fi
fi



g++ zilu2cpa.cpp -o zilu2cpa
if [ $option1 -eq 1 ]; then
	cd $zilu_path
	rm *.cpp > /dev/null
	echo -e -n $yellow"Converting zilu test file to CPAchecker test file "
	for file in `find . -name '*.cfg'`
	do
		echo -e -n $blue"."$file
		#echo -e -n $blue"."
		../zilu2cpa $file
	done
	echo -e $green" [DONE]"$normal

	cd ..
	rm $cpa_path/*.cpp
	mv $zilu_path/*.cpp $cpa_path
fi
rm zilu2cpa





g++ zilu2interproc.cpp -o zilu2interproc
if [ $option2 -eq 1 ]; then
	cd $zilu_path
	rm *.txt
	echo -e -n $yellow"Converting zilu test file to INTERPROC test file "
	for file in `find . -name '*.cfg'`
	do
		echo -e -n $blue"."$file
		#echo -e -n $blue"."
		../zilu2interproc $file
	done
	echo -e $green" [DONE]"$normal

	cd ..
	rm $interproc_path/*.txt
	mv $zilu_path/*.txt $interproc_path
fi

for file in `find $interproc_path -name '*.txt'`
do
	sed -i "s/\ &&\ /\ and\ /g" $file
	sed -i "s/\ &&/\ and\ /g" $file
	sed -i "s/&&\ /\ and\ /g" $file
	sed -i "s/&&/\ and\ /g" $file
	sed -i "s/\ ||\ /\ or\ /g" $file
	sed -i "s/\ ||/\ or\ /g" $file
	sed -i "s/||\ /\ or\ /g" $file
	sed -i "s/||/\ or\ /g" $file

	sed -i "s/\([a-zA-Z][a-zA-Z_0-9]*\)++/\1=\1+1/g" $file
	sed -i "s/\([a-zA-Z][a-zA-Z_0-9]*\)--/\1=\1-1/g" $file
	sed -i "s/\([a-zA-Z][a-zA-Z_0-9]*\)+=/\1=\1+/g" $file
	sed -i "s/\([a-zA-Z][a-zA-Z_0-9]*\)-=/\1=\1-/g" $file
done
rm zilu2interproc
