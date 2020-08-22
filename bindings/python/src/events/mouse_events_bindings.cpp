#include "mouse_events_bindings.h"

#pragma region Mouse Pressed Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* MouseButtonPressedEventObject_GetButton(MouseButtonPressedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.button);
}

PyObject* MouseButtonPressedEventObject_GetLocation(MouseButtonPressedEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

static PyGetSetDef MouseButtonPressedEventObjectGettersSetters[] = {
    { "button", (getter)MouseButtonPressedEventObject_GetButton, (setter)NULL, "Mouse button that was pressed.", NULL },
    { "location", (getter)MouseButtonPressedEventObject_GetLocation, (setter)NULL, "Mouse button that was pressed.", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseButtonPressedEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseButtonPressedEvent",                  /*tp_name*/
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
    MouseButtonPressedEventObjectGettersSetters,/*tp_getset*/
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

PyTypeObject& MouseButtonPressedEventObject_GetType()
{
    return MouseButtonPressedEventObject_Type;
}

#pragma endregion

#pragma region Mouse Clicked Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* MouseButtonClickedEventObject_GetButton(MouseButtonClickedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.button);
}

PyObject* MouseButtonClickedEventObject_GetLocation(MouseButtonClickedEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

static PyGetSetDef MouseButtonClickedEventObjectGettersSetters[] = {
    { "button", (getter)MouseButtonClickedEventObject_GetButton, (setter)NULL, "Mouse button that was Clicked.", NULL },
    { "location", (getter)MouseButtonClickedEventObject_GetLocation, (setter)NULL, "Mouse button that was Clicked.", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseButtonClickedEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseButtonClickedEvent",                  /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    /* methods */
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
    MouseButtonClickedEventObjectGettersSetters,/*tp_getset*/
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

PyTypeObject& MouseButtonClickedEventObject_GetType()
{
    return MouseButtonClickedEventObject_Type;
}

#pragma endregion

#pragma region Mouse Hover On Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* MouseHoverOnEventObject_GetButton(MouseHoverOnEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.button);
}

PyObject* MouseHoverOnEventObject_GetLocation(MouseHoverOnEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

PyObject* MouseHoverOnEventObject_GetDistance(MouseHoverOnEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.distance.x, self->native_event.distance.y);
}

static PyGetSetDef MouseHoverOnEventObjectGettersSetters[] = {
    { "button", (getter)MouseHoverOnEventObject_GetButton, (setter)NULL, "Mouse button that was Clicked.", NULL },
    { "location", (getter)MouseHoverOnEventObject_GetLocation, (setter)NULL, ".", NULL },
    { "distance", (getter)MouseHoverOnEventObject_GetDistance, (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseHoverOnEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseHoverOnEvent",                        /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    /* methods */
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
    MouseHoverOnEventObjectGettersSetters,/*tp_getset*/
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

PyTypeObject& MouseHoverOnEventObject_GetType()
{
    return MouseHoverOnEventObject_Type;
}

#pragma endregion

#pragma region Mouse Hover Off Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* MouseHoverOffEventObject_GetButton(MouseHoverOffEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.button);
}

PyObject* MouseHoverOffEventObject_GetLocation(MouseHoverOffEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

PyObject* MouseHoverOffEventObject_GetDistance(MouseHoverOffEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.distance.x, self->native_event.distance.y);
}

static PyGetSetDef MouseHoverOffEventObjectGettersSetters[] = {
    { "button", (getter)MouseHoverOffEventObject_GetButton, (setter)NULL, "Mouse button that was Clicked.", NULL },
    { "location", (getter)MouseHoverOffEventObject_GetLocation, (setter)NULL, ".", NULL },
    { "distance", (getter)MouseHoverOffEventObject_GetDistance, (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseHoverOffEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseHoverOffEvent",                       /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    /* methods */
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
    MouseHoverOffEventObjectGettersSetters,/*tp_getset*/
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

PyTypeObject& MouseHoverOffEventObject_GetType()
{
    return MouseHoverOffEventObject_Type;
}

#pragma endregion

#pragma region Mouse Moved Event

// ====================== GETTERS/SETTERS ====================== //

PyObject* MouseMovedEventObject_GetButton(MouseMovedEventObject* self, void* closure)
{
    return Py_BuildValue("i", (int)self->native_event.button);
}

PyObject* MouseMovedEventObject_GetLocation(MouseMovedEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

PyObject* MouseMovedEventObject_GetDistance(MouseMovedEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.distance.x, self->native_event.distance.y);
}

static PyGetSetDef MouseMovedEventObjectGettersSetters[] = {
    { "button", (getter)MouseMovedEventObject_GetButton, (setter)NULL, "Mouse button that was Clicked.", NULL },
    { "location", (getter)MouseMovedEventObject_GetLocation, (setter)NULL, ".", NULL },
    { "distance", (getter)MouseMovedEventObject_GetDistance, (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseMovedEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseMovedEvent",                  /*tp_name*/
    0,                                      /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    /* methods */
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
    MouseMovedEventObjectGettersSetters,        /*tp_getset*/
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

PyTypeObject& MouseMovedEventObject_GetType()
{
    return MouseMovedEventObject_Type;
}

#pragma endregion

#pragma region Mouse Scrolled Event

// ====================== GETTERS/SETTERS ====================== //
PyObject* MouseScrolledEventObject_GetLocation(MouseScrolledEventObject* self, void* closure)
{
    return Py_BuildValue("(f, f)", self->native_event.location.x, self->native_event.location.y);
}

PyObject* MouseScrolledEventObject_GetDistance(MouseScrolledEventObject* self, void* closure)
{
    return Py_BuildValue("f", self->native_event.ScrollDistance);
}

static PyGetSetDef MouseScrolledEventObjectGettersSetters[] = {
    { "location", (getter)MouseScrolledEventObject_GetLocation, (setter)NULL, ".", NULL },
    { "distance", (getter)MouseScrolledEventObject_GetDistance, (setter)NULL, ".", NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject MouseScrolledEventObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "MouseScrolledEvent",                       /*tp_name*/
    0,                                          /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    /* methods */
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
    MouseScrolledEventObjectGettersSetters,/*tp_getset*/
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

PyTypeObject& MouseScrolledEventObject_GetType()
{
    return MouseScrolledEventObject_Type;
}

#pragma endregion
