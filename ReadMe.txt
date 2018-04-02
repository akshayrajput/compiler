This compiler is contructed by Akshay Rajput.
It's name is cm (c minus) as this is subset of C compiler written in C, compiled using flex(lexer), bison(parser generator) and clang(C compiler).
This compiler is compiled on linux ubuntu 14.04 32 bit intel core i3 processor.
To run this we have provided some test cases in test folder, you can run this by

./cm < ./test/filename.cm

it will print the assembly code of the program, the code is in IA32 optimized.