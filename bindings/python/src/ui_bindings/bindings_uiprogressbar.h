#pragma once
#include "bindings_uiview.h"

typedef struct _UIProgressBarObject {
	UIViewObject _super;
	Ref<UIProgressBar> handle = nullptr;
} UIProgressBarObject;

PyObject* UIProgressBarObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIProgressBarObject_Init(UIProgressBarObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIProgressBarObject_AddValueChangedEventHandler(UIProgressBarObject* self, PyObject* args);

static PyMethodDef UIProgressBarObjectMethods[] = {
	{ "add_value_changed_event_handler", (PyCFunction)UIProgressBarObject_AddValueChangedEventHandler, METH_VARARGS, "add_value_changed_event_handler" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UIProgressBarObject_GetFilled(UIProgressBarObject* self, void* closure);
PyObject* UIProgressBarObject_GetStroke(UIProgressBarObject* self, void* closure);
PyObject* UIProgressBarObject_GetProgressColor(UIProgressBarObject* self, void* closure);
PyObject* UIProgressBarObject_GetMaxValue(UIProgressBarObject* self, void* closure);
PyObject* UIProgressBarObject_GetMinValue(UIProgressBarObject* self, void* closure);
PyObject* UIProgressBarObject_GetValue(UIProgressBarObject* self, void* closure);

//============ Setters ============ //
int UIProgressBarObject_SetFilled(UIProgressBarObject* self, PyObject* value, void* closure);
int UIProgressBarObject_SetStroke(UIProgressBarObject* self, PyObject* value, void* closure);
int UIProgressBarObject_SetProgressColor(UIProgressBarObject* self, PyObject* value, void* closure);
int UIProgressBarObject_SetMaxValue(UIProgressBarObject* self, PyObject* value, void* closure);
int UIProgressBarObject_SetMinValue(UIProgressBarObject* self, PyObject* value, void* closure);
int UIProgressBarObject_SetValue(UIProgressBarObject* self, PyObject* value, void* closure);

static PyGetSetDef UIProgressBarObjectGettersSetters[] = {
	{ "filled", (getter)UIProgressBarObject_GetFilled, (setter)UIProgressBarObject_SetFilled, "filled", NULL },
	{ "stroke", (getter)UIProgressBarObject_GetStroke, (setter)UIProgressBarObject_SetStroke, "stroke", NULL },
	{ "progress_color", (getter)UIProgressBarObject_GetProgressColor, (setter)UIProgressBarObject_SetProgressColor, "progress_color", NULL },
	{ "max_value", (getter)UIProgressBarObject_GetMaxValue, (setter)UIProgressBarObject_SetMaxValue, "max_value", NULL },
	{ "min_value", (getter)UIProgressBarObject_GetMinValue, (setter)UIProgressBarObject_SetMinValue, "min_value", NULL },
	{ "value", (getter)UIProgressBarObject_GetValue, (setter)UIProgressBarObject_SetValue, "value", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIProgressBarObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIProgressBar", /*tp_name*/
	sizeof(UIProgressBarObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIProgressBarObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UIProgressBarObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIProgressBarObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIProgressBarObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIProgressBarObject_GetType();