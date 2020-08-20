#pragma once
#include "uiview_bindings.h"

typedef struct _UILabelObject {
    UIViewObject _base;
    PyObject* Text;
} UILabelObject;

PyTypeObject& UILabelObject_GetType();
