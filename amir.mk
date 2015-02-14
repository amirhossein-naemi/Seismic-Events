      
all: amir.exe

amir.exe: amir.o
	 g++ -o amir.exe amir.o

amir.o: amir.cpp
	 g++ -c amir.cpp

clean:
	rm *.o
