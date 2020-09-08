#pragma once
#include "bindings_uibutton.h"

typedef struct _UITabViewObject {
	UIViewObject _super;
	Ref<UITabView> handle = nullptr;
} UITabViewObject;

PyObject* UITabViewObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UITabViewObject_Init(UITabViewObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UITabViewObject_AddTab(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_RemoveTabByIndex(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_RemoveTab(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_GetTabByIndex(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_GetTab(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_OpenTabByIndex(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_OpenTab(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_GetOpenedTab(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_GetTabCount(UITabViewObject* self, PyObject* args);
PyObject* UITabViewObject_StyleTabButtons(UITabViewObject* self, PyObject* args);

static PyMethodDef UITabViewObjectMethods[] = {
	{ "add_tab", (PyCFunction)UITabViewObject_AddTab, METH_VARARGS, "add_tab" },
	{ "remove_tab_by_index", (PyCFunction)UITabViewObject_RemoveTabByIndex, METH_VARARGS, "remove_tab_by_index" },
	{ "remove_tab", (PyCFunction)UITabViewObject_RemoveTab, METH_VARARGS, "remove_tab" },
	{ "get_tab_by_index", (PyCFunction)UITabViewObject_GetTabByIndex, METH_VARARGS, "get_tab_by_index" },
	{ "get_tab", (PyCFunction)UITabViewObject_GetTab, METH_VARARGS, "get_tab" },
	{ "open_tab_by_index", (PyCFunction)UITabViewObject_OpenTabByIndex, METH_VARARGS, "open_tab_by_index" },
	{ "open_tab", (PyCFunction)UITabViewObject_OpenTab, METH_VARARGS, "open_tab" },
	{ "get_opened_tab", (PyCFunction)UITabViewObject_GetOpenedTab, METH_VARARGS, "get_opened_tab" },
	{ "get_tab_count", (PyCFunction)UITabViewObject_GetTabCount, METH_VARARGS, "get_tab_count" },
	{ "style_tab_buttons", (PyCFunction)UITabViewObject_StyleTabButtons, METH_VARARGS, "style_tab_buttons" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UITabViewObject_GetTabAreaHeight(UITabViewObject* self, void* closure);
PyObject* UITabViewObject_GetTabWidth(UITabViewObject* self, void* closure);
PyObject* UITabViewObject_GetUnderlineTabs(UITabViewObject* self, void* closure);
PyObject* UITabViewObject_GetTabUnderlineColor(UITabViewObject* self, void* closure);
PyObject* UITabViewObject_GetSelectedTabColor(UITabViewObject* self, void* closure);

//============ Setters ============ //
int UITabViewObject_SetTabAreaHeight(UITabViewObject* self, PyObject* value, void* closure);
int UITabViewObject_SetTabWidth(UITabViewObject* self, PyObject* value, void* closure);
int UITabViewObject_SetUnderlineTabs(UITabViewObject* self, PyObject* value, void* closure);
int UITabViewObject_SetTabUnderlineColor(UITabViewObject* self, PyObject* value, void* closure);
int UITabViewObject_SetSelectedTabColor(UITabViewObject* self, PyObject* value, void* closure);

static PyGetSetDef UITabViewObjectGettersSetters[] = {
	{ "tab_area_height", (getter)UITabViewObject_GetTabAreaHeight, (setter)UITabViewObject_SetTabAreaHeight, "tab_area_height", NULL },
	{ "tab_width", (getter)UITabViewObject_GetTabWidth, (setter)UITabViewObject_SetTabWidth, "tab_width", NULL },
	{ "underline_tabs", (getter)UITabViewObject_GetUnderlineTabs, (setter)UITabViewObject_SetUnderlineTabs, "underline_tabs", NULL },
	{ "tab_underline_color", (getter)UITabViewObject_GetTabUnderlineColor, (setter)UITabViewObject_SetTabUnderlineColor, "tab_underline_color", NULL },
	{ "selected_tab_color", (getter)UITabViewObject_GetSelectedTabColor, (setter)UITabViewObject_SetSelectedTabColor, "selected_tab_color", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UITabViewObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UITabView", /*tp_name*/
	sizeof(UITabViewObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UITabViewObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UITabViewObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UITabViewObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UITabViewObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UITabViewObject_GetType();