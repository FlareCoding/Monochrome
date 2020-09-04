#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _MouseHoverOffEventObject {
	PyObject_HEAD
} MouseHoverOffEventObject;

PyObject* MouseHoverOffEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int MouseHoverOffEventObject_Init(MouseHoverOffEventObject* type, PyObject* args, PyObject* kwds);

static PyTypeObject MouseHoverOffEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"MouseHoverOffEvent", /*tp_name*/
	sizeof(MouseHoverOffEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)MouseHoverOffEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	MouseHoverOffEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& MouseHoverOffEventObject_GetType();