#pragma once
#include "bindings_uiview.h"

typedef struct _UISliderObject {
	UIViewObject _super;
	Ref<UISlider> handle = nullptr;
} UISliderObject;

PyObject* UISliderObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UISliderObject_Init(UISliderObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UISliderObject_AddValueChangedEventHandler(UISliderObject* self, PyObject* args);

static PyMethodDef UISliderObjectMethods[] = {
	{ "add_value_changed_event_handler", (PyCFunction)UISliderObject_AddValueChangedEventHandler, METH_VARARGS, "add_value_changed_event_handler" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UISliderObject_GetSliderKnobShape(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetSliderBarHeight(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetSliderKnobColor(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetTickmarksColor(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetMaxValue(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetMinValue(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetValue(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetIntervals(UISliderObject* self, void* closure);
PyObject* UISliderObject_GetVisibleTickmarks(UISliderObject* self, void* closure);

//============ Setters ============ //
int UISliderObject_SetSliderKnobShape(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetSliderBarHeight(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetSliderKnobColor(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetTickmarksColor(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetMaxValue(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetMinValue(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetValue(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetIntervals(UISliderObject* self, PyObject* value, void* closure);
int UISliderObject_SetVisibleTickmarks(UISliderObject* self, PyObject* value, void* closure);

static PyGetSetDef UISliderObjectGettersSetters[] = {
	{ "slider_knob_shape", (getter)UISliderObject_GetSliderKnobShape, (setter)UISliderObject_SetSliderKnobShape, "slider_knob_shape", NULL },
	{ "slider_bar_height", (getter)UISliderObject_GetSliderBarHeight, (setter)UISliderObject_SetSliderBarHeight, "slider_bar_height", NULL },
	{ "slider_knob_color", (getter)UISliderObject_GetSliderKnobColor, (setter)UISliderObject_SetSliderKnobColor, "slider_knob_color", NULL },
	{ "tickmarks_color", (getter)UISliderObject_GetTickmarksColor, (setter)UISliderObject_SetTickmarksColor, "tickmarks_color", NULL },
	{ "max_value", (getter)UISliderObject_GetMaxValue, (setter)UISliderObject_SetMaxValue, "max_value", NULL },
	{ "min_value", (getter)UISliderObject_GetMinValue, (setter)UISliderObject_SetMinValue, "min_value", NULL },
	{ "value", (getter)UISliderObject_GetValue, (setter)UISliderObject_SetValue, "value", NULL },
	{ "intervals", (getter)UISliderObject_GetIntervals, (setter)UISliderObject_SetIntervals, "intervals", NULL },
	{ "visible_tickmarks", (getter)UISliderObject_GetVisibleTickmarks, (setter)UISliderObject_SetVisibleTickmarks, "visible_tickmarks", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UISliderObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UISlider", /*tp_name*/
	sizeof(UISliderObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UISliderObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UISliderObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UISliderObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UISliderObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UISliderObject_GetType();