#include "bindings_windowclosedevent.h"

PyTypeObject& WindowClosedEventObject_GetType()
{
	return WindowClosedEventObject_Type;
}

PyObject* WindowClosedEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    WindowClosedEventObject* self = (WindowClosedEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int WindowClosedEventObject_Init(WindowClosedEventObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

//================================= //
//============ Setters ============ //
//================================= //

