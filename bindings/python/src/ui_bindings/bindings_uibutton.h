#pragma once
#include "bindings_uilabel.h"

typedef struct _UIButtonObject {
	UIViewObject _super;
	Ref<UIButton> handle = nullptr;
} UIButtonObject;

PyObject* UIButtonObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIButtonObject_Init(UIButtonObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* UIButtonObject_GetFilled(UIButtonObject* self, void* closure);
PyObject* UIButtonObject_GetStroke(UIButtonObject* self, void* closure);
PyObject* UIButtonObject_GetHoverOnColor(UIButtonObject* self, void* closure);
PyObject* UIButtonObject_GetOnMousePressColor(UIButtonObject* self, void* closure);
PyObject* UIButtonObject_GetLabel(UIButtonObject* self, void* closure);

//============ Setters ============ //
int UIButtonObject_SetFilled(UIButtonObject* self, PyObject* value, void* closure);
int UIButtonObject_SetStroke(UIButtonObject* self, PyObject* value, void* closure);
int UIButtonObject_SetHoverOnColor(UIButtonObject* self, PyObject* value, void* closure);
int UIButtonObject_SetOnMousePressColor(UIButtonObject* self, PyObject* value, void* closure);

static PyGetSetDef UIButtonObjectGettersSetters[] = {
	{ "filled", (getter)UIButtonObject_GetFilled, (setter)UIButtonObject_SetFilled, "filled", NULL },
	{ "stroke", (getter)UIButtonObject_GetStroke, (setter)UIButtonObject_SetStroke, "stroke", NULL },
	{ "hover_on_color", (getter)UIButtonObject_GetHoverOnColor, (setter)UIButtonObject_SetHoverOnColor, "hover_on_color", NULL },
	{ "on_mouse_press_color", (getter)UIButtonObject_GetOnMousePressColor, (setter)UIButtonObject_SetOnMousePressColor, "on_mouse_press_color", NULL },
	{ "label", (getter)UIButtonObject_GetLabel, (setter)0, "label", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIButtonObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIButton", /*tp_name*/
	sizeof(UIButtonObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	UIButtonObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIButtonObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIButtonObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIButtonObject_GetType();