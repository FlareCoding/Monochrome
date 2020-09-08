#include "bindings_windowresizedevent.h"

PyTypeObject& WindowResizedEventObject_GetType()
{
	return WindowResizedEventObject_Type;
}

PyObject* WindowResizedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    WindowResizedEventObject* self = (WindowResizedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int WindowResizedEventObject_Init(WindowResizedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* WindowResizedEventObject_GetWidth(WindowResizedEventObject* self, void* closure)
{
	return Py_BuildValue("i", self->handle.width);
}

PyObject* WindowResizedEventObject_GetHeight(WindowResizedEventObject* self, void* closure)
{
	return Py_BuildValue("i", self->handle.height);
}

//================================= //
//============ Setters ============ //
//================================= //

