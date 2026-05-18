
CC  = g++ 
CFLAGS =  -std=c++17 -O3 -march=native -ffast-math -funroll-loops -ftree-vectorize -fomit-frame-pointer 
TIMINGLIBS = -L./ -llbstime 
CLIBS = -lm
OBJS = cputime.o walltime.o 
PROF = 
TESTDIR = files
TARGET = ntt

.PHONY: all clean test test-small test-medium test-large test-xxxlarge test-xxxxlarge test-xxlarge
all: $(TARGET)


cputime.o : cputime.cc   
	$(CC) $(CFLAGS) -c cputime.cc  

walltime.o : walltime.cc   
	$(CC) $(CFLAGS) -c walltime.cc  



ntt.o : ntt.cpp
	$(CC) $(CFLAGS) -c ntt.cpp

ntt : ntt.o lib
	$(CC) $(CFLAGS) -o ntt ntt.o $(TIMINGLIBS) $(CLIBS)


test: ntt test-tiny test-small test-medium test-large test-xxlarge test-xxxlarge test-xxxxlarge 

# Individual tests — params file contains: n q omega
test-tiny: $(TARGET)
	@echo "=== TINY (n=4) ==="
	@read n q omega < $(TESTDIR)/tiny_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/tiny_f.txt $(TESTDIR)/tiny_g.txt $(TESTDIR)/tiny_expected.txt
 
test-small: $(TARGET)
	@echo "=== SMALL (n=16) ==="
	@read n q omega < $(TESTDIR)/small_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/small_f.txt $(TESTDIR)/small_g.txt $(TESTDIR)/small_expected.txt
 
test-medium: $(TARGET)
	@echo "=== MEDIUM (n=256) ==="
	@read n q omega < $(TESTDIR)/medium_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/medium_f.txt $(TESTDIR)/medium_g.txt $(TESTDIR)/medium_expected.txt
 
test-large: $(TARGET)
	@echo "=== LARGE (n=4096) ==="
	@read n q omega < $(TESTDIR)/large_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/large_f.txt $(TESTDIR)/large_g.txt $(TESTDIR)/large_expected.txt


test-xxlarge: $(TARGET)
	@echo "=== XXLARGE (n=32768) ==="
	@read n q omega < $(TESTDIR)/xxlarge_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/xxlarge_f.txt $(TESTDIR)/xxlarge_g.txt $(TESTDIR)/xxlarge_expected.txt

test-xxxlarge: $(TARGET)
	@echo "=== XXXLARGE (n=65536) ==="
	@read n q omega < $(TESTDIR)/xxxlarge_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/xxxlarge_f.txt $(TESTDIR)/xxxlarge_g.txt $(TESTDIR)/xxxlarge_expected.txt

test-xxxxlarge: $(TARGET)
	@echo "=== XXXXLARGE (n=131072) ==="
	@read n q omega < $(TESTDIR)/xxxxlarge_params.txt && \
	./$(TARGET) $$q $$omega $(TESTDIR)/xxxxlarge_f.txt $(TESTDIR)/xxxxlarge_g.txt $(TESTDIR)/xxxxlarge_expected.txt
# Default Targets for Cleaning up the Environment
clean :
	rm *.o
	rm *.a

pristine :
	rm *.o
	rm *.a
	touch *.cc *.f  
	test -f ntt && rm ntt	

ctags :
	ctags  *.cc *.f

# Target for making the library

lib: $(OBJS) 
	ar -rc liblbstime.a $(OBJS) 
	ranlib liblbstime.a
