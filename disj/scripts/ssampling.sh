#!/bin/bash
dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_project=`dirname $dir_project`
. $dir_project"/scripts/include.sh"

function func_findSmtForZ3(){
smtname="kleeAssert0000";
smt2_num=999
i=1
while [ $i -lt $smt2_num ]; do
	if [ $i -ge 100 ]; then
		smtname="kleeAssert00";
	elif [ $i -ge 10 ]; then
		smtname="kleeAssert000";
	fi
	if [ ! -f $smtname""$i".smt2" ]; then
		break
	fi
	i=$(($i+1))
done
smt2_num=$(($i-1))
if [ $smt2_num -eq 0 ]; then
	exit 0
fi
average=`expr $solution_num / $smt2_num`
#echo -e $yellow"[solution_num, average, smt2_num]=["$solution_num", "$average","$smt2_num"]"$normal
#average=`expr $solution_num / $n`
#echo -e $yellow"[solution_num, average, n]=["$solution_num", "$average","$n"]"$normal

i=1
while [ $i -le $smt2_num ]; do
	if [ $i -ge 100 ]; then
		smtname="kleeAssert00";
	elif [ $i -ge 10 ]; then
		smtname="kleeAssert000";
	fi
	if [ ! -f $smtname""$i".smt2" ]; then
		#echo -e $red"No such file. --> "$smtname$i".smt2"$normal
		return 0
	fi
	smt2file=$smtname""$i".smt2"
	#echo "  |-- processing "$smt2file" ---> ["$average"]"
	"$dir_script/smt2.sh" $smt2file $dir_var $average >> $prefix"_b"$branch".cnt"
	res=$?
	if [ $res -gt 1 ]; then
		echo -e $red$bold"A Error Occurs during smt2"$normal
		exit $res 
	fi
	i=$(($i+1))
done
return 0
}



#function main() {
#handle program arguments
dir_org=`pwd`
if [ $# -lt 3 ]; then
	echo -e $red"needs more arguments."
	echo "<USAGE> ssampling prefix|cfgfile invariant_candidate_file num_solutions"
	echo -e $normal"invariant_candidate_file format: branch_no invariant_candidate"
	echo -e "\te.g. -----> 0	3*x+4*y>=40"$red"Tab between"$normal
	exit 1
fi

prefix=`basename -s .cfg $1`
dir_candidate=$2
solution_num=$3

dir_var="$dir_temp/$prefix.var"


#read -r branch candidate < tail -1 $dir_candidate
IFS=$'\t'
read -r branch candidate < $dir_candidate
#echo "^^^^^^"$branch"----"$candidate"^^^^^^"


############################################in tmp folder###########################################
cd $dir_temp
rm -rf klee-*
rm -rf *.smt2
rm -rf $prefix"_b"$branch".cnt"
echo -e "#ifdef\tCandidate"$branch >> candidate.h
echo -e "#undef\tCandidate"$branch >> candidate.h
echo -e "#endif" >> candidate.h
echo -e "#define Candidate"$branch"\t("$candidate")" >> candidate.h


branchfile_cpp=$prefix"_b"$branch".cpp" 
branchfile_o=$prefix"_b"$branch".o" 
if [ ! -f $branchfile_cpp ]; then 
	#echo $dir_parser
	cat "$dir_cfg/$prefix.cfg" | "$dir_parser/parser" -t 3 -o $branchfile_cpp -b $branch
fi

#echo -e $green"Compiling the C files and Run KLEE..."$normal
#llvm-gcc --emit-llvm -c -g $branchfile_cpp > /dev/null
clang -emit-llvm -c -g $branchfile_cpp -o $branchfile_o > /dev/null
#echo -e $blue"Running KLEE to generate path condition --->"$branchfile_o$normal
klee $branchfile_o > /dev/null 2>&1
res=$?
func_findSmtForZ3
if [ $? -ne 0 ]; then
	echo "Error occurs during func_findSmtForZ3"
	exit $res
fi

#res=`wc -l $prefix"_b"$branch".cnt" | awk '{print $1}'`
#echo "---> samples#"$res
#cat $prefix"_b"$branch".cnt"

#echo -e $yellow"end of ssampling"$normal
cd $dir_org
echo -e -n $normal
exit 0
