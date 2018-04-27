output: jall.o
	gcc jall.o -o jall

jall.o: jall.c
	gcc -c jall.c

clean: 
	rm *.o jall