#ifndef PY_OBJ
#define PY_OBJ
/*
Python 3.5 has a standard type hierarchy: https://docs.python.org/3.5/reference/datamodel.html#the-standard-type-hierarchy
We implement it here in object-oriented style.
*/
#include <memory>
#include <string>
#include <vector>

class PyObj;
typedef std::shared_ptr<PyObj> PyObjPtr;
typedef std::vector<PyObjPtr> PyObjList;

class PyObj {
public:
    virtual ~PyObj() = default;
    
    int val;
    std::string str;
};

class NoneType : public PyObj {
public:
    NoneType() = default;
    ~NoneType() = default;
    NoneType(const NoneType&) = delete;

    static typename std::shared_ptr<NoneType> NonePtr;
};  

#endif