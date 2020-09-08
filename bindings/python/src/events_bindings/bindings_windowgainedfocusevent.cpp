#include "bindings_windowgainedfocusevent.h"

PyTypeObject& WindowGainedFocusEventObject_GetType()
{
	return WindowGainedFocusEventObject_Type;
}

PyObject* WindowGainedFocusEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    WindowGainedFocusEventObject* self = (WindowGainedFocusEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int WindowGainedFocusEventObject_Init(WindowGainedFocusEventObject* type, PyObject* args, PyObject* kwds)
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

