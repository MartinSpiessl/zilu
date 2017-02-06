#!/bin/bash
file=$1
#remove more space
sed -i 's/\ \ /\ /g' $file
sed -i 's/(\ /(/g' $file
sed -i 's/\ )/)/g' $file
#sed -i 's/)\ (/)(/g' $file
sed -i 's/\n\n/\n/g' $file

sed -i 's/\?//g' $file

sed -i 's/(set-logic/;(set-logic/g' $file
#sed -i '1d' $file
sed -i 's/(Array\ (_\ BitVec\ 32)\ (_\ BitVec\ 8)))/\ Int)/g' $file

# remove (_ bv12 32)
sed -i 's/(_\ bv\([0-9]*\)\ 32)/\1/g' $file

#replace bvslt/bvsle with <= operator
sed -i 's/bvslt/</g' $file
sed -i 's/bvsgt/>/g' $file
sed -i 's/bvsle/<=/g' $file
sed -i 's/bvsge/>=/g' $file

# replace bv*** with +-*/
sed -i 's/bvadd/+/g' $file
sed -i 's/bvsub/-/g' $file
sed -i 's/bvmul/*/g' $file
sed -i 's/bvdiv/\//g' $file

sed -i 's/bvsadd/+/g' $file
sed -i 's/bvssub/-/g' $file
sed -i 's/bvsmul/*/g' $file
sed -i 's/bvsdiv/\//g' $file

#sed -i 's/(bvsrem\ \([a-zA-Z0-9_]*\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $file
sed -i 's/(bvsrem\ /(mod\ /g' $file
#sed -i 's/(bvand\ \(([^()]*)\)\ \([0-9]*\))/(mod\ \1\ \2)/g' $file

# (bvxor -1 m) == (- -1 m)
sed -i 's/(bvxor\ 4294967295\ /(-\ -1\ /g' $file

sed -i 's/\([2-4][0-9]\{9\}\)/(-\ \1\ 4294967296)/g' $file

#replace concat of bitvector vector with int 
sed -i 's/(concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 3)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 2)\ (concat\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 1)\ (select\ \([a-zA-Z][a-zA-A0-9_]*\)\ 0))))/\1\ /g' $file

# remove (check-sat)
sed -i 's/(check-sat)/;(check-sat)/g' $file
# remove (get-model)
#sed -i 's/(get-model)/;(get-model)/g' $file
# remove (exit)
sed -i 's/(exit)/;(exit)/g' $file

#remove more space
sed -i 's/\ \ /\ /g' $file
sed -i 's/(\ /(/g' $file
sed -i 's/\ )/)/g' $file
#sed -i 's/)\ (/)(/g' $file
sed -i 's/\n\n/\n/g' $file

sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 1)/(mod\ \1\ 2)/g' $file
sed -i 's/(bvand\ \([a-zA-Z0-9_]*\)\ 3)/(mod\ \1\ 4)/g' $file
sed -i 's/(bvand\ \(([^()]*)\)\ 1)/(mod\ \1\ 2)/g' $file
sed -i 's/(bvand\ \(([^()]*)\)\ 3)/(mod\ \1\ 4)/g' $file

