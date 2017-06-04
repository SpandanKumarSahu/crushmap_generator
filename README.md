# Using Makefile
`make cmg` : To compile and run the `crush_map_generator`
`make examples`: To compile the crushmaps in examples/ folder
`make generate`: To compile the crushmaps in crushmaps/ folder
Other commands can be found in the Makefile.

# One Step Solution
`./generate.sh`

# Generating examples
`./generate_examples.sh`
	
# Building the generator:
`g++ std=c++11 crush_map_generator.cpp -o cmg`

# Compiling the crush-map:
`crushtool -c <file_to_compile> -o <destination/name_of_compiled_file>`

# To Run :
`./cmg`

# 
* Some test cases have already been included in examples folder.
* The crushmaps of each run are stored in the crushmaps/ folder.
* The compiled crushmaps in examples/ will be under examples_bin/ folder.
* The compiled version of the generated crushmaps are under crushmaps_bin/ folder.
