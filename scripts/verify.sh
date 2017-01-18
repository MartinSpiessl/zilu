#!/bin/bash
#dir_include=/home/parallels/Research/GitHub/ZILUv3/scripts
#dir_include=`dirname $(pwd)/${0}`
dir_include=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
. $dir_include"/include.sh"

if [ $# -lt 1 ]; then
	echo "./verify.sh needs more parameters"
	echo "./verify.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi

dir_org=`pwd`
prefix=$1
cnt_num=1
if [ $# -gt 1 ]; then
	cnt_num=$2
fi

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
file_c_verif=$prefix".c"
file_c1_verif=$prefix"_klee1.c"
file_c2_verif=$prefix"_klee2.c"
file_c3_verif=$prefix"_klee3.c"
file_o_verif=$prefix".o"


function func_findsmt4z3(){
smtname="kleeAssert0000";
n=99
u=1
while [ $u -lt $n ]; do
	if [ $u -ge 10 ]; then
		smtname="kleeAssert000";
	fi
	path_smt2=$(pwd)"/"$smtname""$u".smt2"
	path_model=$(pwd)"/"$smtname""$u".model"
	if [ ! -f $path_smt2 ]; then
		#echo  "No such file."
		return 0
	fi
	echo -n "  |-- processing $smtname$u.smt2 ---> "
	func_bv2int $path_smt2 
	func_z3solve $path_smt2 $path_model
	if [ $? -eq 0 ]; then
		func_modelfile2values $path_model
		echo ${values[*]} > $path_cnt
		cnt_num=$(($cnt_num-1))
		while [ $cnt_num -gt 0 ]; do
			func_updatesmt2file $path_smt2
			func_z3solve $path_smt2 $path_model
			if [ $? -eq 0 ]; then
				func_modelfile2values $path_model
				echo ${values[*]} >> $path_cnt
				cnt_num=$(($cnt_num-1))
			else
				break
			fi
		done


		echo -n -e $red$bold" [sat] [FAIL]"$normal
		echo -e " >>> counter example(s) are  stored at "$yellow$path_cnt$normal
		return 1
	fi

	#echo -e -n $red$bold"z3solve exits with non-zero value"$normal
	echo -e $green$bold" [unsat] [PASS]"$normal
	u=$(($u+1))
done
return 0
}


function func_kleeverify(){
u=$1
cd $prefix"_klee"$u 
rm -rf klee-*
rm -rf *.smt2
echo -e $green"Compiling the C files and Run KLEE..."$u$normal
llvm-gcc --emit-llvm -c -g $file_c_verif > /dev/null
echo -e $blue"Running KLEE to generate path condition"$normal
klee $file_o_verif > /dev/null 2>&1
#klee -write-smt2s $file_o_verif #> /dev/null 2>&1
func_findsmt4z3
ret=$?
#echo -n -e $red$ret$normal
if [ $ret -eq 2 ]; then
	exit $ret
elif [ $ret -eq 1 ]; then
	echo -n -e $red">>>NOT A VALID INVARIVANT...Reason: "
	if [ $u -eq 1 ]; then
		echo -e $bold"Property I (precondition ==> invariant) FAILS. "
	elif [ $u -eq 2 ]; then
		echo -e $bold"Property II (invariant && loopcondition =S=> invariant) FAILS."
	elif [ $u -eq 3 ]; then
		echo -e $bold"Property III (invariant && ~loopcondition ==> postcondition) FAILS."
	fi
	echo -e $normal #" stop here..."
	exit $ret
fi
#echo -e $blue"[PASS]"$normal
cd ..
return 0
}


cd $dir_project
mkdir -p tmp
func_varfile2vars $path_var
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
#echo "" >> $path_tmp_cfg
echo -n "invariant=" >> $path_tmp_cfg
cat $path_inv >> $path_tmp_cfg
echo "" >> $path_tmp_cfg
echo -e $green$bold"[Done]"$normal
#cat $path_tmp_cfg


##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
echo -n -e $blue"Generating three C files to do the verification by KLEE..."$normal
$dir_parser"/cfg2verif" $path_tmp_cfg $path_verif
#cat $path_tmp_cfg | $dir_parser"/parser" -t 4 -c 1 -o $dir_temp"/"$file_c1_verif
#cat $path_tmp_cfg | $dir_parser"/parser" -t 4 -c 2 -o $dir_temp"/"$file_c2_verif
#cat $path_tmp_cfg | $dir_parser"/parser" -t 4 -c 3 -o $dir_temp"/"$file_c3_verif
#$dir_tool"/bin/cfg2verif" $path_tmp_cfg $path_verif
echo -e $green$bold"[Done]"$normal


##########################################################################
# File preparation for verificattion
##########################################################################
cd $dir_temp
mkdir -p $prefix"_klee1" $prefix"_klee2" $prefix"_klee3"
mv $file_c1_verif $prefix"_klee1/"$file_c_verif
mv $file_c2_verif $prefix"_klee2/"$file_c_verif
mv $file_c3_verif $prefix"_klee3/"$file_c_verif

func_kleeverify 1
func_kleeverify 2
func_kleeverify 3

cd $dir_project
echo -e $bold$green"-----------------------------------------------------------finish proving---------------------------------------------------------------"$normal

echo -n -e $green"The invariant can be "$yellow
cat $path_inv
echo -e $normal
#echo -e $green".\nEND"$normal
cd $dir_org
exit 0
