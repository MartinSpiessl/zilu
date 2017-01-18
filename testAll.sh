#!/bin/bash
red="\e[31m"
green="\e[32m"
yellow="\e[33m"
blue="\e[34m"
normal="\e[0m"
bold="\e[1m"

if [ $# -ge 1 ]; then
	cfgfolder=$1
else
	cfgfolder="cfg"
fi

times=3
if [ $# -ge 2 ]; then
	times=$2
fi

if [ -d "./result" ]; then
	mv ./result ./result_old_`date +%m%d%H%M`
fi
mkdir -p result

from=`date +%m%d%H%M`
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for cfgfile in `ls $cfgfolder/*.cfg`
do
	cfgfile_base=`basename -s .cfg $cfgfile`
	./testOne.sh $cfgfile_base $times 0
	./testOne.sh $cfgfile_base $times 1
	./testOne.sh $cfgfile_base $times 2
done

to=`date +%m%d%H%M`

if [ -d "./result" ]; then
	mv ./result "./result_"$from"-"$to
fi
exit 0
