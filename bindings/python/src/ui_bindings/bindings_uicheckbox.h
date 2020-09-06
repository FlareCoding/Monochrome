#pragma once
#include "bindings_uilabel.h"

typedef struct _UICheckboxObject {
	UIViewObject _super;
	Ref<UICheckbox> handle = nullptr;
} UICheckboxObject;

PyObject* UICheckboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UICheckboxObject_Init(UICheckboxObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UICheckboxObject_Toggle(UICheckboxObject* self, PyObject* args);
PyObject* UICheckboxObject_AddValueChangedEventHandler(UICheckboxObject* self, PyObject* args);

static PyMethodDef UICheckboxObjectMethods[] = {
	{ "toggle", (PyCFunction)UICheckboxObject_Toggle, METH_VARARGS, "toggle" },
	{ "add_value_changed_event_handler", (PyCFunction)UICheckboxObject_AddValueChangedEventHandler, METH_VARARGS, "add_value_changed_event_handler" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UICheckboxObject_GetChecked(UICheckboxObject* self, void* closure);
PyObject* UICheckboxObject_GetBoxSize(UICheckboxObject* self, void* closure);
PyObject* UICheckboxObject_GetLabelMargins(UICheckboxObject* self, void* closure);
PyObject* UICheckboxObject_GetCheckmarkColor(UICheckboxObject* self, void* closure);
PyObject* UICheckboxObject_GetCheckedBoxColor(UICheckboxObject* self, void* closure);
PyObject* UICheckboxObject_GetLabel(UICheckboxObject* self, void* closure);

//============ Setters ============ //
int UICheckboxObject_SetChecked(UICheckboxObject* self, PyObject* value, void* closure);
int UICheckboxObject_SetBoxSize(UICheckboxObject* self, PyObject* value, void* closure);
int UICheckboxObject_SetLabelMargins(UICheckboxObject* self, PyObject* value, void* closure);
int UICheckboxObject_SetCheckmarkColor(UICheckboxObject* self, PyObject* value, void* closure);
int UICheckboxObject_SetCheckedBoxColor(UICheckboxObject* self, PyObject* value, void* closure);

static PyGetSetDef UICheckboxObjectGettersSetters[] = {
	{ "checked", (getter)UICheckboxObject_GetChecked, (setter)UICheckboxObject_SetChecked, "checked", NULL },
	{ "box_size", (getter)UICheckboxObject_GetBoxSize, (setter)UICheckboxObject_SetBoxSize, "box_size", NULL },
	{ "label_margins", (getter)UICheckboxObject_GetLabelMargins, (setter)UICheckboxObject_SetLabelMargins, "label_margins", NULL },
	{ "checkmark_color", (getter)UICheckboxObject_GetCheckmarkColor, (setter)UICheckboxObject_SetCheckmarkColor, "checkmark_color", NULL },
	{ "checked_box_color", (getter)UICheckboxObject_GetCheckedBoxColor, (setter)UICheckboxObject_SetCheckedBoxColor, "checked_box_color", NULL },
	{ "label", (getter)UICheckboxObject_GetLabelMargins, (setter)UICheckboxObject_SetLabelMargins, "label", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UICheckboxObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UICheckbox", /*tp_name*/
	sizeof(UICheckboxObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UICheckboxObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UICheckboxObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UICheckboxObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UICheckboxObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UICheckboxObject_GetType();