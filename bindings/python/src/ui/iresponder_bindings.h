#pragma once
#include "../events/mouse_events_bindings.h"
#include "../events/keyboard_events_bindings.h"

typedef struct _IResponderObject {
    PyObject_HEAD
    Ref<IResponder> handle;
} IResponderObject;

PyTypeObject& IResponderObject_GetType();
