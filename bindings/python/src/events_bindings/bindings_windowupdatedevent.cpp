#include "bindings_windowupdatedevent.h"

PyTypeObject& WindowUpdatedEventObject_GetType()
{
	return WindowUpdatedEventObject_Type;
}

PyObject* WindowUpdatedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    WindowUpdatedEventObject* self = (WindowUpdatedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int WindowUpdatedEventObject_Init(WindowUpdatedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* WindowUpdatedEventObject_GetEventProcessingTime(WindowUpdatedEventObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle.EventProcessingTime);
}

PyObject* WindowUpdatedEventObject_GetGraphicsRenderingTime(WindowUpdatedEventObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle.GraphicsRenderingTime);
}

//================================= //
//============ Setters ============ //
//================================= //

