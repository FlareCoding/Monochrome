#include "bindings_mousehoveroffevent.h"

PyTypeObject& MouseHoverOffEventObject_GetType()
{
	return MouseHoverOffEventObject_Type;
}

PyObject* MouseHoverOffEventObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MouseHoverOffEventObject* self = (MouseHoverOffEventObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int MouseHoverOffEventObject_Init(MouseHoverOffEventObject* type, PyObject* args, PyObject* kwds)
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

