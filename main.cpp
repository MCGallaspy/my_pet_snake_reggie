#include <iostream>

#include "virtual_machine.hpp"

using std::make_shared;

int main() {
/* Some example bytecode for a very simple func:

   def foo(a, b): 
..   return a + b 
..   
=> None
   foo.__code__
=> <code object foo at 0x7f4bdaf26390, file "python", line 1>
   dis.dis(foo)
  2           0 LOAD_FAST                0 (a)
              3 LOAD_FAST                1 (b)
              6 BINARY_ADD
              7 RETURN_VALUE

*/	
    CodeObj code;
    const uint8_t arr[] = {
        1, 0, 0, \
        1, 1, 0, \
        2, \
        3, \
    };
    vector<uint8_t> v(arr, arr + sizeof(arr) / sizeof(arr[0]));
    code.instrs = v;
    PyObjPtr a = make_shared<PyObj>(200), b = make_shared<PyObj>(7);
    code.objects = PyObjList();
    code.objects.push_back(a);
    code.objects.push_back(b);
    
    VirtualMachine vm;
    vm.run_code(code);
    
    return 0;
}
