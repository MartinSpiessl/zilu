#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

Nv=0
vars=()

function var2vars() {
echo "---> function var2vars "$1
varfile_var2vars=$1
#read Nv , line < $varfile_var2vars
#read line < $varfile_var2vars
i=-1
for line in $(cat $varfile_var2vars)
do
	#echo $i">"$line
	if [ $i -eq -1 ]
	then
		Nv=line
	else
		vars[$i]=$line
	fi
	i=$(($i+1))
done
Nv=${#vars[*]}
#assert($Nv -eq ${#vars[*]})
echo "<--- function var2vars"$1" --> "${vars[*]}
return 0
}


function bv2int() {
#echo "---> function bv2int "$1
smt2file_bv2int=$1
#remove more space
sed -i 's/\ \ /\ /g' $smt2file_bv2int
sed -i 's/(\ /(/g' $smt2file_bv2int
sed -i 's/\ )/)/g' $smt2file_bv2int
#sed -i 's/)\ (/)(/g' $smt2file_bv2int
sed -i 's/\n\n/\n/g' $smt2file_bv2int

sed -i 's/\?//g' $smt2file_bv2int

sed -i 's/(set-logic/;(set-logic/g' $smt2file_bv2int
sed -i 's/(Array\ (_\ BitVec\ 32)\ (_\ BitVec\ 8)))/\ Int)/g' $smt2file_bv2int

# remove (_ bv12 32)
sed -i 's/(_\ bv\([0-9]*\)\ 32)/\1/g' $smt2file_bv2int

#replace bvslt/bvsle with <= operator
sed -i 's/bvslt/</g' $smt2file_bv2int
sed -i 's/bvsgt/>/g' $smt2file_bv2int
sed -i 's/bvsle/<=/g' $smt2file_bv2int
sed -i 's/bvsge/>=/g' $smt2file_bv2int

# replace bv*** with +-*/
sed -i 's/bvadd/+/g' $smt2file_bv2int
sed -i 's/bvsub/-/g' $smt2file_bv2int
sed -i 's/bvmul/*/g' $smt2file_bv2int
sed -i 's/bvdiv/\//g' $smt2file_bv2int

sed -i 's/bvsadd/+/g' $smt2file_bv2int
sed -i 's/bvssub/-/g' $smt2file_bv2int
sed -i 's/bvsmul/*/g' $smt2file_bv2int
sed -i 's/bvsdiv/\//g' $smt2file_bv2int

#sed -i 's/(bvsrem\ \([a-zA-Z0-9_]*\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $smt2file_bv2int
sed -i 's/(bvsrem\ /(mod\ /g' $smt2file_bv2int
#sed -i 's/(bvand\ \(([^()]*)\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $smt2file_bv2int

# (bvxor -1 m) == (- -1 m)
sed -i 's/(bvxor\ 4294967295\ /(-\ -1\ /g' $smt2file_bv2int

sed -i 's/\([2-4][0-9]\{9\}\)/(-\ \1\ 4294967296)/g' $smt2file_bv2int

#replace concat of bitvector vector with int 
sed -i 's/(concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 3)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 2)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 1)\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 0))))/\1\ /g' $smt2file_bv2int

#echo ${vars[*]}
i=0
while [ $i -lt $Nv ]
do
	var=${vars[$i]}
	#stmt="(declare-fun\ "$var"\ ()\ Int)"
	stmt="(declare-fun $var () Int)"
	#echo $var"-------------"$stmt
	#grep -q "(declare-fun )"$var" () Int" $smt2file_bv2int
	grep -q "$stmt" $smt2file_bv2int
	if [ $? -ne 0 ]
	then
		#echo $stmt" ===> NotExists"
		sed -i "1 a$stmt" $smt2file_bv2int
	#else
	#	echo $stmt" ===> Exists"
	fi
	i=$(($i+1))
done

# remove (get-model)
grep -q "(get-model)" $smt2file_bv2int
if [ $? -ne 0 ]; then
	#echo "(get-model) ===> NotExists"
	sed -i '/(exit)/i\(get-model)' $smt2file_bv2int
fi
# remove (exit)
#sed -i 's/(exit)/;(exit)/g' $smt2file_bv2int

#remove more space
sed -i 's/\ \ /\ /g' $smt2file_bv2int
sed -i 's/(\ /(/g' $smt2file_bv2int
sed -i 's/\ )/)/g' $smt2file_bv2int
sed -i 's/\n\n/\n/g' $smt2file_bv2int

sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 1)/(mod\ \1\ 2)/g' $smt2file_bv2int
sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 3)/(mod\ \1\ 4)/g' $smt2file_bv2int
sed -i 's/(bvand\ \(([^()]*)\)\ 1)/(mod\ \1\ 2)/g' $smt2file_bv2int
sed -i 's/(bvand\ \(([^()]*)\)\ 3)/(mod\ \1\ 4)/g' $smt2file_bv2int
#echo "<--- function bv2int "$1
return 0
}


#function main() {
#echo $#
if [ $# -lt 2 ]
then
	echo "missing parameters..."
	echo "[USAGE] smt2file_bv2int.sh smt2file varfile"
	exit 2
fi
smt2file=$1
varfile=$2

var2vars $varfile
bv2int $smt2file
exit 0
#end_main}

