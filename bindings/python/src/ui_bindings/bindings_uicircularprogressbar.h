#pragma once
#include "bindings_uiview.h"

typedef struct _UICircularProgressBarObject {
	UIViewObject _super;
	Ref<UICircularProgressBar> handle = nullptr;
} UICircularProgressBarObject;

PyObject* UICircularProgressBarObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UICircularProgressBarObject_Init(UICircularProgressBarObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UICircularProgressBarObject_AddValueChangedEventHandler(UICircularProgressBarObject* self, PyObject* args);

static PyMethodDef UICircularProgressBarObjectMethods[] = {
	{ "add_value_changed_event_handler", (PyCFunction)UICircularProgressBarObject_AddValueChangedEventHandler, METH_VARARGS, "add_value_changed_event_handler" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UICircularProgressBarObject_GetStroke(UICircularProgressBarObject* self, void* closure);
PyObject* UICircularProgressBarObject_GetProgressColor(UICircularProgressBarObject* self, void* closure);
PyObject* UICircularProgressBarObject_GetMaxValue(UICircularProgressBarObject* self, void* closure);
PyObject* UICircularProgressBarObject_GetMinValue(UICircularProgressBarObject* self, void* closure);
PyObject* UICircularProgressBarObject_GetValue(UICircularProgressBarObject* self, void* closure);

//============ Setters ============ //
int UICircularProgressBarObject_SetStroke(UICircularProgressBarObject* self, PyObject* value, void* closure);
int UICircularProgressBarObject_SetProgressColor(UICircularProgressBarObject* self, PyObject* value, void* closure);
int UICircularProgressBarObject_SetMaxValue(UICircularProgressBarObject* self, PyObject* value, void* closure);
int UICircularProgressBarObject_SetMinValue(UICircularProgressBarObject* self, PyObject* value, void* closure);
int UICircularProgressBarObject_SetValue(UICircularProgressBarObject* self, PyObject* value, void* closure);

static PyGetSetDef UICircularProgressBarObjectGettersSetters[] = {
	{ "stroke", (getter)UICircularProgressBarObject_GetStroke, (setter)UICircularProgressBarObject_SetStroke, "stroke", NULL },
	{ "progress_color", (getter)UICircularProgressBarObject_GetProgressColor, (setter)UICircularProgressBarObject_SetProgressColor, "progress_color", NULL },
	{ "max_value", (getter)UICircularProgressBarObject_GetMaxValue, (setter)UICircularProgressBarObject_SetMaxValue, "max_value", NULL },
	{ "min_value", (getter)UICircularProgressBarObject_GetMinValue, (setter)UICircularProgressBarObject_SetMinValue, "min_value", NULL },
	{ "value", (getter)UICircularProgressBarObject_GetValue, (setter)UICircularProgressBarObject_SetValue, "value", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UICircularProgressBarObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UICircularProgressBar", /*tp_name*/
	sizeof(UICircularProgressBarObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UICircularProgressBarObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UICircularProgressBarObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UICircularProgressBarObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UICircularProgressBarObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UICircularProgressBarObject_GetType();