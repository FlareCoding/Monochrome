#include "bindings_frame.h"

PyTypeObject& FrameObject_GetType()
{
	return FrameObject_Type;
}

PyObject* FrameObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    FrameObject* self = (FrameObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	self->handle = Ref<Frame>((Frame*)native_ptr);
    return (PyObject*)self;
}

int FrameObject_Init(FrameObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* FrameObject_GetPosition(FrameObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle->position.x, self->handle->position.y);
}

PyObject* FrameObject_GetSize(FrameObject* self, void* closure)
{
	return Py_BuildValue("(f, f)", self->handle->size.x, self->handle->size.y);
}

//================================= //
//============ Setters ============ //
//================================= //

int FrameObject_SetPosition(FrameObject* self, PyObject* value, void* closure)
{
	Position position = self->handle->position;
	if (!PyArg_ParseTuple(value, "ff", &position.x, &position.y))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Parameters: (float, float)");
        return -1;
    }

	self->handle->position = position;
	return 0;
}

int FrameObject_SetSize(FrameObject* self, PyObject* value, void* closure)
{
	Size size = self->handle->size;
	if (!PyArg_ParseTuple(value, "ff", &size.x, &size.y))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Parameters: (float, float)");
        return -1;
    }

	self->handle->size = size;
	return 0;
}

