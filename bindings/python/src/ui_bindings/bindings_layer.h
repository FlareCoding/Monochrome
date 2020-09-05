#pragma once
#include "bindings_frame.h"
#include "bindings_color.h"

typedef struct _LayerObject {
	PyObject_HEAD
	Ref<Layer> handle = nullptr;
} LayerObject;

PyObject* LayerObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int LayerObject_Init(LayerObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* LayerObject_GetColor(LayerObject* self, void* closure);
PyObject* LayerObject_GetFrame(LayerObject* self, void* closure);

//============ Setters ============ //
int LayerObject_SetColor(LayerObject* self, PyObject* value, void* closure);
int LayerObject_SetFrame(LayerObject* self, PyObject* value, void* closure);

static PyGetSetDef LayerObjectGettersSetters[] = {
	{ "color", (getter)LayerObject_GetColor, (setter)LayerObject_SetColor, "color", NULL },
	{ "frame", (getter)LayerObject_GetFrame, (setter)LayerObject_SetFrame, "frame", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject LayerObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Layer", /*tp_name*/
	sizeof(LayerObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	LayerObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)LayerObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	LayerObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& LayerObject_GetType();