target : main.o

main.o : CamV4L2.cpp CamV4L2.h main.c
	gcc main.c CamV4L2.cpp -I /usr/src/linux-headers-4.15.0-156-generic/arch/x86/include -I /usr/src/linux-headers-4.15.0-156-generic/include -o main.o

clean : 
	rm *.o