#ifndef PY_OBJ
#define PY_OBJ
/*
Python 3.5 has a standard type hierarchy: https://docs.python.org/3.5/reference/datamodel.html#the-standard-type-hierarchy
We implement it here in object-oriented style.
*/
#include <memory>
#include <string>
#include <vector>

#include <stdint.h>

class PyObj;
typedef std::shared_ptr<PyObj> PyObjPtr;
typedef std::vector<PyObjPtr> PyObjList;

class PyObj {
public:
    virtual ~PyObj() = default;
    virtual int32_t get_val() { return val; }
    std::string str;
    
    explicit PyObj(int32_t _int) : val(_int) {}
    PyObj()  = default;

private:
    int32_t val;
};

class NoneType : public PyObj {
public:
    NoneType() = default;
    ~NoneType() = default;
    NoneType(const NoneType&) = delete;

    static typename std::shared_ptr<NoneType> NonePtr;
};  

class Number : public PyObj {};

/*
Note that Python specifies its integers must be arbitrary-precision.
We limit the precision here for now for simplicity.
*/
class Integer : public Number {
private:
    int32_t m_int;
public:
    explicit Integer(int32_t _int) : m_int(_int) {}
    Integer() = delete;
    
    int32_t get_int() { return m_int; }
};


class Float : public Number {
private:
    double m_val;
public:
    explicit Float(double v) : m_val(v) {}
    Float() = delete;

    double get_float() { return m_val; }
};


typedef std::shared_ptr<Float> FloatPtr;


#endif