#pragma once
#include "uilabel_bindings.h"

typedef struct _UIButtonObject {
    UIViewObject _base;
    UILabelObject* Label;
} UIButtonObject;

PyTypeObject& UIButtonObject_GetType();
