#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

dir_project=$(cd $(dirname $BASH_SOURCE[0]) && pwd)
dir_project=`dirname $dir_project`

dir_temp=$dir_project"/tmp"
dir_test=$dir_project"/test"
dir_script=$dir_project"/scripts"
dir_parser=$dir_project"/build/parser"
dir_cfg=$dir_project"/cfg"
dir_build=$dir_project"/build"

#echo "dir_project=="$dir_project
#echo "dir_temp=="$dir_temp
#echo "dir_tool=="$dir_tool
#echo "dir_build=="$dir_build
#echo "dir_script=="$dir_script
#echo "dir_parser=="$dir_parser


Nv=0
vars=()
values=()


function func_varfile2vars() {
#echo "---> function func_varfile2vars "$1
if [ ${#vars[*]} -ne 0 ]; then
	return 0
fi
varfile=$1
i=-1
for line in $(cat $varfile); do
	if [ $i -eq -1 ]; then
		Nv=line
	else
		vars[$i]=$line
	fi
	i=$(($i+1))
done
Nv=${#vars[*]}
#assert($Nv -eq ${#vars[*]})
#echo "<--- function func_varfile2vars"$1" --> "${vars[*]}
return 0
}


function func_modelfile2values() {
#echo "---> function model2values "$1
modelfile=$1
#cat $modelfile
nv=0
while read var; do
	read value
	i=0
	while [ $i -lt $Nv ]; do
		if [[ ${vars[$i]}"x" == $var"x" ]]; then
			values[$i]=$value
			#echo -e "$blue vars[$i]:${vars[$i]}=$value$normal"
			nv=$(($nv+1))
		fi
		i=$((i+1))
	done
done <$modelfile
### some vars are unbouned...
if [ $nv -lt $Nv ]; then
	index=0
	while [ $index -lt $Nv ]; do
		grep -q "${vars[$index]}" $modelfile
		if [ $? -ne 0 ]; then
			values[$index]=$(($(date +%s) % 32))
			#echo ${vars[$index]}" is not bounded..., so randomly pick the value <${values[$index]}>"
			#values[$index]=$(((RANDOM % 100) - 50))
		fi
		index=$(($index+1))
	done
fi

#echo ${values[*]}
#echo "<--- function model2values "$1" --> "${values[*]}
}



function func_bv2int() {
#echo "---> function func_bv2int "$1
smt2file=$1
#remove more space
sed -i 's/\ \ /\ /g' $smt2file
sed -i 's/(\ /(/g' $smt2file
sed -i 's/\ )/)/g' $smt2file
#sed -i 's/)\ (/)(/g' $smt2file
sed -i 's/\n\n/\n/g' $smt2file

sed -i 's/\?//g' $smt2file

sed -i 's/(set-logic/;(set-logic/g' $smt2file
sed -i 's/(Array\ (_\ BitVec\ 32)\ (_\ BitVec\ 8)))/\ Int)/g' $smt2file

# remove (_ bv12 32)
sed -i 's/(_\ bv\([0-9]*\)\ 32)/\1/g' $smt2file

#replace bvslt/bvsle with <= operator
sed -i 's/bvslt/</g' $smt2file
sed -i 's/bvsgt/>/g' $smt2file
sed -i 's/bvsle/<=/g' $smt2file
sed -i 's/bvsge/>=/g' $smt2file

# replace bv*** with +-*/
sed -i 's/bvadd/+/g' $smt2file
sed -i 's/bvsub/-/g' $smt2file
sed -i 's/bvmul/*/g' $smt2file
sed -i 's/bvdiv/\//g' $smt2file

sed -i 's/bvsadd/+/g' $smt2file
sed -i 's/bvssub/-/g' $smt2file
sed -i 's/bvsmul/*/g' $smt2file
sed -i 's/bvsdiv/\//g' $smt2file

#sed -i 's/(bvsrem\ \([a-zA-Z0-9_]*\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $smt2file
sed -i 's/(bvsrem\ /(mod\ /g' $smt2file
#sed -i 's/(bvand\ \(([^()]*)\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $smt2file

# (bvxor -1 m) == (- -1 m)
sed -i 's/(bvxor\ 4294967295\ /(-\ -1\ /g' $smt2file

sed -i 's/\([2-4][0-9]\{9\}\)/(-\ \1\ 4294967296)/g' $smt2file

#replace concat of bitvector vector with int 
sed -i 's/(concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 3)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 2)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 1)\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 0))))/\1\ /g' $smt2file

# ----------------------------------------------------------------------------------
# remove (check-sat)
#sed -i 's/(check-sat)/;(check-sat)/g' $smt2file
# ----------------------------------------------------------------------------------

#remove more space
sed -i 's/\ \ /\ /g' $smt2file
sed -i 's/(\ /(/g' $smt2file
sed -i 's/\ )/)/g' $smt2file
#sed -i 's/)\ (/)(/g' $smt2file
sed -i 's/\n\n/\n/g' $smt2file

sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 1)/(mod\ \1\ 2)/g' $smt2file
sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 3)/(mod\ \1\ 4)/g' $smt2file
sed -i 's/(bvand\ \(([^()]*)\)\ 1)/(mod\ \1\ 2)/g' $smt2file
sed -i 's/(bvand\ \(([^()]*)\)\ 3)/(mod\ \1\ 4)/g' $smt2file

#echo ${vars[*]}
#echo "########################################################################################"
#cat $smt2file
i=0
while [ $i -lt $Nv ]
do
	var=${vars[$i]}
	#stmt="(declare-fun\ "$var"\ ()\ Int)"
	stmt="(declare-fun $var () Int)"
	#echo $var"-------------"$stmt
	#grep -q "(declare-fun )"$var" () Int" $smt2file
	grep -q "$stmt" $smt2file
	if [ $? -ne 0 ]; then
		#echo -e $yellow"!!$stmt: NotExists, add++"$normal
		sed -i "1 a$stmt" $smt2file
		#else
		#echo -e $green"!!$stmt: Exists, pass"$normal
	fi
	i=$(($i+1))
done
#echo "########################################################################################"

# remove (get-model)
grep -q "(get-model)" $smt2file
if [ $? -ne 0 ]; then
	#echo "(get-model) ===> NotExists"
	sed -i '/(exit)/i\(get-model)' $smt2file
fi

#echo "<--- function func_bv2int "$1
return 0
}



function func_z3solve() {
#echo "---> function solve "$1" "$2" @pwd="`pwd`
smt2file=$1
modelfile=$2

z3 $smt2file > $modelfile
#echo "solve result-------------------------------"
#cat $modelfile
#echo "-------------------------------------------"
res=$?
if [ $res -ne 0 ]; then
	#echo "z3 --> $res"
	#echo "unsat"
	return $res
	#exit $res
	#grep -n "unsat" $modelfile | cut -d ':' -f 1 > result
	#if [[ $result == "1" ]]; then
	#	exit 1
	#else
	#	exit 2  #### other errors....
	#fi
fi

sed -i '1,2d' $modelfile
sed -i 's/\ \ /\ /g' $modelfile
sed -i 's/(define-fun\ \([a-zA-Z_][a-zA-Z_0-9]*\)\ ()\ Int/\1/g' $modelfile
sed -i 's/()//g' $modelfile
sed -i 's/Int//g' $modelfile
sed -i 's/(-\ \([1-9][0-9]*\))/-\1/g' $modelfile
sed -i 's/)//g' $modelfile
sed -i 's/\ \ /\ /g' $modelfile
#echo "<--- function solve "$1" "$2" sat"
#cat $modelfile
return 0
}


function func_updatesmt2file() {
#should be called after array values is SET
#echo "---> function func_updatesmt2file"$1
smt2file=$1

str="(assert "
i=0
while [ $i -lt $(($Nv-1)) ]; do
	str=$str"(or "
	i=$(($i+1))
done

i=0
while [ $i -lt $Nv ]; do
	if [[ "${vars[$i]}x" == "x" ]]; then
		echo "vars[$i] is empty"
		exit -1
	fi
	if [[ "${values[$i]}x" == "x" ]]; then
		echo "values[$i] is empty"
		exit -1
	fi

	str="$str(not (= ${vars[$i]} ${values[$i]}))"
	if [ $i -ge 1 ]; then
		str="$str)"
	else
		str="$str "
	fi
	i=$(($i+1))
done

str=$str")"
#str=$str"\n(check-sat)"
#sed -i "s:(check-sat):$str:" $smt2file
sed -i "/(check-sat)/i$str" $smt2file
#echo "<--- function func_updatesmt2file"$1
return 0
}
