#!/bin/sh

./clean.sh
cp ../libft_malloc* .

gcc -o test0 test0.c
gcc -o test1 test1.c
gcc -o test2 test2.c
gcc -o test3 test3.c
gcc -o test4 test4.c
gcc -o test5 test5.c
gcc -o test6 test6.c -L ../ -lft_malloc
gcc -w -o test7 -Wint-to-void-pointer-cast test7.c -L ../ -lft_malloc
