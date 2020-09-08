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
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<Frame>((Frame*)native_ptr);
	else
		self->handle = MakeRef<Frame>();

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
	return PyObject_CallFunction(
		(PyObject*)&PointObject_GetType(), 
		"ffK", 
		0.0f, 0.0f,
		(unsigned long long)&self->handle->position
	);
}

PyObject* FrameObject_GetSize(FrameObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&PointObject_GetType(), 
		"ffK", 
		0.0f, 0.0f,
		(unsigned long long)&self->handle->size
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int FrameObject_SetPosition(FrameObject* self, PyObject* value, void* closure)
{
	PointObject* obj;
    if (!PyArg_Parse(value, "O", &obj))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->position = *obj->handle.get();
	return 0;
}

int FrameObject_SetSize(FrameObject* self, PyObject* value, void* closure)
{
	PointObject* obj;
    if (!PyArg_Parse(value, "O", &obj))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->size = *obj->handle.get();
	return 0;
}

