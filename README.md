# simple-compiler
A compiler for a simple language.
# Documentation
There are 4 different commands:
1. Var: this creates a variable with the name of the next argument. Example: Var x;
2. Set: this sets variable arg1 to the value of arg2. Example: Set x 3+4;
3. Input: this takes a variable as an argument, and sets the variable to command line input from the user. Example: Input x;
4. Print: this prints a value. Example: Print 3/(x-1);

All lines must end in a semi-colon, and spaces don't matter.

# How to Use
You write the code to the code.txt file. Then to run, you do "./build.sh", and then "./main". You may need to do "chmod +x build.sh" if it says permission denied.
To compile the c code, run gcc main.c -o main.
