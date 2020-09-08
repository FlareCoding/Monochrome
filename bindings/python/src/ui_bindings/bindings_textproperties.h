#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _TextPropertiesObject {
	PyObject_HEAD
	Ref<TextProperties> handle = nullptr;
} TextPropertiesObject;

PyObject* TextPropertiesObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int TextPropertiesObject_Init(TextPropertiesObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* TextPropertiesObject_GetFont(TextPropertiesObject* self, void* closure);
PyObject* TextPropertiesObject_GetFontSize(TextPropertiesObject* self, void* closure);
PyObject* TextPropertiesObject_GetAlignment(TextPropertiesObject* self, void* closure);
PyObject* TextPropertiesObject_GetVerticalAlignment(TextPropertiesObject* self, void* closure);
PyObject* TextPropertiesObject_GetStyle(TextPropertiesObject* self, void* closure);
PyObject* TextPropertiesObject_GetWrapping(TextPropertiesObject* self, void* closure);

//============ Setters ============ //
int TextPropertiesObject_SetFont(TextPropertiesObject* self, PyObject* value, void* closure);
int TextPropertiesObject_SetFontSize(TextPropertiesObject* self, PyObject* value, void* closure);
int TextPropertiesObject_SetAlignment(TextPropertiesObject* self, PyObject* value, void* closure);
int TextPropertiesObject_SetVerticalAlignment(TextPropertiesObject* self, PyObject* value, void* closure);
int TextPropertiesObject_SetStyle(TextPropertiesObject* self, PyObject* value, void* closure);
int TextPropertiesObject_SetWrapping(TextPropertiesObject* self, PyObject* value, void* closure);

static PyGetSetDef TextPropertiesObjectGettersSetters[] = {
	{ "font", (getter)TextPropertiesObject_GetFont, (setter)TextPropertiesObject_SetFont, "font", NULL },
	{ "font_size", (getter)TextPropertiesObject_GetFontSize, (setter)TextPropertiesObject_SetFontSize, "font_size", NULL },
	{ "alignment", (getter)TextPropertiesObject_GetAlignment, (setter)TextPropertiesObject_SetAlignment, "alignment", NULL },
	{ "vertical_alignment", (getter)TextPropertiesObject_GetVerticalAlignment, (setter)TextPropertiesObject_SetVerticalAlignment, "vertical_alignment", NULL },
	{ "style", (getter)TextPropertiesObject_GetStyle, (setter)TextPropertiesObject_SetStyle, "style", NULL },
	{ "wrapping", (getter)TextPropertiesObject_GetWrapping, (setter)TextPropertiesObject_SetWrapping, "wrapping", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject TextPropertiesObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"TextProperties", /*tp_name*/
	sizeof(TextPropertiesObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	TextPropertiesObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)TextPropertiesObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	TextPropertiesObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& TextPropertiesObject_GetType();