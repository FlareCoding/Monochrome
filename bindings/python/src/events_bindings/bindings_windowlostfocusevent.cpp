#include "bindings_windowlostfocusevent.h"

PyTypeObject& WindowLostFocusEventObject_GetType()
{
	return WindowLostFocusEventObject_Type;
}

PyObject* WindowLostFocusEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    WindowLostFocusEventObject* self = (WindowLostFocusEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int WindowLostFocusEventObject_Init(WindowLostFocusEventObject* type, PyObject* args, PyObject* kwds)
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

