all: parser

OBJS = parser.o  \
       tokens.o	 \
	   main.o

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags`# -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

clean:
	-$(RM) parser.cpp parser.hpp tokens.cpp $(OBJS)

parser.cpp: parser.y
	bison -d -o $@ $^
	
parser.hpp: parser.cpp node.hpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

%.o: %.cpp LoopHandler.hpp 
	g++ -c $(CPPFLAGS) -o $@ $<

parser: $(OBJS) node.hpp
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

03.cpp: test/03.cfg parser
	cat test/03.cfg | ./parser -t 1 -o test/03.cpp -i test/03 -v test/03.var -d test/03.ds -x test/03.cnt
