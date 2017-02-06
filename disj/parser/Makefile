all: parser

OBJS = parser.o  \
       tokens.o	 \
	   main.o

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags`# -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf parser.cpp parser.hpp tokens.cpp $(OBJS) conj.cpp new2.cpp Nv_Nb

parser.cpp: parser.y
	bison -d -o $@ $^
	
parser.hpp: parser.cpp node.hpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

%.o: %.cpp LoopHandler.hpp 
	g++ -c $(CPPFLAGS) -o $@ $<

parser: $(OBJS) node.hpp
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

test: parser test/new2.cfg test/conj.cfg
	cat test/new2.cfg| ./parser -t 2 -c new2.ce -o new2.cpp -v new2.var -i new2.inv

	#cat test/conj.cfg | ./parser
	#cat test/new2.cfg | ./parser
	#cat test/conj.cfg| ./parser -t 1 -c conj.ce -o conj.cpp -v conj.var -i conj.inv
