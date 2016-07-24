#ifndef PY_OBJ
#define PY_OBJ
#include <memory>
#include <vector>

class PyObj;
typedef std::shared_ptr<PyObj> PyObjPtr;
typedef std::vector<PyObjPtr> PyObjList;

struct PyObj {
    int val;
    PyObj(int v) : val(v) {}
};

#endif