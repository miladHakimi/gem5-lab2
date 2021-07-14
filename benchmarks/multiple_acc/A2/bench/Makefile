.PHONY: build A2 clean

build: A2

A2: $(FILES) 
	clang -emit-llvm -c A2.c -o A2.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll A2.o -o A2.bc
	llvm-dis A2.bc -o A2.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
