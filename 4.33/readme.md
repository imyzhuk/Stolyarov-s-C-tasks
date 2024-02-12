If you use 64-bit operation system, you should type:
1. nasm -f elf calls.asm
2. gcc -m32 calls.o 4.33.c -o 4.33
3. ./4.33