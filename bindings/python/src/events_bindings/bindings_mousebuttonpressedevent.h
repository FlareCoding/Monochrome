#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _MouseButtonPressedEventObject {
	PyObject_HEAD
	MouseButtonPressedEvent handle = MouseButtonPressedEvent({ 0, 0 }, MouseButton::None);
} MouseButtonPressedEventObject;

PyObject* MouseButtonPressedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int MouseButtonPressedEventObject_Init(MouseButtonPressedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* MouseButtonPressedEventObject_GetButton(MouseButtonPressedEventObject* self, void* closure);
PyObject* MouseButtonPressedEventObject_GetLocation(MouseButtonPressedEventObject* self, void* closure);

static PyGetSetDef MouseButtonPressedEventObjectGettersSetters[] = {
	{ "button", (getter)MouseButtonPressedEventObject_GetButton, (setter)0, "button", NULL },
	{ "location", (getter)MouseButtonPressedEventObject_GetLocation, (setter)0, "location", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject MouseButtonPressedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"MouseButtonPressedEvent", /*tp_name*/
	sizeof(MouseButtonPressedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	MouseButtonPressedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)MouseButtonPressedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	MouseButtonPressedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& MouseButtonPressedEventObject_GetType();
