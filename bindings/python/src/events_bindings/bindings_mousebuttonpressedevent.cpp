#include "bindings_mousebuttonpressedevent.h"

PyTypeObject& MouseButtonPressedEventObject_GetType()
{
    return MouseButtonPressedEventObject_Type;
}

PyObject* MouseButtonPressedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MouseButtonPressedEventObject* self = (MouseButtonPressedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int MouseButtonPressedEventObject_Init(MouseButtonPressedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* MouseButtonPressedEventObject_GetButton(MouseButtonPressedEventObject* self, void* closure)
{
	return Py_BuildValue("i", (int)self->handle.button);
}

PyObject* MouseButtonPressedEventObject_GetLocation(MouseButtonPressedEventObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle.location.x, self->handle.location.y);
}

//================================= //
//============ Setters ============ //
//================================= //

