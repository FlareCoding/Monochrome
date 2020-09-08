#pragma once
#include "bindings_uiview.h"
#include "bindings_textproperties.h"

typedef struct _UITextAreaObject {
	UIViewObject _super;
	Ref<UITextArea> handle = nullptr;
} UITextAreaObject;

PyObject* UITextAreaObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UITextAreaObject_Init(UITextAreaObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* UITextAreaObject_GetText(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetProperties(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetTextColor(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetTopMargins(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetBottomMargins(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetLeftMargins(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetRightMargins(UITextAreaObject* self, void* closure);
PyObject* UITextAreaObject_GetAutoScroll(UITextAreaObject* self, void* closure);

//============ Setters ============ //
int UITextAreaObject_SetText(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetTextColor(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetTopMargins(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetBottomMargins(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetLeftMargins(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetRightMargins(UITextAreaObject* self, PyObject* value, void* closure);
int UITextAreaObject_SetAutoScroll(UITextAreaObject* self, PyObject* value, void* closure);

static PyGetSetDef UITextAreaObjectGettersSetters[] = {
	{ "text", (getter)UITextAreaObject_GetText, (setter)UITextAreaObject_SetText, "text", NULL },
	{ "properties", (getter)UITextAreaObject_GetProperties, (setter)0, "properties", NULL },
	{ "text_color", (getter)UITextAreaObject_GetTextColor, (setter)UITextAreaObject_SetTextColor, "text_color", NULL },
	{ "top_margins", (getter)UITextAreaObject_GetTopMargins, (setter)UITextAreaObject_SetTopMargins, "top_margins", NULL },
	{ "bottom_margins", (getter)UITextAreaObject_GetBottomMargins, (setter)UITextAreaObject_SetBottomMargins, "bottom_margins", NULL },
	{ "left_margins", (getter)UITextAreaObject_GetLeftMargins, (setter)UITextAreaObject_SetLeftMargins, "left_margins", NULL },
	{ "right_margins", (getter)UITextAreaObject_GetRightMargins, (setter)UITextAreaObject_SetRightMargins, "right_margins", NULL },
	{ "auto_scroll", (getter)UITextAreaObject_GetAutoScroll, (setter)UITextAreaObject_SetAutoScroll, "auto_scroll", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UITextAreaObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UITextArea", /*tp_name*/
	sizeof(UITextAreaObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	UITextAreaObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UITextAreaObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UITextAreaObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UITextAreaObject_GetType();