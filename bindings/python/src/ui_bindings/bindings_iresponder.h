#pragma once
#include "../events_bindings/events_bindings.h"

typedef struct _IResponderObject {
	PyObject_HEAD
	Ref<IResponder> handle = nullptr;
} IResponderObject;

PyObject* IResponderObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int IResponderObject_Init(IResponderObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* IResponderObject_AddMousePressedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddMouseReleasedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddMouseClickedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddMouseMovedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddHoverOnEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddHoverOffEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddKeyPressedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddKeyReleasedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddFocusChangedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddWindowResizedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddWindowClosedEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddWindowGainedFocusEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddWindowLostFocusEventHandler(IResponderObject* self, PyObject* args);
PyObject* IResponderObject_AddWindowUpdatedEventHandler(IResponderObject* self, PyObject* args);

static PyMethodDef IResponderObjectMethods[] = {
	{ "add_mouse_pressed_event_handler", (PyCFunction)IResponderObject_AddMousePressedEventHandler, METH_VARARGS, "add_mouse_pressed_event_handler" },
	{ "add_mouse_released_event_handler", (PyCFunction)IResponderObject_AddMouseReleasedEventHandler, METH_VARARGS, "add_mouse_released_event_handler" },
	{ "add_mouse_clicked_event_handler", (PyCFunction)IResponderObject_AddMouseClickedEventHandler, METH_VARARGS, "add_mouse_clicked_event_handler" },
	{ "add_mouse_moved_event_handler", (PyCFunction)IResponderObject_AddMouseMovedEventHandler, METH_VARARGS, "add_mouse_moved_event_handler" },
	{ "add_hover_on_event_handler", (PyCFunction)IResponderObject_AddHoverOnEventHandler, METH_VARARGS, "add_hover_on_event_handler" },
	{ "add_hover_off_event_handler", (PyCFunction)IResponderObject_AddHoverOffEventHandler, METH_VARARGS, "add_hover_off_event_handler" },
	{ "add_key_pressed_event_handler", (PyCFunction)IResponderObject_AddKeyPressedEventHandler, METH_VARARGS, "add_key_pressed_event_handler" },
	{ "add_key_released_event_handler", (PyCFunction)IResponderObject_AddKeyReleasedEventHandler, METH_VARARGS, "add_key_released_event_handler" },
	{ "add_focus_changed_event_handler", (PyCFunction)IResponderObject_AddFocusChangedEventHandler, METH_VARARGS, "add_focus_changed_event_handler" },
	{ "add_window_resized_event_handler", (PyCFunction)IResponderObject_AddWindowResizedEventHandler, METH_VARARGS, "add_window_resized_event_handler" },
	{ "add_window_closed_event_handler", (PyCFunction)IResponderObject_AddWindowClosedEventHandler, METH_VARARGS, "add_window_closed_event_handler" },
	{ "add_window_gained_focus_event_handler", (PyCFunction)IResponderObject_AddWindowGainedFocusEventHandler, METH_VARARGS, "add_window_gained_focus_event_handler" },
	{ "add_window_lost_focus_event_handler", (PyCFunction)IResponderObject_AddWindowLostFocusEventHandler, METH_VARARGS, "add_window_lost_focus_event_handler" },
	{ "add_window_updated_event_handler", (PyCFunction)IResponderObject_AddWindowUpdatedEventHandler, METH_VARARGS, "add_window_updated_event_handler" },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject IResponderObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"IResponder", /*tp_name*/
	sizeof(IResponderObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	IResponderObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	0, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)IResponderObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	IResponderObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& IResponderObject_GetType();
