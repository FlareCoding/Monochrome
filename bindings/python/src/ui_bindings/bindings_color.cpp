#include "bindings_color.h"

PyTypeObject& ColorObject_GetType()
{
	return ColorObject_Type;
}

PyObject* ColorObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    ColorObject* self = (ColorObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	uint32_t r, g, b;
	float alpha = 1.0f;
	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "III|fK", &r, &g, &b, &alpha, &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<Color>((Color*)native_ptr);
	else
		self->handle = MakeRef<Color>(r, g, b, alpha);

    return (PyObject*)self;
}

int ColorObject_Init(ColorObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* ColorObject_GetR(ColorObject* self, void* closure)
{
	return Py_BuildValue("I", self->handle->r);
}

PyObject* ColorObject_GetG(ColorObject* self, void* closure)
{
	return Py_BuildValue("I", self->handle->g);
}

PyObject* ColorObject_GetB(ColorObject* self, void* closure)
{
	return Py_BuildValue("I", self->handle->b);
}

PyObject* ColorObject_GetAlpha(ColorObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->alpha);
}

//================================= //
//============ Setters ============ //
//================================= //

int ColorObject_SetR(ColorObject* self, PyObject* value, void* closure)
{
	unsigned int val;
	if (!PyArg_Parse(value, "I", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a uint32_t");
        return -1;
    }

	self->handle->r = val;
	return 0;
}

int ColorObject_SetG(ColorObject* self, PyObject* value, void* closure)
{
	unsigned int val;
	if (!PyArg_Parse(value, "I", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a uint32_t");
        return -1;
    }

	self->handle->g = val;
	return 0;
}

int ColorObject_SetB(ColorObject* self, PyObject* value, void* closure)
{
	unsigned int val;
	if (!PyArg_Parse(value, "I", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a uint32_t");
        return -1;
    }

	self->handle->b= val;
	return 0;
}

int ColorObject_SetAlpha(ColorObject* self, PyObject* value, void* closure)
{
	float val = 1.0f;
	if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a float");
        return -1;
    }

	self->handle->alpha = val;
	return 0;
}

