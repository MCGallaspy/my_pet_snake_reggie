#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include <iostream>
#include <memory>
#include <utility>

#include "virtual_machine.hpp"
#include "PyObj.hpp"

class Instruction {
public:
    virtual Why dispatch(const PyFrame& frame, uint16_t args, VirtualMachine* vm) {
        Why why = None;
        switch(m_type) {
            case 1: {
                vm->m_stack.push_back(frame.code.objects.at(static_cast<size_t>(args)));
                break;
            }
            case 2: {
                auto a = vm->m_stack.back()->val;
                vm->m_stack.pop_back();
                auto b = (vm->m_stack.back())->val;
                vm->m_stack.pop_back();
                vm->m_stack.push_back(std::make_shared<PyObj>(a + b));
                break;
            }
            case 3: {
                why = Return;
                std::cout << "returning " << vm->m_stack.back()->val << std::endl;
                vm->m_stack.pop_back();
                break;
            }
        }
        return why;
    }
 
    bool has_args() {
        return m_type == 1;
    }
    
    Instruction(uint8_t t) : m_type(t) {}
private:
    uint8_t m_type;
};

InstructionPtr make_instr(uint8_t byte) {
    return std::make_shared<Instruction>(byte);
}
#endif