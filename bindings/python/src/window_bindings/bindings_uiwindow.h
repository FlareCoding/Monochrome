#pragma once
#include "../ui_bindings/bindings_uiview.h"

typedef struct _UIWindowObject {
	PyObject_HEAD
	Ref<UIWindow> handle = nullptr;
} UIWindowObject;

PyObject* UIWindowObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIWindowObject_Init(UIWindowObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIWindowObject_StartWindowLoop(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_AddView(UIWindowObject* self, PyObject* args);

static PyMethodDef UIWindowObjectMethods[] = {
	{ "start_window_loop", (PyCFunction)UIWindowObject_StartWindowLoop, METH_VARARGS, "start_window_loop" },
	{ "add_view", (PyCFunction)UIWindowObject_AddView, METH_VARARGS, "add_view" },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIWindowObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIWindow", /*tp_name*/
	sizeof(UIWindowObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIWindowObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIWindowObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIWindowObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIWindowObject_GetType();
