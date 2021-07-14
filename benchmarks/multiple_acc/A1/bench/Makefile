.PHONY: build A1 clean

build: A1

A1: $(FILES)
	clang -emit-llvm -c A1.c -o A1.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll A1.o -o A1.bc
	llvm-dis A1.bc -o A1.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
