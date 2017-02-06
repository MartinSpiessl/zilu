#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"



function solve() {
#echo "---> function solve "$1" "$2" @pwd="`pwd`
smt2file_solve=$1
modelfile_solve=$2
tmpfile_solve="z3tmp"
#echo " before >> z3"
z3 $smt2file_solve > $tmpfile_solve
res=$?
#echo " after << z3"
#echo "solve result-------------------------------"
#cat $tmpfile_solve
#echo "-------------------------------------------"
if [ $res -ne 0 ]; then
	if [ $res -eq 1 ]; then
		echo "unsat" > $modelfile_solve
	fi
	#echo "<--- function solve "$1" "$2" unsat"
	exit $res
fi

sed '1,2d' $tmpfile_solve > $modelfile_solve
rm $tmpfile_solve
sed -i 's/\ \ /\ /g' $modelfile_solve
sed -i 's/(define-fun\ \([a-zA-Z_][a-zA-Z_0-9]*\)\ ()\ Int/\1/g' $modelfile_solve
sed -i 's/()//g' $modelfile_solve
sed -i 's/Int//g' $modelfile_solve
sed -i 's/(-\ \([1-9][0-9]*\))/-\1/g' $modelfile_solve
sed -i 's/)//g' $modelfile_solve
sed -i 's/\ \ /\ /g' $modelfile_solve
#echo "<--- function solve "$1" "$2" sat"
#cat $modelfile_solve
return 0
}



#function main() {
#echo $#
if [ $# -lt 2 ]
then
	echo "missing parameters..."
	echo "[USAGE] smt_solver.sh smt2file modelfile"
	exit 2
fi
smt2file=$1
modelfile=$2

var2vars $varfile
bv2int $smt2file

#echo "loop time="$time".........."
solve $smt2file $modelfile
if [ $? -ne 0 ]
then
	#echo "Z3 can only get "$time" solutions...."
	exit 1
fi
exit 0
#end_main}

