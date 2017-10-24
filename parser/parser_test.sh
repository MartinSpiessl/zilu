#/bin/bash

if [ $? -lt 2 ]; then
	file="./test/03.cfg"
else
	file=$1
fi
##########################################################################
# Generate C files to verify using cfg file and inv file
##########################################################################
dir_parser=./build
dir_temp=./tmp
mkdir -p $dir_parser $dir_temp

cd $dir_parser
cmake ..
make | cp parser.hpp ..
make
cd ..

#echo -n -e $blue"Generating C files to get initial positive/negative value by KLEE..."$normal
cat $file | $dir_parser"/parser" -t 5 -c 1 -o $dir_temp"/last.c"
#cat $path_cfg | $dir_parser"/parser" -t 5 -c -1 -o $dir_temp"/"$prefix"_init2/"$file_c_init
#echo -n -e $green$bold"[Done]"$normal

