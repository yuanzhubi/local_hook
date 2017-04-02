#!/bin/bash


cat testcase1.c
echo "X86 Basic test start:"

echo "O0 with fPIC test:"
gcc testcase1.c -fPIC -g -m32 && ./a.out
echo "O0 without fPIC test:"
gcc testcase1.c -g -m32&& ./a.out

echo "O3 with fPIC test:"
gcc testcase1.c -O3 -fPIC -g -m32 && ./a.out

echo "O3 without fPIC test:"
gcc testcase1.c -O3 -g -m32 && ./a.out 


echo "Locality test start:"
export LD_LIBRARY_PATH=./
gcc testcase2.c  -shared -o libtest2.so -fPIC -g -m32 -O3 

echo "O0 with fPIC test:"
gcc testcase1.c -fPIC -g  -m32 -L./ -ltest2 -D LOCALITY_TEST  && ./a.out
echo "O0 without fPIC test:"
gcc testcase1.c -g -m32  -L./ -ltest2 -D LOCALITY_TEST && ./a.out

echo "O3 with fPIC test:"
gcc testcase1.c -O3 -fPIC -g -m32  -L./ -ltest2 -D LOCALITY_TEST && ./a.out

echo "O3 without fPIC test:"
gcc testcase1.c -O3 -g -m32 -L./ -ltest2 -D LOCALITY_TEST && ./a.out

