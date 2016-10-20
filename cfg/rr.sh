#!/bin/bash
i=0
#for file in `find cfg/ -maxdepth 1 -name '*.cfg'`
for file in `find . -maxdepth 1 -name '*.cfg'`
do
	i=$(($i+1))
	cfgfile=`basename -s .cfg $file`
	echo "\multicolumn{1}{|c|}{$cfgfile}		&conjuctive			&1 2 3 &4 5 6 &\textbf{7.89}			&1 2 3 &4 5 6   &7.89		&\multicolumn{1}{|c|}{\cmark} \\\\"
done
exit 0

