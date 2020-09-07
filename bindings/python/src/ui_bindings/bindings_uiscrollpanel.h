#pragma once
#include "bindings_uiview.h"

typedef struct _UIScrollPanelObject {
	UIViewObject _super;
	Ref<UIScrollPanel> handle = nullptr;
} UIScrollPanelObject;

PyObject* UIScrollPanelObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIScrollPanelObject_Init(UIScrollPanelObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIScrollPanelObject_AddChild(UIScrollPanelObject* self, PyObject* args);
PyObject* UIScrollPanelObject_RemoveChild(UIScrollPanelObject* self, PyObject* args);
PyObject* UIScrollPanelObject_Clear(UIScrollPanelObject* self, PyObject* args);
PyObject* UIScrollPanelObject_ScrollToTop(UIScrollPanelObject* self, PyObject* args);
PyObject* UIScrollPanelObject_ScrollToBottom(UIScrollPanelObject* self, PyObject* args);
PyObject* UIScrollPanelObject_ScrollContent(UIScrollPanelObject* self, PyObject* args);

static PyMethodDef UIScrollPanelObjectMethods[] = {
	{ "add_child", (PyCFunction)UIScrollPanelObject_AddChild, METH_VARARGS, "add_child" },
	{ "remove_child", (PyCFunction)UIScrollPanelObject_RemoveChild, METH_VARARGS, "remove_child" },
	{ "clear", (PyCFunction)UIScrollPanelObject_Clear, METH_VARARGS, "clear" },
	{ "scroll_to_top", (PyCFunction)UIScrollPanelObject_ScrollToTop, METH_VARARGS, "scroll_to_top" },
	{ "scroll_to_bottom", (PyCFunction)UIScrollPanelObject_ScrollToBottom, METH_VARARGS, "scroll_to_bottom" },
	{ "scroll_content", (PyCFunction)UIScrollPanelObject_ScrollContent, METH_VARARGS, "scroll_content" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UIScrollPanelObject_GetScrollbarColor(UIScrollPanelObject* self, void* closure);
PyObject* UIScrollPanelObject_GetContentView(UIScrollPanelObject* self, void* closure);

//============ Setters ============ //
int UIScrollPanelObject_SetScrollbarColor(UIScrollPanelObject* self, PyObject* value, void* closure);

static PyGetSetDef UIScrollPanelObjectGettersSetters[] = {
	{ "scrollbar_color", (getter)UIScrollPanelObject_GetScrollbarColor, (setter)UIScrollPanelObject_SetScrollbarColor, "scrollbar_color", NULL },
	{ "content_view", (getter)UIScrollPanelObject_GetContentView, (setter)0, "content_view", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIScrollPanelObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIScrollPanel", /*tp_name*/
	sizeof(UIScrollPanelObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIScrollPanelObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UIScrollPanelObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIScrollPanelObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIScrollPanelObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIScrollPanelObject_GetType();