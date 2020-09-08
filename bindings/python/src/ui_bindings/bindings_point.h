#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _PointObject {
	PyObject_HEAD
	Ref<Point> handle = nullptr;
} PointObject;

PyObject* PointObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int PointObject_Init(PointObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* PointObject_GetX(PointObject* self, void* closure);
PyObject* PointObject_GetY(PointObject* self, void* closure);

//============ Setters ============ //
int PointObject_SetX(PointObject* self, PyObject* value, void* closure);
int PointObject_SetY(PointObject* self, PyObject* value, void* closure);

static PyGetSetDef PointObjectGettersSetters[] = {
	{ "x", (getter)PointObject_GetX, (setter)PointObject_SetX, "x", NULL },
	{ "y", (getter)PointObject_GetY, (setter)PointObject_SetY, "y", NULL },
	{ "width", (getter)PointObject_GetX, (setter)PointObject_SetX, "width", NULL },
	{ "height", (getter)PointObject_GetY, (setter)PointObject_SetY, "height", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject PointObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Point", /*tp_name*/
	sizeof(PointObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	PointObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)PointObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	PointObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& PointObject_GetType();