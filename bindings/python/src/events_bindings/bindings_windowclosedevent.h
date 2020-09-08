#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _WindowClosedEventObject {
	PyObject_HEAD
} WindowClosedEventObject;

PyObject* WindowClosedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int WindowClosedEventObject_Init(WindowClosedEventObject* type, PyObject* args, PyObject* kwds);

static PyTypeObject WindowClosedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"WindowClosedEvent", /*tp_name*/
	sizeof(WindowClosedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)WindowClosedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	WindowClosedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& WindowClosedEventObject_GetType();