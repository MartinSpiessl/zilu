#!/bin/bash
dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_project=`dirname $dir_project`
. $dir_project"/scripts/include.sh"

#function main() {
#echo $#
dir_org=`pwd`
if [ $# -lt 3 ]; then
	echo "missing parameters..."
	echo "[USAGE] smt.sh smt2file varfile #solutions"
	exit 2
fi
#cd $dir_project
#smt2file=$1
tmp_smt2file="tmp.smt2"
tmp_modelfile="tmp.model"

cp $1 $tmp_smt2file
varfile=$2
num=$3

func_varfile2vars $varfile
func_bv2int $tmp_smt2file

time=0
while [ $time -lt $num ]
do
	#echo "loop time="$time".........."
	func_z3solve $tmp_smt2file $tmp_modelfile
	if [ $? -ne 0 ]; then
		#echo "Z3 can only get "$time" solutions...."
		exit 1
	fi
	func_modelfile2values $tmp_modelfile
	#echo ${values[*]}
	func_updatesmt2file $tmp_smt2file
	time=$(($time+1))
done
rm -f $tmp_modelfile
rm -f $tmp_smt2file
#cd $dir_org
exit 0
#end_main}
