#!/bin/bash

rm -rf bin > /dev/null
mkdir bin
rm -rf crushmaps_bin > /dev/null
mkdir crushmaps_bin
rm -rf crushmaps > /dev/null 
mkdir crushmaps
g++ -std=c++11 crush_map_generator.cpp -o bin/cmg
./bin/cmg
for f in ./crushmaps/*.txt
do
    g=`basename $f`
    h="${g%.txt}"
    `crushtool -c crushmaps/"$g" -o crushmaps_bin/"$h".bin` 
done
