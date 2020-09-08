#include "bindings_mousehoveronevent.h"

PyTypeObject& MouseHoverOnEventObject_GetType()
{
	return MouseHoverOnEventObject_Type;
}

PyObject* MouseHoverOnEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MouseHoverOnEventObject* self = (MouseHoverOnEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int MouseHoverOnEventObject_Init(MouseHoverOnEventObject* type, PyObject* args, PyObject* kwds)
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

