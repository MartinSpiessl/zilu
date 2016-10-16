#!/bin/bash
#dir_include=/home/parallels/Research/GitHub/ZILUv3/scripts
#dir_include=`dirname $(pwd)/${0}`
dir_include=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
echo $dir_include
. $dir_include"/include.sh"

dir_org=`pwd`
if [ $# -lt 1 ]; then
	echo "./verify.sh needs more parameters"
	echo "./verify.sh cofig_prefix"
	echo "try it again..."
	exit 1
fi
cnt_num=8
if [ $# -gt 1 ]; then
	cnt_num=$2
fi

cd $dir_project
prefix=$1
mkdir -p tmp

file_cfg=$prefix".cfg"
path_cfg=$dir_cfg"/"$file_cfg
file_var=$prefix".var"
path_var=$dir_temp"/"$file_var
file_cnt=$prefix".cnt"
path_cnt=$dir_temp"/"$file_cnt
file_cnt_lib=$prefix".cntlib"
path_cnt_lib=$dir_temp"/"$file_cnt_lib

file_c_init=$prefix"_init.c"
file_o_init=$prefix"_init.o"
file_c1_init=$prefix"_init1.c"
file_c2_init=$prefix"_init2.c"


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
				#echo ${values[*]}
				echo ${values[*]} >> $path_cnt
				cnt_num=$(($cnt_num-1))
			else
				break
			fi
		done


		echo -n -e $red$bold" [sat] [FAIL]"$normal
		echo -e " >>> init sample(s) are stored at "$yellow$path_cnt$normal
		cat $path_cnt
		return 1
	fi

	#echo -e -n $red$bold"z3solve exits with non-zero value"$normal
	echo -e $green$bold" [unsat] [PASS]"$normal
	u=$(($u+1))
done
return 0
}

function func_kleeinit(){
#u=$1
cd $dir_temp"/"$prefix"_init" 
rm -rf klee-*
rm -rf *.smt2
echo -e $green"Compiling the C files and Run KLEE..."$u$normal
llvm-gcc --emit-llvm -c -g $file_c_init > /dev/null
echo -e $blue"Running KLEE to generate path condition"$normal
klee $file_o_init > /dev/null 2>&1
#klee -write-smt2s $file_o_verif #> /dev/null 2>&1
func_findsmt4z3
ret=$?
#echo -n -e $red$ret$normal
cd ..
return 0
}

##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
echo -n -e $blue"Generating C files to get initial positive/negative value by KLEE..."$normal
$dir_parser"/cfg2init" $path_cfg  $dir_temp"/"$file_c_init
#cat $path_cfg | $dir_parser"/parser" -t 5 -c 1 -o $dir_temp"/"$file_c1_init
#cat $path_cfg | $dir_parser"/parser" -t 5 -c -1 -o $dir_temp"/"$file_c2_init
echo -n -e $green$bold"[Done]"$normal


##########################################################################
# File preparation for verificattion
##########################################################################
cd $dir_temp
mkdir -p $prefix"_init" 
mv $file_c_init $prefix"_init/"$file_c_init
cd $dir_project
#mkdir -p $prefix"_init1" $prefix"_init2" 
#mv $file_c1_init $prefix"_init1/"$file_c_init
#mv $file_c2_init $prefix"_init2/"$file_c_init

func_kleeinit
#func_kleeinit 2

echo -e -n $yellow"INIT SAMPLES: "
cat $path_cnt
echo -e $normal

cd $dir_project
cd $dir_org
exit 0
