#include <iostream>
#include <memory>
#include <string>

#include <stdio.h>

#include "PyObj.hpp"
#include "unmarshal.hpp"

PyObjPtr unmarshal(const byte contents[]) {
    size_t offset = -1;
    return _unmarshal(contents, offset);
}

PyObjPtr _unmarshal(const byte contents[], size_t& offset) {
    byte type = contents[++offset] & ~FLAG_REF;
    bool is_short = false;
    switch(type) {
        case TYPE_CODE:
            std::cout << "Unmarshaling code..." << std::endl;
            return unmarshal_code(contents, offset);
            break;
        case TYPE_TUPLE:
        case TYPE_SMALL_TUPLE:
            std::cout << "Got tuple type " << type << std::endl;
            return _unmarshal_tuple(contents, --offset); // backup offset to read type again
            break;
        case TYPE_NONE:
            std::cout << "Got None type " << type << std::endl;
            return NoneType::NonePtr;
            break;
        case TYPE_SHORT_ASCII:
        case TYPE_SHORT_ASCII_INTERNED:
            is_short = true;
        case TYPE_STR:
            std::cout << "Got str type " << type << std::endl;
            return _unmarshal_str(contents, offset, is_short);
            break;
        case TYPE_INT: {
            std::cout << "Got int type " << type << std::endl;
            auto tmp = read_int32(contents, offset);
            return std::make_shared<Integer>(tmp);
            break;
        }
        case TYPE_REF: {
            auto tmp = std::make_shared<PyObj>(read_int32(contents, offset));
            std::cout << "Got ref type " << type << " to " << tmp->get_val() << std::endl;
            return tmp;
            break;
        }   
        case TYPE_DICT: {
            std::cout << "Got dict type " << type << std::endl;
            return _unmarshal_dict(contents, offset);
            break;
        }
        case TYPE_NULL:
            return nullptr;
            break;
        case TYPE_TRUE:
            std::cout << "Got True!" << std::endl;
            return nullptr;
            break;
        case TYPE_FALSE:
            std::cout << "Got False!" << std::endl;
            return nullptr;
            break;
        case TYPE_BINARY_FLOAT:
            std::cout << "Got binary float: " << _unmarshal_nstr(contents, offset, 8)->str << std::endl;
            return nullptr;
            break;
        }
    
    std::cout << "Got unknown type " << type << std::endl;
    std::cout << "Type is zero? " << (type == 0 ? "yes" : "no") << std::endl;
    std::cout << "Offset is " << offset << std::endl;
    exit(0);
}

PyObjPtr _unmarshal_nstr(const byte contents[], size_t& offset, size_t n) {
    std::string str;
    for (size_t i=0; i<n; ++i) {
        str.push_back(static_cast<char>(contents[++offset]));
    }
    PyObjPtr retval = std::make_shared<PyObj>();
    retval->str = str;
    std::cout << "Reading string of size " << n << " it was " << str << std::endl;
    return retval;
}

PyObjPtr _unmarshal_dict(const byte contents[], size_t& offset) {
    while (true) {
        PyObjPtr key = _unmarshal(contents, offset);
        if (key == nullptr) break;
        std::cout << "Got key " << key->str << std::endl;
    
        PyObjPtr value = _unmarshal(contents, offset);
        if (value == nullptr) break;
        std::cout << "Got value " << value->str << std::endl;
    }
    return nullptr;
}

int32_t read_int32(const byte contents[], size_t& offset) {
    uint32_t val = 0;
    uint32_t base = 1;
    for (int i=0; i<4; ++i) {
        uint8_t tmp = contents[++offset];
        val += static_cast<int32_t>(tmp) * base;
        base *= 256;
    }
    return val;
}

PyObjPtr _unmarshal_str(const byte contents[], size_t& offset, bool is_short) {
    size_t len;
    if (!is_short) {
        std::cout << "Wide string... ";
        len = read_int32(contents, offset);
    } else {
        std::cout << "Short string... ";
        len = static_cast<size_t>(contents[++offset]);
    }
    std::string str;
    for (size_t i=0; i<len; ++i) {
        str.push_back(static_cast<char>(contents[++offset]));
    }
    PyObjPtr retval = std::make_shared<PyObj>();
    retval->str = str;
    std::cout << "Reading string of size " << len << " it was " << str << std::endl;
    return retval;
}

PyObjPtr _unmarshal_tuple(const byte contents[], size_t& offset) {
    byte type = contents[++offset] & ~FLAG_REF;
    size_t n = 0;
    if (type == TYPE_SMALL_TUPLE) {
        n = static_cast<size_t>(contents[++offset]);
    } else if (type == TYPE_TUPLE) {
        uint32_t tmp = read_int32(contents, offset);
        n = static_cast<size_t>(tmp);
    } else {
        std::cout << "Got unknown type " << type << std::endl;
        std::cout << "Offset " << offset << std::endl;
        uint32_t tmp = read_int32(contents, offset);
        n = static_cast<size_t>(tmp);
    }
        // Placeholder code...
    char buf[255];
    sprintf(buf, "Made a tuple of size %llu", n);
    std::cout << buf << std::endl;
        
    PyObjPtr retval = std::make_shared<PyObj>();
    retval->str = std::string(buf);

    for (size_t i=0; i<n; ++i) {
        _unmarshal(contents, offset);
    };
   
    return retval;
}

PyObjPtr unmarshal_code(const byte contents[], size_t& offset) {
    // size_t offset = 1; // 0th byte is the type code.
    
    uint32_t argc = read_int32(contents, offset);
    
    uint32_t kw_argc = read_int32(contents, offset);

    uint32_t nlocals = read_int32(contents, offset);

    uint32_t stack_size = read_int32(contents, offset);
    
    uint32_t flags = read_int32(contents, offset);
     
    std::cout << "Unmarshalling the code bytestring..." << std::endl;
    PyObjPtr code = _unmarshal(contents, offset);
    
    PyObjPtr consts = _unmarshal(contents, offset);
    PyObjPtr names = _unmarshal(contents, offset);
    PyObjPtr varnames = _unmarshal(contents, offset);
    PyObjPtr freevars = _unmarshal(contents, offset);
    PyObjPtr cellvars = _unmarshal(contents, offset);
    
    std::string filename = _unmarshal(contents, offset)->str;

    std::string name = _unmarshal(contents, offset)->str;

    uint32_t first_line_no = read_int32(contents, offset);
    
    PyObjPtr lnotab = _unmarshal(contents, offset);

    std::cout << "filename: " << filename << std::endl
                 << "name: " << name << std::endl
                 << "consts: " << consts->str << std::endl
                 << "names: " << names->str << std::endl
                 << "varnames: " << varnames->str << std::endl
                 << "freevars: " << freevars->str << std::endl
                 << "cellvars: " << cellvars->str << std::endl
                 << argc << kw_argc << nlocals << stack_size << flags << first_line_no << std::endl
                 << lnotab->str << std::endl;
           
    return std::make_shared<PyObj>();
}
