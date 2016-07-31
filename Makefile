all: main.exe

main.exe: unmarshal.o virtual_machine.o PyObj.o
	g++ -std=c++14 PyObj.o unmarshal.o virtual_machine.o main.cpp -o main.exe

unmarshal.o:
	g++ -std=c++14 -c PyObj.hpp unmarshal.hpp unmarshal.cpp

virtual_machine.o:
	g++ -std=c++14 -c PyObj.hpp instructions.hpp virtual_machine.hpp virtual_machine.cpp

PyObj.o:
	g++ -std=c++14 -c PyObj.hpp PyObj.cpp

clean:
	rm *.o