#ifndef PY_OBJ
#define PY_OBJ

#include <memory>
#include <string>
#include <vector>

class PyObj;
typedef std::shared_ptr<PyObj> PyObjPtr;
typedef std::vector<PyObjPtr> PyObjList;

struct PyObj {
    int val;
    std::string str;
    
    PyObj(PyObj&& orig) {
        this->val = orig.val;
        this->str = orig.str;
    }
    
    PyObj() {}
    
    PyObj(int n) : val(n) {}
};

#endif