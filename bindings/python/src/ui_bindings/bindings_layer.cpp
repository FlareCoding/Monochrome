#include "bindings_layer.h"

PyTypeObject& LayerObject_GetType()
{
	return LayerObject_Type;
}

PyObject* LayerObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    LayerObject* self = (LayerObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	self->handle = Ref<Layer>((Layer*)native_ptr);
    return (PyObject*)self;
}

int LayerObject_Init(LayerObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* LayerObject_GetColor(LayerObject* self, void* closure)
{
	return Py_BuildValue(
		"(I, I, I, f)",
		self->handle->color.r,
		self->handle->color.g,
		self->handle->color.b,
		self->handle->color.alpha
	);
}

PyObject* LayerObject_GetFrame(LayerObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&FrameObject_GetType(), 
		"K", 
		(unsigned long long)&self->handle->frame
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int LayerObject_SetColor(LayerObject* self, PyObject* value, void* closure)
{
	Color color = self->handle->color;
	if (!PyArg_ParseTuple(value, "III|f", &color.r, &color.g, &color.b, &color.alpha))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Parameters: (int, int, int, [optional]float)");
        return -1;
    }

	self->handle->color = color;
	return 0;
}
