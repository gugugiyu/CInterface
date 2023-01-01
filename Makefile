output: sample.o CInterface.o
	gcc ./sample.o ./CInterface.o -o output

sample.o: ./sample.c
	gcc -c ./sample.c

CInterface.o: ./CInterface.c ./CInterface.h 
	gcc -c ./CInterface.c
	
clean:
	rm *.o output.exe output.exe.stackdump