#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _ColorObject {
	PyObject_HEAD
	Ref<Color> handle = nullptr;
} ColorObject;

PyObject* ColorObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int ColorObject_Init(ColorObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* ColorObject_GetR(ColorObject* self, void* closure);
PyObject* ColorObject_GetG(ColorObject* self, void* closure);
PyObject* ColorObject_GetB(ColorObject* self, void* closure);
PyObject* ColorObject_GetAlpha(ColorObject* self, void* closure);

//============ Setters ============ //
int ColorObject_SetR(ColorObject* self, PyObject* value, void* closure);
int ColorObject_SetG(ColorObject* self, PyObject* value, void* closure);
int ColorObject_SetB(ColorObject* self, PyObject* value, void* closure);
int ColorObject_SetAlpha(ColorObject* self, PyObject* value, void* closure);

static PyGetSetDef ColorObjectGettersSetters[] = {
	{ "r", (getter)ColorObject_GetR, (setter)ColorObject_SetR, "r", NULL },
	{ "g", (getter)ColorObject_GetG, (setter)ColorObject_SetG, "g", NULL },
	{ "b", (getter)ColorObject_GetB, (setter)ColorObject_SetB, "b", NULL },
	{ "alpha", (getter)ColorObject_GetAlpha, (setter)ColorObject_SetAlpha, "alpha", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject ColorObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Color", /*tp_name*/
	sizeof(ColorObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	ColorObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)ColorObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	ColorObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& ColorObject_GetType();