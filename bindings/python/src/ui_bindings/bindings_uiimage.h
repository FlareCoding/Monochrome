#pragma once
#include "bindings_uiview.h"

typedef struct _UIImageObject {
	UIViewObject _super;
	Ref<UIImage> handle = nullptr;
} UIImageObject;

PyObject* UIImageObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIImageObject_Init(UIImageObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIImageObject_LoadImageFromFile(UIImageObject* self, PyObject* args);
PyObject* UIImageObject_LoadWebImage(UIImageObject* self, PyObject* args);
PyObject* UIImageObject_IsImageLoaded(UIImageObject* self, PyObject* args);
PyObject* UIImageObject_GetWidth(UIImageObject* self, PyObject* args);
PyObject* UIImageObject_GetHeight(UIImageObject* self, PyObject* args);

static PyMethodDef UIImageObjectMethods[] = {
	{ "load_image_from_file", (PyCFunction)UIImageObject_LoadImageFromFile, METH_VARARGS, "load_image_from_file" },
	{ "load_web_image", (PyCFunction)UIImageObject_LoadWebImage, METH_VARARGS, "load_web_image" },
	{ "is_image_loaded", (PyCFunction)UIImageObject_IsImageLoaded, METH_VARARGS, "is_image_loaded" },
	{ "get_width", (PyCFunction)UIImageObject_GetWidth, METH_VARARGS, "get_width" },
	{ "get_height", (PyCFunction)UIImageObject_GetHeight, METH_VARARGS, "get_height" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UIImageObject_GetOpacity(UIImageObject* self, void* closure);

//============ Setters ============ //
int UIImageObject_SetOpacity(UIImageObject* self, PyObject* value, void* closure);

static PyGetSetDef UIImageObjectGettersSetters[] = {
	{ "opacity", (getter)UIImageObject_GetOpacity, (setter)UIImageObject_SetOpacity, "opacity", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIImageObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIImage", /*tp_name*/
	sizeof(UIImageObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIImageObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UIImageObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIImageObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIImageObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIImageObject_GetType();