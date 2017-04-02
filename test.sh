#!/bin/bash


cat testcase1.c
echo "Basic test start:"

echo "O0 with fPIC test:"
gcc testcase1.c -fPIC -g && ./a.out
echo "O0 without fPIC test:"
gcc testcase1.c -g && ./a.out

echo "O3 with fPIC test:"
gcc testcase1.c -O3 -fPIC -g && ./a.out

echo "O3 without fPIC test:"
gcc testcase1.c -O3 -g && ./a.out 


echo "Locality test start:"
export LD_LIBRARY_PATH=./
gcc testcase2.c  -shared -o libtest2.so -fPIC -g  -O3 

echo "O0 with fPIC test:"
gcc testcase1.c -fPIC -g -L./ -ltest2 -D LOCALITY_TEST  && ./a.out
echo "O0 without fPIC test:"
gcc testcase1.c -g  -L./ -ltest2 -D LOCALITY_TEST && ./a.out

echo "O3 with fPIC test:"
gcc testcase1.c -O3 -fPIC -g  -L./ -ltest2 -D LOCALITY_TEST && ./a.out

echo "O3 without fPIC test:"
gcc testcase1.c -O3 -g -L./ -ltest2 -D LOCALITY_TEST && ./a.out

