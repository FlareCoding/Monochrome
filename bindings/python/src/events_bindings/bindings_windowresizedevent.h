#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _WindowResizedEventObject {
	PyObject_HEAD
	WindowResizedEvent handle = WindowResizedEvent(nullptr, 0, 0);
} WindowResizedEventObject;

PyObject* WindowResizedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int WindowResizedEventObject_Init(WindowResizedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* WindowResizedEventObject_GetWidth(WindowResizedEventObject* self, void* closure);
PyObject* WindowResizedEventObject_GetHeight(WindowResizedEventObject* self, void* closure);

static PyGetSetDef WindowResizedEventObjectGettersSetters[] = {
	{ "width", (getter)WindowResizedEventObject_GetWidth, (setter)0, "width", NULL },
	{ "height", (getter)WindowResizedEventObject_GetHeight, (setter)0, "height", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject WindowResizedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"WindowResizedEvent", /*tp_name*/
	sizeof(WindowResizedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	WindowResizedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)WindowResizedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	WindowResizedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& WindowResizedEventObject_GetType();