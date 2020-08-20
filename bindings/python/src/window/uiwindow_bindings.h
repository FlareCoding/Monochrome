#pragma once
#include "../ui/uiview_bindings.h"

typedef struct _UIWindowObject {
    PyObject_HEAD
    
    Ref<UIWindow> handle = nullptr;
} UIWindowObject;

PyTypeObject& UIWindowObject_GetType();
