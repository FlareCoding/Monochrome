#pragma once
#include "../ui_bindings/bindings_uiview.h"

typedef struct _UIWindowObject {
	PyObject_HEAD
	Ref<UIWindow> handle = nullptr;
} UIWindowObject;

PyObject* UIWindowObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIWindowObject_Init(UIWindowObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIWindowObject_GetNativeHandle(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetWidth(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetHeight(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetTitle(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetDpi(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_SetSize(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_SetPos(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_SetTitle(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_AddView(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_RemoveView(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_Update(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_ForceUpdate(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_StartWindowLoop(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_IsOpened(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetBackgroundColor(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_SetBackgroundColor(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetCloseButtonRef(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetMaximizeButtonRef(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetMinimizeButtonRef(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetWindowTitleLabelRef(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_SetModernWindowButtonsColor(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_FocusView(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetFocusedView(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_IsViewFocused(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetMouseCursorPos(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_GetAbsoluteMouseCursorPos(UIWindowObject* self, PyObject* args);
PyObject* UIWindowObject_RemoveAllViews(UIWindowObject* self, PyObject* args);

static PyMethodDef UIWindowObjectMethods[] = {
	{ "get_native_handle", (PyCFunction)UIWindowObject_GetNativeHandle, METH_VARARGS, "get_native_handle" },
	{ "get_width", (PyCFunction)UIWindowObject_GetWidth, METH_VARARGS, "get_width" },
	{ "get_height", (PyCFunction)UIWindowObject_GetHeight, METH_VARARGS, "get_height" },
	{ "get_title", (PyCFunction)UIWindowObject_GetTitle, METH_VARARGS, "get_title" },
	{ "get_dpi", (PyCFunction)UIWindowObject_GetDpi, METH_VARARGS, "get_dpi" },
	{ "set_size", (PyCFunction)UIWindowObject_SetSize, METH_VARARGS, "set_size" },
	{ "set_pos", (PyCFunction)UIWindowObject_SetPos, METH_VARARGS, "set_pos" },
	{ "set_title", (PyCFunction)UIWindowObject_SetTitle, METH_VARARGS, "set_title" },
	{ "add_view", (PyCFunction)UIWindowObject_AddView, METH_VARARGS, "add_view" },
	{ "remove_view", (PyCFunction)UIWindowObject_RemoveView, METH_VARARGS, "remove_view" },
	{ "update", (PyCFunction)UIWindowObject_Update, METH_VARARGS, "update" },
	{ "force_update", (PyCFunction)UIWindowObject_ForceUpdate, METH_VARARGS, "force_update" },
	{ "start_window_loop", (PyCFunction)UIWindowObject_StartWindowLoop, METH_VARARGS, "start_window_loop" },
	{ "is_opened", (PyCFunction)UIWindowObject_IsOpened, METH_VARARGS, "is_opened" },
	{ "get_background_color", (PyCFunction)UIWindowObject_GetBackgroundColor, METH_VARARGS, "get_background_color" },
	{ "set_background_color", (PyCFunction)UIWindowObject_SetBackgroundColor, METH_VARARGS, "set_background_color" },
	{ "get_close_button_ref", (PyCFunction)UIWindowObject_GetCloseButtonRef, METH_VARARGS, "get_close_button_ref" },
	{ "get_maximize_button_ref", (PyCFunction)UIWindowObject_GetMaximizeButtonRef, METH_VARARGS, "get_maximize_button_ref" },
	{ "get_minimize_button_ref", (PyCFunction)UIWindowObject_GetMinimizeButtonRef, METH_VARARGS, "get_minimize_button_ref" },
	{ "get_window_title_label_ref", (PyCFunction)UIWindowObject_GetWindowTitleLabelRef, METH_VARARGS, "get_window_title_label_ref" },
	{ "set_modern_window_buttons_color", (PyCFunction)UIWindowObject_SetModernWindowButtonsColor, METH_VARARGS, "set_modern_window_buttons_color" },
	{ "focus_view", (PyCFunction)UIWindowObject_FocusView, METH_VARARGS, "focus_view" },
	{ "get_focused_view", (PyCFunction)UIWindowObject_GetFocusedView, METH_VARARGS, "get_focused_view" },
	{ "is_view_focused", (PyCFunction)UIWindowObject_IsViewFocused, METH_VARARGS, "is_view_focused" },
	{ "get_mouse_cursor_pos", (PyCFunction)UIWindowObject_GetMouseCursorPos, METH_VARARGS, "get_mouse_cursor_pos" },
	{ "get_absolute_mouse_cursor_pos", (PyCFunction)UIWindowObject_GetAbsoluteMouseCursorPos, METH_VARARGS, "get_absolute_mouse_cursor_pos" },
	{ "remove_all_views", (PyCFunction)UIWindowObject_RemoveAllViews, METH_VARARGS, "remove_all_views" },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIWindowObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UIWindow", /*tp_name*/
	sizeof(UIWindowObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIWindowObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIWindowObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIWindowObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIWindowObject_GetType();