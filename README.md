Dictionary
===========

A C++ implementation of a dictionary which supports the following operation - 
```
* Dynamic insertion of words
* Queries -
    1. Prefix search and count
    2. Suffix search and count
    3. Substring search and count
```

Design Details
--------------

<<<<<<< HEAD
The basic structure used is to implement the dictionary is a *generalized suffix tree*. It stores each of the word in 
the dictionary in the form of a tree such that each suffix has a root to node path.  

Each word is appended with a '$' and prepended with a '#' followed by the line number on which it appears.
For example, the words apple, mango, orange will be inserted into the suffix tree as $apple#1, $mango#2 and $orange#3. 
=======
The basic structure used is to implement the dictionary is a *generalized suffix tree*. It stores each of the word in   
the dictionary in the form of a tree such that each suffix has a root to node path.  

Each word is appended with a '$' and prepended with a '#' followed by the line number on which it appears.  
For example, the words apple, mango, orange will be inserted into the suffix tree as $apple#1, $mango#2 and $orange#3.  
>>>>>>> origin/master
A suffix tree supports substring search in general and for other operations, these special characters are used.  
* '$' helps in prefix query. A prefix query for string S is a substring query of $S  
* '#' helps in suffix query. A suffix query for string S is a substring query of S#  
* The number in the end helps create a distinct node for each duplicate suffix in the word  

### Algorithm
<<<<<<< HEAD
Suffix tree construction is performed using the [Ukkonen's algorithm](https://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf)
for online suffix tree creation. Each suffix of every word is represented as a node in this tree. Each node (internal and leaf)
additionally stores the words (*indices of words in actual implementation*) all the nodes in its subtree are part of.  

A substring query is performed by tracing the path from the root along with the edges corresponding to the string. 
On reaching the final node along this path, the result is the set of words on this node. Prefix and suffix queries can
be reduced to substring search and mentioned above.  

### Complexity
The suffix tree construction is linear in the total size of the words inserted. If *N* words are to be inserted in the tree
and the average length of each word is *W*, the suffix tree creation has complexity *O(N.W)*.   

The substring query of a string *s* of length *S* has complexity *O(S)*. Prefix and suffix queries also have the same
=======
Suffix tree construction is performed using the [Ukkonen's algorithm](https://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf)   
for online suffix tree creation. Each suffix of every word is represented as a node in this tree. Each node (internal and leaf)   
additionally stores the words (*indices of words in actual implementation*) all the nodes in its subtree are part of.  

A substring query is performed by tracing the path from the root along with the edges corresponding to the string.   
On reaching the final node along this path, the result is the set of words on this node. Prefix and suffix queries can  
be reduced to substring search and mentioned above.  

### Complexity
The suffix tree construction is linear in the total size of the words inserted. If *N* words are to be inserted in the tree  
and the average length of each word is *W*, the suffix tree creation has complexity *O(N.W)*.   

The substring query of a string *s* of length *S* has complexity *O(S)*. Prefix and suffix queries also have the same  
>>>>>>> origin/master
complexity.  

### Other features
* Google tests for basic operations like insertion and queries
* Benchmarking for performance

Build and Run
-------------

### Requirements
1. C++17 compiler
2. CMake
3. Google Test
4. Google Benchmark

### Building

*Configure CMake*  
```bash
$ cmake -H<path to dictionary directory> -B<path to the build folder to be generated>
```

*Build all the binaries*   
```bash
$ cmake --build <path to the build folder generated above>
```

For example, if you are currently in the dictionary directory  
```bash
$ cmake -H. -Bbuild  
$ cmake --build build
```
### Binaries

The three main binaries (inside the build directory) are -
1. *src/dictionary*
```bash
 Core implementation of the dictionary. It takes two command line arguments - 
 the word list file and the query file. 

 $ build/src/dictionary /path/to/wordList.txt /path/to/queryList.txt

 The output is printed to the standard console output and the errors on standard console error.
 ```

2. *tests/suffixTreeTest* 
```bash
Unit tests of the suffix tree and can be run standlone.
$ build/tests/suffixTreeTest

Also integrated with ctest so these tests can be executed by running 
$ ctest
inside the build directory
```
3. perf/suffixTreePerf - performance benchmarking
```bash
  Performance benchmarking of the suffix tree. Run standalone using 
  $ build/perf/suffixTreePerf
```


