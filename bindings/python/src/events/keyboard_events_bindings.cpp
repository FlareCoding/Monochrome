#include "keyboard_events_bindings.h"

#pragma region Key Pressed Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* KeyPressedEventObject_GetKeyCode(KeyPressedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.keycode);
}

PyObject* KeyPressedEventObject_GetKey(KeyPressedEventObject* self, void* closure)
{
    return Py_BuildValue("s", std::string(1, McKeycodeToChar(self->native_event.keycode, self->native_event.capital, self->native_event.capslock_on)).c_str());
}

PyObject* KeyPressedEventObject_GetRepeated(KeyPressedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.repeated);
}

PyObject* KeyPressedEventObject_GetCapital(KeyPressedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.capital);
}

PyObject* KeyPressedEventObject_GetCapsLockOn(KeyPressedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.capslock_on);
}

static PyGetSetDef KeyPressedEventObjectGettersSetters[] = {
    { "keycode",    (getter)KeyPressedEventObject_GetKeyCode,       (setter)NULL, ".", NULL },
    { "key",        (getter)KeyPressedEventObject_GetKey,           (setter)NULL, ".", NULL },
    { "repeated",   (getter)KeyPressedEventObject_GetRepeated,      (setter)NULL, ".", NULL },
    { "capital",    (getter)KeyPressedEventObject_GetCapital,       (setter)NULL, ".", NULL },
    { "capslock",   (getter)KeyPressedEventObject_GetCapsLockOn,    (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject KeyPressedEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "KeyPressedEvent",                  /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    0,                                          /*tp_dealloc*/
    0,                                          /*tp_vectorcall_offset*/
    0,                                          /*tp_getattr*/
    0,                                          /*tp_setattr*/
    0,                                          /*tp_as_async*/
    0,                                          /*tp_repr*/
    0,                                          /*tp_as_number*/
    0,                                          /*tp_as_sequence*/
    0,                                          /*tp_as_mapping*/
    0,                                          /*tp_hash*/
    0,                                          /*tp_call*/
    0,                                          /*tp_str*/
    0,                                          /*tp_getattro*/
    0,                                          /*tp_setattro*/
    0,                                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /*tp_flags*/
    0,                                          /*tp_doc*/
    0,                                          /*tp_traverse*/
    0,                                          /*tp_clear*/
    0,                                          /*tp_richcompare*/
    0,                                          /*tp_weaklistoffset*/
    0,                                          /*tp_iter*/
    0,                                          /*tp_iternext*/
    0,                                          /*tp_methods*/
    0,                                          /*tp_members*/
    KeyPressedEventObjectGettersSetters,/*tp_getset*/
    0,                                          /*tp_base*/
    0,                                          /*tp_dict*/
    0,                                          /*tp_descr_get*/
    0,                                          /*tp_descr_set*/
    0,                                          /*tp_dictoffset*/
    0,                                          /*tp_init*/
    0,                                          /*tp_alloc*/
    0,                                          /*tp_new*/
    0,                                          /*tp_free*/
    0,                                          /*tp_is_gc*/
};

PyTypeObject& KeyPressedEventObject_GetType()
{
    return KeyPressedEventObject_Type;
}

#pragma endregion

#pragma region Key Released Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* KeyReleasedEventObject_GetKeyCode(KeyReleasedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.keycode);
}

PyObject* KeyReleasedEventObject_GetKey(KeyReleasedEventObject* self, void* closure)
{
    return Py_BuildValue("s", std::string(1, McKeycodeToChar(self->native_event.keycode, 0, 0)).c_str());
}

static PyGetSetDef KeyReleasedEventObjectGettersSetters[] = {
    { "keycode",    (getter)KeyReleasedEventObject_GetKeyCode,       (setter)NULL, ".", NULL },
    { "key",        (getter)KeyReleasedEventObject_GetKey,           (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject KeyReleasedEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "KeyReleasedEvent",                          /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    0,                                          /*tp_dealloc*/
    0,                                          /*tp_vectorcall_offset*/
    0,                                          /*tp_getattr*/
    0,                                          /*tp_setattr*/
    0,                                          /*tp_as_async*/
    0,                                          /*tp_repr*/
    0,                                          /*tp_as_number*/
    0,                                          /*tp_as_sequence*/
    0,                                          /*tp_as_mapping*/
    0,                                          /*tp_hash*/
    0,                                          /*tp_call*/
    0,                                          /*tp_str*/
    0,                                          /*tp_getattro*/
    0,                                          /*tp_setattro*/
    0,                                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /*tp_flags*/
    0,                                          /*tp_doc*/
    0,                                          /*tp_traverse*/
    0,                                          /*tp_clear*/
    0,                                          /*tp_richcompare*/
    0,                                          /*tp_weaklistoffset*/
    0,                                          /*tp_iter*/
    0,                                          /*tp_iternext*/
    0,                                          /*tp_methods*/
    0,                                          /*tp_members*/
    KeyReleasedEventObjectGettersSetters,/*tp_getset*/
    0,                                          /*tp_base*/
    0,                                          /*tp_dict*/
    0,                                          /*tp_descr_get*/
    0,                                          /*tp_descr_set*/
    0,                                          /*tp_dictoffset*/
    0,                                          /*tp_init*/
    0,                                          /*tp_alloc*/
    0,                                          /*tp_new*/
    0,                                          /*tp_free*/
    0,                                          /*tp_is_gc*/
};

PyTypeObject& KeyReleasedEventObject_GetType()
{
    return KeyReleasedEventObject_Type;
}

#pragma endregion
