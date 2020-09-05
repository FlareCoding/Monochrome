#pragma once
#include "bindings_textproperties.h"
#include "bindings_uiview.h"

typedef struct _UILabelObject {
	UIViewObject _super;
	Ref<UILabel> handle = nullptr;
} UILabelObject;

PyObject* UILabelObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UILabelObject_Init(UILabelObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* UILabelObject_GetText(UILabelObject* self, void* closure);
PyObject* UILabelObject_GetProperties(UILabelObject* self, void* closure);
PyObject* UILabelObject_GetColor(UILabelObject* self, void* closure);

//============ Setters ============ //
int UILabelObject_SetText(UILabelObject* self, PyObject* value, void* closure);
int UILabelObject_SetColor(UILabelObject* self, PyObject* value, void* closure);

static PyGetSetDef UILabelObjectGettersSetters[] = {
	{ "text", (getter)UILabelObject_GetText, (setter)UILabelObject_SetText, "text", NULL },
	{ "properties", (getter)UILabelObject_GetProperties, (setter)0, "properties", NULL },
	{ "color", (getter)UILabelObject_GetColor, (setter)UILabelObject_SetColor, "color", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UILabelObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UILabel", /*tp_name*/
	sizeof(UILabelObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	UILabelObjectGettersSetters, /*tp_getset*/
	&UIViewObject_GetType(), /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UILabelObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UILabelObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UILabelObject_GetType();