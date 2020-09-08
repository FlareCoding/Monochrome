#include "bindings_focuschangedevent.h"

PyTypeObject& FocusChangedEventObject_GetType()
{
	return FocusChangedEventObject_Type;
}

PyObject* FocusChangedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    FocusChangedEventObject* self = (FocusChangedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int FocusChangedEventObject_Init(FocusChangedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* FocusChangedEventObject_GetGainedFocus(FocusChangedEventObject* self, void* closure)
{
	return (self->handle) ? Py_True : Py_False;
}

//================================= //
//============ Setters ============ //
//================================= //

