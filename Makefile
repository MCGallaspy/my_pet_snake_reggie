GPP_CMD = g++ -Wall -Werror -fno-rtti -std=c++14

all: main.exe

main.exe: unmarshal.o virtual_machine.o PyObj.o
	$(GPP_CMD) PyObj.o unmarshal.o virtual_machine.o main.cpp -o main.exe

unmarshal.o: unmarshal.hpp unmarshal.cpp
	$(GPP_CMD) -c PyObj.hpp unmarshal.hpp unmarshal.cpp

virtual_machine.o: PyObj.hpp instructions.hpp virtual_machine.hpp virtual_machine.cpp
	$(GPP_CMD) -c PyObj.hpp instructions.hpp virtual_machine.hpp virtual_machine.cpp

PyObj.o: PyObj.hpp PyObj.cpp
	$(GPP_CMD) -c PyObj.hpp PyObj.cpp

clean:
	rm *.o