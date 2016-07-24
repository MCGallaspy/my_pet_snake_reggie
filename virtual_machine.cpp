#include <memory>
#include <utility>

#include "instructions.hpp"
#include "virtual_machine.hpp"

void VirtualMachine::run_code(const CodeObj& code) {
    PyFramePtr frame = make_frame(code);
    run_frame(frame);
}

PyFramePtr VirtualMachine::make_frame(const CodeObj& code) {
    return std::make_shared<PyFrame>(code, nullptr, PyObjList(), vector<string>(), vector<BlockPtr>(), 0);
}

size_t VirtualMachine::run_frame(PyFramePtr frame) {
    m_frames.push_back(frame);
    while (true) {
        InstructionPtr instr = get_instruction();
        uint16_t arg = get_arg(instr);
        Why why = instr->dispatch(*m_frames.back(), arg, this);
        
        while (why != None && !frame->block_stack.empty()) {
            why = manage_block_stack(why);
        }
        
        if (why!=None) break;
    }
    m_frames.pop_back();
    return m_return_value;
}

Why VirtualMachine::manage_block_stack(Why why) {
    PyFramePtr frame = m_frames.back();
    BlockPtr block = frame->block_stack.back();
    if (block->type == Loop && why == Continue) {
        jump(m_return_value);
        return None;
    }
    
    size_t offset = block->type == ExceptionHandler ? 3 : 0;
    while (frame->stack.size() > block->handler + offset) {
        frame->stack.pop_back();
    }
    
    if (block->type == ExceptionHandler) {
        // Set exception stuff
    }
    
    if (block->type == Loop and why == Break) {
        jump(block->handler);
        return None;
    }
    
    return why;
}

InstructionPtr VirtualMachine::get_instruction() {
    PyFramePtr frame = m_frames.back();
    size_t offset = frame->last_instr_ind;
    InstructionPtr instr = make_instr(frame->code.instrs.at(offset));
    frame->last_instr_ind += 1;
    return instr;
}

uint16_t VirtualMachine::get_arg(InstructionPtr instr) {
    uint16_t arg{0};
    if (instr->has_args()) {
        PyFramePtr frame = m_frames.back();
        size_t offset = frame->last_instr_ind;
        arg += static_cast<uint16_t>(frame->code.instrs.at(offset)) + \
            static_cast<uint16_t>(frame->code.instrs.at(offset+1))*256;
        frame->last_instr_ind += 2;
    }
    return arg;
}

void VirtualMachine::jump(size_t inst_ind) {
    PyFramePtr frame = m_frames.back();
    frame->last_instr_ind = inst_ind;
}