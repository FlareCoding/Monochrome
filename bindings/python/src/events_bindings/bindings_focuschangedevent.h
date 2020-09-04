#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _FocusChangedEventObject {
	PyObject_HEAD
	bool handle = false;
} FocusChangedEventObject;

PyObject* FocusChangedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int FocusChangedEventObject_Init(FocusChangedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* FocusChangedEventObject_GetGainedFocus(FocusChangedEventObject* self, void* closure);

static PyGetSetDef FocusChangedEventObjectGettersSetters[] = {
	{ "gained_focus", (getter)FocusChangedEventObject_GetGainedFocus, (setter)0, "gained_focus", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject FocusChangedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"FocusChangedEvent", /*tp_name*/
	sizeof(FocusChangedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	FocusChangedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)FocusChangedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	FocusChangedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& FocusChangedEventObject_GetType();