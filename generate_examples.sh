#!/bin/bash

rm -rf examples_bin > /dev/null
mkdir examples_bin
for f in ./examples/*.txt
do
    g=`basename $f`
    h="${g%.txt}"
    `crushtool -c examples/"$g" -o examples_bin/"$h".bin` 
done
