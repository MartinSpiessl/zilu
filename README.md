## ZILU Installation
#### pre-requirement
* [git](https://git-scm.com/downloads)
* [cmake](https://cmake.org/) version 2.8 or later
* [clang](http://clang.llvm.org/get_started.html)
* "make" and otherLLVM essential building tools, you can add if needed
* [z3](https://github.com/Z3Prover/z3) the installation folder should be "/usr" or "/usr/local", otherwise you should modify $Z3_ROOT in cmake.base in the project directory so cmake can find it. 
* [GSL](http://www.gnu.org/software/gsl/) This library is used to solve equations in our project. (ubuntu: apt-get install libgsl0-dev)
* [KLEE](https://klee.github.io/) only test llvm2.9 yet, so try to build KLEE by [build-llvm29](http://klee.github.io/build-llvm29/)


#### patch KLEE source code
This modification aims at generating smt2 file for each path condition.
The principle is to add a new method call **``klee_fail && klee_pass''**, and in its method handler we output the path condition to files.

* klee.tar.gz is the patch files. It should be applied to KLEE with commit signature 6609a03e68bf551f433ddd0fd8cf64a8683ee2ee
+ Extract the gz file to replace the same files in KLEE project.
+ After the patch, you can continue to proceed build KLEE.


#### Get ZILU
```
git clone git@github.com:lijiaying/ZILU.git
```

#### Test ZILU
```
cd ZILU
mkdir build
./run.sh 05
```

#### Notes
+ The '05' is a file located in 'cfg' folder without extension.

## Enjoy your tour with our Invariant Inference Framework: ZILU
