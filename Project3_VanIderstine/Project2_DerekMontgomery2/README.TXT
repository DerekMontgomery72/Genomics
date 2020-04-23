CPTS 471 Project 2
Derek Montgomery
Student ID: 11508236

This Application Was Created using Visual Studio Community 2019.
It can be built by opening the .sln file under the Visual studio folder
run through there.

Alternatively it should be capable of being built with g++ on linux simply by
building all the included .cpp and header files, and running the executable.

It has been built to both accomodate no input file in the comand arguments and
in the format <executable> <FASTA file> <Alphabet>

It will create the Suffix tree, then run DFS and print out the 
start character of each branch. Then will give the longest repeating string
found by taking the longest link from root to an internal node.

It will output a DFS.txt file and a BWT.txt file containing the results of the two functions 
respectively.

