#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _KeyPressedEventObject {
	PyObject_HEAD
	KeyPressedEvent handle = KeyPressedEvent(KeyCode::None, false, false, false);
} KeyPressedEventObject;

PyObject* KeyPressedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int KeyPressedEventObject_Init(KeyPressedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* KeyPressedEventObject_GetKeycode(KeyPressedEventObject* self, void* closure);
PyObject* KeyPressedEventObject_GetRepeated(KeyPressedEventObject* self, void* closure);
PyObject* KeyPressedEventObject_GetCapital(KeyPressedEventObject* self, void* closure);
PyObject* KeyPressedEventObject_GetCapslockOn(KeyPressedEventObject* self, void* closure);

static PyGetSetDef KeyPressedEventObjectGettersSetters[] = {
	{ "keycode", (getter)KeyPressedEventObject_GetKeycode, (setter)0, "keycode", NULL },
	{ "repeated", (getter)KeyPressedEventObject_GetRepeated, (setter)0, "repeated", NULL },
	{ "capital", (getter)KeyPressedEventObject_GetCapital, (setter)0, "capital", NULL },
	{ "capslock_on", (getter)KeyPressedEventObject_GetCapslockOn, (setter)0, "capslock_on", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject KeyPressedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"KeyPressedEvent", /*tp_name*/
	sizeof(KeyPressedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	KeyPressedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)KeyPressedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	KeyPressedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& KeyPressedEventObject_GetType();