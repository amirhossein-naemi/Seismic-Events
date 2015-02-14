rem gcc -o hellomake hellomake.c hellofunc.c -I.
rem The -I. is included so that gcc will look in the current directory (.) for the include file hellomake.h.
rem gcc -o amir.exe amir.cpp
rem g++ -o amir.exe amir.cpp
rem make amir.mk
make -f amir.mk
make -f amir.mk clean
rem The -I. is included so that gcc will look in the current directory (.) for the include file hellomake.h.
pause