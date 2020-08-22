#pragma once
#include <Monochrome.h>
using namespace mc;

#include <Python.h>

typedef struct _KeyPressedEventObject {
    PyObject_HEAD
    KeyPressedEvent native_event = KeyPressedEvent(KeyCode::None, false, false, false);
} KeyPressedEventObject;

PyTypeObject& KeyPressedEventObject_GetType();

typedef struct _KeyReleasedEventObject{
    PyObject_HEAD
    KeyReleasedEvent native_event = KeyReleasedEvent(KeyCode::None);
} KeyReleasedEventObject;

PyTypeObject& KeyReleasedEventObject_GetType();
