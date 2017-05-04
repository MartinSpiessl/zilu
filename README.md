## ZILU Installation
ZILU has been tested on Ubuntu 14.04 x64. It should work on other Linux platfroms, but we have not tested yet.

#### pre-requirement (most of the package can be installed with apt-get on Ubuntu)
- apt-get install git cmake flex bison clang
- [git](https://git-scm.com/downloads)
- [cmake](https://cmake.org/) version 2.8 or later
- [flex](https://github.com/westes/flex) version 2.6.1 or later
- [bison](https://www.gnu.org/software/bison/) version 2.6.0 or later
- [clang](http://clang.llvm.org/get_started.html)
- "make" and otherLLVM essential building tools, you can add if needed
- [z3](https://github.com/Z3Prover/z3) is applied to check the smt-constraints generated by KLEE.
	- get z3:
		git clone https://github.com/Z3Prover/z3.git
	- build z3
```
	cd z3
	python contrib/cmake/bootstrap.py create
	mkdir build
	cd build
	cmake -G "Unix Makefiles" ../
	make -j4 # Replace 4 with an appropriate number
	sudo make install
```
- [GSL](http://www.gnu.org/software/gsl/) This library is used to solve equations in our project. 
	- apt-get install libgsl0-dev
- [KLEE](https://klee.github.io/) only test llvm2.9 yet, so try to build KLEE by [build-llvm29](http://klee.github.io/build-llvm29/). 
	- If you can not build KLEE with the above instruction, maybe you can refer to 
		- [link1](http://blog.opensecurityresearch.com/2014/07)
		- [link2](http://klee-dev.keeda.stanford.narkive.com/RqzseDmo/problems-when-compiling-klee-runtime).


#### Get the latest ZILU
```
git clone https://github.com/lijiaying/ZILU.git
cd ZILU
```

#### Patch KLEE source code
This modification aims at generating smt2 file for each path condition.
We mainly add new method calls **``klee_fail && klee_pass''**, as we would like to output the path condition to files in these method calls.

- klee.tar.gz is the patch. It should be applied to KLEE with commit signature 6609a03e68bf551f433ddd0fd8cf64a8683ee2ee, the short SHA: 6609a03.
- Change to Klee with commit 6609a03. 
	- git checkout 6609a03
- Extract the gz file to replace the same files in KLEE project.
	- tar -xvzf klee.tar.gz
- After the patch, you can continue to proceed build KLEE. 
	- cp -r klee/\* ../klee-project-home/
- Rebuild and install the patched KLEE. 
	- make ENABLE_OPTIMIZED=1
	- sudo make install

#### Test ZILU
```
./run.sh 03  or  ./run.sh cfg/03.cfg
```

#### Notes
- The '03' is a file '03.cfg' without extension which is located in 'cfg' folder.

## Enjoy your tour with our Invariant Inference Framework: ZILU
