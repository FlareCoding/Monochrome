#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _MouseMovedEventObject {
	PyObject_HEAD
	MouseMovedEvent handle = MouseMovedEvent({ 0, 0 }, { 0, 0 }, MouseButton::None);
} MouseMovedEventObject;

PyObject* MouseMovedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int MouseMovedEventObject_Init(MouseMovedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* MouseMovedEventObject_GetButton(MouseMovedEventObject* self, void* closure);
PyObject* MouseMovedEventObject_GetLocation(MouseMovedEventObject* self, void* closure);
PyObject* MouseMovedEventObject_GetDistance(MouseMovedEventObject* self, void* closure);

static PyGetSetDef MouseMovedEventObjectGettersSetters[] = {
	{ "button", (getter)MouseMovedEventObject_GetButton, (setter)0, "button", NULL },
	{ "location", (getter)MouseMovedEventObject_GetLocation, (setter)0, "location", NULL },
	{ "distance", (getter)MouseMovedEventObject_GetDistance, (setter)0, "distance", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject MouseMovedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"MouseMovedEvent", /*tp_name*/
	sizeof(MouseMovedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	MouseMovedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)MouseMovedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	MouseMovedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& MouseMovedEventObject_GetType();