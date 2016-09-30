#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
bold="\e[1m"
normal="\e[0m"

if [ $# -lt 1 ]
then
	echo "gen_init.sh needs more parameters"
	echo "gen_init.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

prefix=$1

dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_project=`dirname $dir_project`
. $dir_project"/scripts/include.sh"


dir_pwd=`pwd`
cd $dir_project
mkdir -p tmp

file_cfg=$prefix".cfg"
path_cfg=$dir_cfg"/"$file_cfg
file_var=$prefix".var"
path_var=$dir_temp"/"$file_var
file_cnt=$prefix".cnt"
path_cnt=$dir_temp"/"$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp"/"$file_cnt_lib

file_verif=$prefix".c"
path_verif=$dir_temp"/"$file_verif
file_c_verif=$prefix"_init.c"
file_o_verif=$prefix"_init.o"


function func_findSmtForZ3(){
smtname="failAssert0000";
n=99
i=1
while [ $i -lt $n ]; do
	if [ ! -f $smtname""$i".smt2" ]; then
		#echo  "No such file."
		return 0
	fi
	path_smt2=$smtname""$i".smt2"
	path_model=$smtname""$i".model"
	#echo -n "  |-- processing "$path_smt2" ---> "
	$dir_tool"/smt2_bv2int.sh" $path_smt2 
	$dir_parser"/smt2solver" $path_smt2 > $path_model
	result=$?
	if [ $result -gt 1 ]; then
		echo -e $red$bold"A Error Occurs during smt2solver"$normal
		exit 2 
	fi

	sed -i 's/\ \ /\ /g' $path_model
	sed -i 's/(define-fun\ \([a-zA-Z_][a-zA-Z_0-9]*\)\ ()\ Int/\1/g' $path_model
	sed -i 's/()//g' $path_model
	sed -i 's/Int//g' $path_model
	sed -i 's/(-\ \([1-9][0-9]*\))/-\1/g' $path_model
	sed -i 's/)//g' $path_model
	sed -i 's/\ \ /\ /g' $path_model

	cat $path_model | $dir_parser"/model_parser" $path_var $path_cnt
	result=$?
	#cat $path_model
	#cat $path_cnt
	if [ $result -eq 0 ]; then
		# unsat
		#echo -e $green$bold" [unsat] [PASS]"$normal
		i=$(($i+1))
	elif [ $result -eq 2 ]; then
		#echo -e $red$bold"Error Occurs during model parsing"$normal
		exit 2 
	else
		#echo -n -e $red$bold" [sat] [FAIL]"$normal
		echo -e " >>> an init value is stored at "$yellow$path_cnt$normal
		#cat "../../"$path_cnt >> "../../"$path_cnt_lib
		return 1
	fi
done
return 0
}

function KleeInit(){
cd $dir_temp"/"$prefix"_init" 
rm -rf klee-*
rm -rf *.smt2
llvm-gcc --emit-llvm -c -g $file_c_verif > /dev/null
#echo -e $blue"Running KLEE to generate path condition"$normal
klee -write-smt2s $file_o_verif > /dev/null 2>&1
#klee -write-smt2s $file_o_verif #> /dev/null 2>&1
ret=$?
func_findSmtForZ3
ret=$?
#cat $path_cnt
#echo -n -e $red$ret$normal
if [ $ret -eq 2 ]; then
	exit $ret
elif [ $ret -eq 1 ]; then
	#echo -n -e $red">>>Getting an initial input..."$normal
	exit $ret
fi
#echo -e $blue"[PASS]"$normal
cd $dir_project
return 0
}


##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
echo -n -e $blue"Generating a C file to get initial positive value by KLEE..."$normal
$dir_parser"/cfg2init" $path_cfg $path_verif
echo -e $green$bold"[Done]"$normal


##########################################################################
# File preparation for verificattion
##########################################################################
cd $dir_temp
mkdir -p $prefix"_init" 
mv $file_c_verif $prefix"_init/"$file_c_verif
cd $dir_project

KleeInit

cd $dir_pwd
exit 0
