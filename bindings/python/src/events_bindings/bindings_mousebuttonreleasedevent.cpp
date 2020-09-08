#include "bindings_mousebuttonreleasedevent.h"

PyTypeObject& MouseButtonReleasedEventObject_GetType()
{	
    return MouseButtonReleasedEventObject_Type;
}

PyObject* MouseButtonReleasedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MouseButtonReleasedEventObject* self = (MouseButtonReleasedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int MouseButtonReleasedEventObject_Init(MouseButtonReleasedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* MouseButtonReleasedEventObject_GetButton(MouseButtonReleasedEventObject* self, void* closure)
{
	return Py_BuildValue("i", (int)self->handle.button);
}

PyObject* MouseButtonReleasedEventObject_GetLocation(MouseButtonReleasedEventObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle.location.x, self->handle.location.y);
}

//================================= //
//============ Setters ============ //
//================================= //

