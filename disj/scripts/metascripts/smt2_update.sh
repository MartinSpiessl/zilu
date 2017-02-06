#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

Nv=0
vars=()
values=()

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

function model2values() {
echo "---> function model2values "$1
modelfile_model2values=$1
i=0
u=0
while read var
do
	read value
	while [ $i -lt $Nv ]
	do
		if [[ ${vars[$i]}"x" == $var"x" ]]
		then
			values[$i]=$value
			#echo "value[$i]=$value"
			u=$(($u+1))
		fi
		i=$((i+1))
	done
done <$modelfile_model2values

if [ $u -lt $Nv ] ### some vars are unbouned...
then
	u=0
	while [ $u -lt $Nv ]
	do
		grep -q "${vars[$u]}" $modelfile_model2values
		if [ $? -ne 0 ]
		then
			#values[$u]=$(($(date +%s) % 4))
			values[$u]=$(((RANDOM % 100) - 50))
			#echo ${var[$u]}" is not bounded..., so randomly pick the value <${value[$u]}>"
		fi
		u=$(($u+1))
	done
fi
			
echo "<--- function model2values "$1" --> "${values[*]}
}




function updatesmt2file() {
#echo "---> function updatesmt2file"$1
smt2file_updatesmt2file=$1

str="(assert "
i=0
while [ $i -lt $(($Nv-1)) ]; do
	str=$str"(or "
	i=$(($i+1))
done

i=0
while [ $i -lt $Nv ]
do
	str=$str"(not (= "${vars[$i]}" "${values[$i]}"))"
	if [ $i -ge 1 ]; then
		str=$str")"
	else
		str=$str" "
	fi
	i=$(($i+1))
done

str=$str")"
#echo $str 
#echo ${array[@]}

str=$str"\n(check-sat)"
sed -i "s:(check-sat):$str:" $smt2file_updatesmt2file
#echo "<--- function updatesmt2file"$1
return 0
}


#function main() {
#echo $#
if [ $# -lt 2 ]
then
	echo "missing parameters..."
	echo "[USAGE] smt.sh smt2file varfile"
	exit 2
fi
smt2file=$1
varfile=$2

var2vars $varfile
model2values tmp.model
echo ${values[*]}
updatesmt2file $smt2file
#end_main}

