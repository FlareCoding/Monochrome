#include "bindings_keyreleasedevent.h"

PyTypeObject& KeyReleasedEventObject_GetType()
{
	return KeyReleasedEventObject_Type;
}

PyObject* KeyReleasedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    KeyReleasedEventObject* self = (KeyReleasedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int KeyReleasedEventObject_Init(KeyReleasedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* KeyReleasedEventObject_GetKeycode(KeyReleasedEventObject* self, void* closure)
{
	return Py_BuildValue("i", self->handle.keycode);
}

//================================= //
//============ Setters ============ //
//================================= //

