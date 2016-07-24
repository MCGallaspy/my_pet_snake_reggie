/*
The Python virtual machine is a stack machine.
See https://github.com/python/cpython/blob/3.5/Python/ceval.c#L797 for more
info on how CPython evaluates Python bytecode.
*/
#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

#include <memory>
#include <string>
#include <vector>

#include <stdint.h>

#include "PyObj.hpp"

class Instruction;
typedef std::shared_ptr<Instruction> InstructionPtr;

template<typename T>
using vector = typename std::vector<T>;

typedef std::string string;

enum BlockType {
    Loop,
    ExceptionHandler
};

struct Block {
    BlockType type;
    size_t handler;
};
typedef std::shared_ptr<Block> BlockPtr;

struct CodeObj {
    vector<uint8_t> instrs;
    vector<PyObjPtr> objects;
    vector<string> names;
};

struct PyFrame;
typedef  std::shared_ptr<PyFrame> PyFramePtr;
struct PyFrame {
    const CodeObj& code;
    PyFramePtr previous;
    PyObjList stack;
    vector<string> names;
    vector<BlockPtr> block_stack;
    size_t last_instr_ind;
    PyFrame(const CodeObj& code_, PyFramePtr previous_, PyObjList stack_, vector<string> names_, vector<BlockPtr> bs_, size_t ind_) :
        code(code_), previous(previous_), stack(stack_), names(names_), block_stack(bs_), last_instr_ind(ind_) {}
};
typedef vector<PyFramePtr> PyFrameList;

enum Why {
    None,
    Continue,
    Break,
    Exception,
    Return
};

class VirtualMachine {

friend class Instruction;

private:    
    PyFramePtr make_frame(const CodeObj& code);
    size_t run_frame(PyFramePtr frame);
    InstructionPtr get_instruction();
    uint16_t get_arg(InstructionPtr instr);
    Why manage_block_stack(Why);
    void jump(size_t);
    
    // Members having to do with execution state
    PyFrameList m_frames;
    size_t m_return_value;
    PyObjList m_stack;

public:
    void run_code(const CodeObj& code);
    VirtualMachine() {
        m_stack = PyObjList();
    };
};

#endif