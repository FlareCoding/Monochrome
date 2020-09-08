#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _MouseButtonReleasedEventObject {
	PyObject_HEAD
	MouseButtonReleasedEvent handle = MouseButtonReleasedEvent({ 0, 0 }, MouseButton::None);
} MouseButtonReleasedEventObject;

PyObject* MouseButtonReleasedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int MouseButtonReleasedEventObject_Init(MouseButtonReleasedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* MouseButtonReleasedEventObject_GetButton(MouseButtonReleasedEventObject* self, void* closure);
PyObject* MouseButtonReleasedEventObject_GetLocation(MouseButtonReleasedEventObject* self, void* closure);

static PyGetSetDef MouseButtonReleasedEventObjectGettersSetters[] = {
	{ "button", (getter)MouseButtonReleasedEventObject_GetButton, (setter)0, "button", NULL },
	{ "location", (getter)MouseButtonReleasedEventObject_GetLocation, (setter)0, "location", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject MouseButtonReleasedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"MouseButtonReleasedEvent", /*tp_name*/
	sizeof(MouseButtonReleasedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	MouseButtonReleasedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)MouseButtonReleasedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	MouseButtonReleasedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& MouseButtonReleasedEventObject_GetType();