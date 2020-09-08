#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _WindowLostFocusEventObject {
	PyObject_HEAD
} WindowLostFocusEventObject;

PyObject* WindowLostFocusEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int WindowLostFocusEventObject_Init(WindowLostFocusEventObject* type, PyObject* args, PyObject* kwds);

static PyTypeObject WindowLostFocusEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"WindowLostFocusEvent", /*tp_name*/
	sizeof(WindowLostFocusEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)WindowLostFocusEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	WindowLostFocusEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& WindowLostFocusEventObject_GetType();