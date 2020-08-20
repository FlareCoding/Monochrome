#pragma once
#include <Monochrome.h>
using namespace mc;

#include <Python.h>

typedef struct _IResponderObject {
    PyObject_HEAD
    Ref<IResponder> handle;
} IResponderObject;

PyTypeObject& IResponderObject_GetType();
