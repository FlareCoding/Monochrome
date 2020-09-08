#pragma once
#include "bindings_uiview.h"
#include "bindings_textproperties.h"

typedef struct _UITextboxObject {
	UIViewObject _super;
	Ref<UITextbox> handle = nullptr;
} UITextboxObject;

PyObject* UITextboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UITextboxObject_Init(UITextboxObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* UITextboxObject_GetStroke(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GetFocusedHighlightColor(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GetText(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GetPlaceholder(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GettextProperties(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GetTextColor(UITextboxObject* self, void* closure);
PyObject* UITextboxObject_GetReadOnly(UITextboxObject* self, void* closure);

//============ Setters ============ //
int UITextboxObject_SetStroke(UITextboxObject* self, PyObject* value, void* closure);
int UITextboxObject_SetFocusedHighlightColor(UITextboxObject* self, PyObject* value, void* closure);
int UITextboxObject_SetText(UITextboxObject* self, PyObject* value, void* closure);
int UITextboxObject_SetPlaceholder(UITextboxObject* self, PyObject* value, void* closure);
int UITextboxObject_SetTextColor(UITextboxObject* self, PyObject* value, void* closure);
int UITextboxObject_SetReadOnly(UITextboxObject* self, PyObject* value, void* closure);

static PyGetSetDef UITextboxObjectGettersSetters[] = {
	{ "stroke", (getter)UITextboxObject_GetStroke, (setter)UITextboxObject_SetStroke, "stroke", NULL },
	{ "focused_highlight_color", (getter)UITextboxObject_GetFocusedHighlightColor, (setter)UITextboxObject_SetFocusedHighlightColor, "focused_highlight_color", NULL },
	{ "text", (getter)UITextboxObject_GetStroke, (setter)UITextboxObject_SetStroke, "text", NULL },
	{ "placeholder", (getter)UITextboxObject_GetPlaceholder, (setter)UITextboxObject_SetPlaceholder, "placeholder", NULL },
	{ "text_properties", (getter)UITextboxObject_GettextProperties, (setter)0, "text_properties", NULL },
	{ "text_color", (getter)UITextboxObject_GetTextColor, (setter)UITextboxObject_SetTextColor, "text_color", NULL },
	{ "read_only", (getter)UITextboxObject_GetReadOnly, (setter)UITextboxObject_SetReadOnly, "read_only", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UITextboxObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UITextbox", /*tp_name*/
	sizeof(UITextboxObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	UITextboxObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UITextboxObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UITextboxObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UITextboxObject_GetType();