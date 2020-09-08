#include "bindings_mousemovedevent.h"

PyTypeObject& MouseMovedEventObject_GetType()
{
	return MouseMovedEventObject_Type;
}

PyObject* MouseMovedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MouseMovedEventObject* self = (MouseMovedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int MouseMovedEventObject_Init(MouseMovedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* MouseMovedEventObject_GetButton(MouseMovedEventObject* self, void* closure)
{
	return Py_BuildValue("i", (int)self->handle.button);
}

PyObject* MouseMovedEventObject_GetLocation(MouseMovedEventObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle.location.x, self->handle.location.y);
}

PyObject* MouseMovedEventObject_GetDistance(MouseMovedEventObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle.distance.x, self->handle.distance.y);
}

//================================= //
//============ Setters ============ //
//================================= //

