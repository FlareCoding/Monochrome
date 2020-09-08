#pragma once
#include "bindings_point.h"

typedef struct _FrameObject {
	PyObject_HEAD
	Ref<Frame> handle = nullptr;
} FrameObject;

PyObject* FrameObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int FrameObject_Init(FrameObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* FrameObject_GetPosition(FrameObject* self, void* closure);
PyObject* FrameObject_GetSize(FrameObject* self, void* closure);

//============ Setters ============ //
int FrameObject_SetPosition(FrameObject* self, PyObject* value, void* closure);
int FrameObject_SetSize(FrameObject* self, PyObject* value, void* closure);

static PyGetSetDef FrameObjectGettersSetters[] = {
	{ "position", (getter)FrameObject_GetPosition, (setter)FrameObject_SetPosition, "position", NULL },
	{ "size", (getter)FrameObject_GetSize, (setter)FrameObject_SetSize, "size", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject FrameObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Frame", /*tp_name*/
	sizeof(FrameObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	FrameObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)FrameObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	FrameObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& FrameObject_GetType();