#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
bold="\e[1m"
normal="\e[0m"

if [ $# -lt 1 ]
then
	echo "./verify.sh needs more parameters"
	echo "./verify.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_cfg="cfg/"
dir_test="test/"
dir_temp="tmp/"
dir_tool="tools/bin/"


prefix=$1
mkdir -p tmp

file_cfg=$prefix".cfg"
path_cfg=$dir_cfg""$file_cfg
file_var=$prefix".var"
path_var=$dir_temp""$file_var
file_cnt=$prefix".cnt"
path_cnt=$dir_temp""$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp""$file_cnt_lib

file_verif=$prefix".c"
path_verif=$dir_temp""$file_verif
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
	"../../tools/smt2_bv2int.sh" $path_smt2 
	"../../tools/bin/smt2solver" $path_smt2 > $path_model
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

	cat $path_model | "../../"$dir_tool"model_parser" "../../"$path_var "../../"$path_cnt
	result=$?
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
u=$1
cd $prefix"_init" 
rm -rf klee-*
rm -rf *.smt2
llvm-gcc --emit-llvm -c -g $file_c_verif > /dev/null
#echo -e $blue"Running KLEE to generate path condition"$normal
klee -write-smt2s $file_o_verif > /dev/null 2>&1
#klee -write-smt2s $file_o_verif #> /dev/null 2>&1
ret=$?
func_findSmtForZ3
ret=$?
#echo -n -e $red$ret$normal
if [ $ret -eq 2 ]; then
	exit $ret
fi
if [ $ret -eq 1 ]; then
	#echo -n -e $red">>>Getting an initial input..."$normal
	exit $ret
fi
#echo -e $blue"[PASS]"$normal
cd ..
return 0
}


##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
echo -n -e $blue"Generating a C file to get initial positive value by KLEE..."$normal
$dir_tool"cfg2init" $path_cfg $path_verif
echo -n -e $green$bold"[Done]"$normal


##########################################################################
# File preparation for verificattion
##########################################################################
cd $dir_temp
mkdir -p $prefix"_init" 
mv $file_c_verif $prefix"_init/"$file_c_verif

KleeInit

cd ..
exit 0
