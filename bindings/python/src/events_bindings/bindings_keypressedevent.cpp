#include "bindings_keypressedevent.h"

PyTypeObject& KeyPressedEventObject_GetType()
{
	return KeyPressedEventObject_Type;
}

PyObject* KeyPressedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    KeyPressedEventObject* self = (KeyPressedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int KeyPressedEventObject_Init(KeyPressedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* KeyPressedEventObject_GetKeycode(KeyPressedEventObject* self, void* closure)
{
	return Py_BuildValue("i", self->handle.keycode);
}

PyObject* KeyPressedEventObject_GetRepeated(KeyPressedEventObject* self, void* closure)
{
	return (self->handle.repeated) ? Py_True : Py_False;
}

PyObject* KeyPressedEventObject_GetCapital(KeyPressedEventObject* self, void* closure)
{
	return (self->handle.capital) ? Py_True : Py_False;
}

PyObject* KeyPressedEventObject_GetCapslockOn(KeyPressedEventObject* self, void* closure)
{
	return (self->handle.capslock_on) ? Py_True : Py_False;
}

//================================= //
//============ Setters ============ //
//================================= //

