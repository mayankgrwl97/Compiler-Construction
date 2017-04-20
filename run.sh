#!/bin/bash
gcc *.c
./a.out testcase1.txt o.txt > temp1.asm
nasm -f elf64 -F dwarf -g temp1.asm 
gcc temp1.o
./a.out