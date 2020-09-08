#pragma once
#include <Python.h>
#include <Monochrome.h>
using namespace mc;

typedef struct _WindowUpdatedEventObject {
	PyObject_HEAD
	WindowUpdatedEvent handle = WindowUpdatedEvent(nullptr, 0, 0);
} WindowUpdatedEventObject;

PyObject* WindowUpdatedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int WindowUpdatedEventObject_Init(WindowUpdatedEventObject* type, PyObject* args, PyObject* kwds);

//============ Getters ============ //
PyObject* WindowUpdatedEventObject_GetEventProcessingTime(WindowUpdatedEventObject* self, void* closure);
PyObject* WindowUpdatedEventObject_GetGraphicsRenderingTime(WindowUpdatedEventObject* self, void* closure);

static PyGetSetDef WindowUpdatedEventObjectGettersSetters[] = {
	{ "event_processing_time", (getter)WindowUpdatedEventObject_GetEventProcessingTime, (setter)0, "event_processing_time", NULL },
	{ "graphics_rendering_time", (getter)WindowUpdatedEventObject_GetGraphicsRenderingTime, (setter)0, "graphics_rendering_time", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject WindowUpdatedEventObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"WindowUpdatedEvent", /*tp_name*/
	sizeof(WindowUpdatedEventObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	0, /*tp_methods*/
	0, /*tp_members*/
	WindowUpdatedEventObjectGettersSetters, /*tp_getset*/
	0, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)WindowUpdatedEventObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	WindowUpdatedEventObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& WindowUpdatedEventObject_GetType();