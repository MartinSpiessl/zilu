#!/bin/bash

if [ $# -lt 1 ]; then
	echo "verify.sh needs more parameters"
	echo "verify.sh cofig_prefix"
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
file_inv=$prefix".inv"
path_inv=$dir_temp"/"$file_inv
file_cnt=$prefix".cnt"
path_cnt=$dir_temp"/"$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp"/"$file_cnt_lib

file_verif=$prefix".c"
path_verif=$dir_temp"/"$file_verif
file_c_verif=$prefix"_klee.c"
file_c1_verif=$prefix"_klee1.c"
file_c2_verif=$prefix"_klee2.c"
file_c3_verif=$prefix"_klee3.c"
file_o_verif=$prefix"_klee.o"

cnt_num=8

function func_findSmtForZ3(){
smtname="kleeAssert0000";
n=99
i=1
while [ $i -lt $n ]; do
	if [ ! -f $smtname""$i".smt2" ]; then
		#echo  "No such file."
		return 0
	fi
	path_smt2=$smtname""$i".smt2"
	path_model=$smtname""$i".model"
	echo -n "  |-- processing "$path_smt2" ---> "
	#cat $path_smt2
	#z3 $path_smt2
	#echo "==========================="
	$dir_tool"/smt2_bv2int.sh" $path_smt2 
	#cat $path_smt2
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
	if [ $result -eq 0 ]; then
		# unsat
		echo -e $green$bold" [unsat] [PASS]"$normal
		i=$(($i+1))
	elif [ $result -eq 2 ]; then
		echo -e $red$bold"Error Occurs during model parsing"$normal
		exit 2 
	else
		echo -n -e $red$bold" [sat] [FAIL]"$normal
		echo -e " >>> counter example is stored at "$yellow$path_cnt$normal
		#cat "../../"$path_cnt >> "../../"$path_cnt_lib
		return 1
	fi
done
return 0
}


function new_func_findSmtForZ3(){
smtname="kleeAssert0000";
n=99
u=1
while [ $u -lt $n ]; do
	if [ $u -ge 10 ]; then
		smtname="kleeAssert000";
	fi
	path_smt2=$smtname""$u".smt2"
	path_model=$smtname""$u".model"
	if [ ! -f $path_smt2 ]; then
		#echo  "No such file."
		return 0
	fi
	echo -n "  |-- processing $smtname$u.smt2 ---> "
	func_bv2int $path_smt2 
	res=$?
	echo -e $yellow"smt2:"$normal
	cat $path_smt2
	func_z3solve $path_smt2 $path_model
	res=$?
	echo -e $yellow"model:"$normal
	cat $path_model
	if [ $res -eq 0 ]; then
		func_modelfile2values $path_model
		echo ${values[*]} > $path_cnt
		echo ${values[*]}
		cnt_num=$(($cnt_num-1))
		echo "cnt_num-->"$cnt_num
		while [ $cnt_num -gt 0 ]; do
			func_updatesmt2file $path_smt2
			res=$?
			echo -e $yellow"smt2:"$normal
			cat $path_smt2
			func_z3solve $path_smt2 $path_model
			res=$?
			echo -e $yellow"model:"$normal
			cat $path_model
			if [ $res -eq 0 ]; then
				func_modelfile2values $path_model
				echo ${values[*]} >> $path_cnt
				echo ${values[*]}
				cnt_num=$(($cnt_num-1))
			else
				break
			fi
		done


		echo -n -e $red$bold" [sat] [FAIL]"$normal
		echo -e " >>> counter example(s) are  stored at "$yellow$path_cnt$normal
		cnt_num=8
		return 1
	fi

	#echo -e -n $red$bold"z3solve exits with non-zero value"$normal
	echo -e $green$bold" [unsat] [PASS]"$normal
	u=$(($u+1))
done
return 0
}


function KleeVerify(){
u=$1
cd $dir_temp"/"$prefix"_klee"$u 
rm -rf klee-*
rm -rf *.smt2
echo -e $green"Compiling the C files and Run KLEE..."$u$normal
llvm-gcc --emit-llvm -c -g $file_c_verif > /dev/null
#echo -e $blue"Running KLEE to generate path condition"$normal
klee -write-smt2s $file_o_verif > /dev/null 2>&1
#klee -write-smt2s $file_o_verif #> /dev/null 2>&1
ret=$?
new_func_findSmtForZ3
ret=$?
#echo -n -e $red$ret$normal
if [ $ret -eq 2 ]; then
	exit $ret
fi
if [ $ret -eq 1 ]; then
	echo -n -e $red">>>NOT A VALID INVARIVANT..."
	if [ $u -eq 1 ]; then
		echo -e $bold"Reason: Property I (precondition ==> invariant) FAILED. stop here..."$normal
	elif [ $u -eq 2 ]; then
		echo -e $bold"Reason: Property II (invariant && loopcondition =S=> invariant) FAILED. stop here..."$normal 
	elif [ $u -eq 3 ]; then
		echo -e $bold"Reason: Property III (invariant && ~loopcondition ==> postcondition) FAILED. stop here..."$normal
	fi
	exit $ret
fi
#echo -e $blue"[PASS]"$normal
cd $dir_project
return 0
}


#**********************************************************************************************
# verification phase
#**********************************************************************************************
##########################################################################
# From inv files to prepare for verification step
##########################################################################
echo -n -e $blue"Invariant file is located at "$path_inv" >>> "$normal
cat $path_inv
echo ""


##########################################################################
# Generating a new config file contains the invariant candidate...
##########################################################################
echo -n -e $blue"Generating a new config file contains the invariant candidate..."$normal
path_tmp_cfg=$dir_temp"/tmp.cfg"
cp $path_cfg $path_tmp_cfg
echo "" >> $path_tmp_cfg
echo -n "invariant=" >> $path_tmp_cfg
cat $path_inv >> $path_tmp_cfg
echo -e $green$bold"[Done]"$normal


##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
echo -n -e $blue"Generating three C files to do the verification by KLEE..."$normal
$dir_parser"/cfg2verif" $path_tmp_cfg $path_verif
echo -e $green$bold"[Done]"$normal


##########################################################################
# File preparation for verificattion
##########################################################################
cd $dir_temp
mkdir -p $prefix"_klee1" $prefix"_klee2" $prefix"_klee3"
mv $file_c1_verif $prefix"_klee1/"$file_c_verif
mv $file_c2_verif $prefix"_klee2/"$file_c_verif
mv $file_c3_verif $prefix"_klee3/"$file_c_verif
cd $dir_project


KleeVerify 1
KleeVerify 2
KleeVerify 3

#cd ..
echo -e $bold$green"-----------------------------------------------------------finish proving---------------------------------------------------------------"$normal
echo -n -e $green"The invariant can be "$yellow
cat $path_inv
echo -e "\n"$normal
#echo -e $green".\nEND"$normal
cd $dir_pwd
exit 0
