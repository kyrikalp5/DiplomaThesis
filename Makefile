a.out:  main.o gray_2_binary.o ibr.o
	gcc main.c gray_2_binary.c ibr.c -lm

clean: 
	rm *.o a.out