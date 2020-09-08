#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _WindowGainedFocusEventObject {
	PyObject_HEAD
} WindowGainedFocusEventObject;

PyObject* WindowGainedFocusEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int WindowGainedFocusEventObject_Init(WindowGainedFocusEventObject* type, PyObject* args, PyObject* kwds);

static PyTypeObject WindowGainedFocusEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"WindowGainedFocusEvent", /*tp_name*/
	sizeof(WindowGainedFocusEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)WindowGainedFocusEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	WindowGainedFocusEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& WindowGainedFocusEventObject_GetType();