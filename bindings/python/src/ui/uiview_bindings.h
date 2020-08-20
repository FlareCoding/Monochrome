#pragma once
#include "iresponder_bindings.h"

typedef struct _LayerObject {
    PyObject_HEAD
    Ref<Layer> handle = nullptr;
} LayerObject;

PyTypeObject& LayerObject_GetType();

typedef struct _UIViewObject {
    IResponderObject _responder_base;
    Ref<UIView> handle;
    LayerObject* layer;
} UIViewObject;

PyTypeObject& UIViewObject_GetType();
