#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _KeyReleasedEventObject {
	PyObject_HEAD
	KeyReleasedEvent handle = KeyReleasedEvent(KeyCode::None);
} KeyReleasedEventObject;

PyObject* KeyReleasedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int KeyReleasedEventObject_Init(KeyReleasedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* KeyReleasedEventObject_GetKeycode(KeyReleasedEventObject* self, void* closure);

static PyGetSetDef KeyReleasedEventObjectGettersSetters[] = {
	{ "keycode", (getter)KeyReleasedEventObject_GetKeycode, (setter)0, "keycode", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject KeyReleasedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"KeyReleasedEvent", /*tp_name*/
	sizeof(KeyReleasedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	KeyReleasedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)KeyReleasedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	KeyReleasedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& KeyReleasedEventObject_GetType();