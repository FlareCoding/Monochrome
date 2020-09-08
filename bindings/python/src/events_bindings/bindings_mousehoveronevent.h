#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _MouseHoverOnEventObject {
	PyObject_HEAD
} MouseHoverOnEventObject;

PyObject* MouseHoverOnEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int MouseHoverOnEventObject_Init(MouseHoverOnEventObject* type, PyObject* args, PyObject* kwds);

static PyTypeObject MouseHoverOnEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"MouseHoverOnEvent", /*tp_name*/
	sizeof(MouseHoverOnEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)MouseHoverOnEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	MouseHoverOnEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& MouseHoverOnEventObject_GetType();