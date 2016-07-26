#include <fstream>
#include <iostream>
#include <vector>

#include "virtual_machine.hpp"
#include "unmarshal.hpp"

using std::ifstream;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
    ifstream file("test_dict.cpython-35.pyc", ifstream::binary);
    
    if(!file.good()) {
        std::cout << "Bad file" << std::endl;
    }
    
    // Burn the magic number...
    char c;
    for(int i=0; i<12; ++i) {
        file.get(c);
    }

    std::vector<uint8_t> code;
    while (file.get(c)) {
        code.push_back(static_cast<uint8_t>(c));
    }
 
    std::cout << std::endl;

    unmarshal(code.data());

    return 0;
}
