#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

#define MODULE_REGISTER_CLASS(mod, name, ptype) if (PyType_Ready(ptype) < 0) { \
                                                    Py_XDECREF(mod); \
                                                    return -1; } \
                                                PyModule_AddObject(mod, name, (PyObject*)ptype);

namespace core
{   
}
