#include "bindings_point.h"

PyTypeObject& PointObject_GetType()
{
	return PointObject_Type;
}

PyObject* PointObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    PointObject* self = (PointObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	float x = 0, y = 0;
	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "ff|K", &x, &y, &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<Point>((Point*)native_ptr);
	else
	{
        self->handle = MakeRef<Point>();
        self->handle->x = x;
        self->handle->y = y;
    }

    return (PyObject*)self;
}

int PointObject_Init(PointObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* PointObject_GetX(PointObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->x);
}

PyObject* PointObject_GetY(PointObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->y);
}

//================================= //
//============ Setters ============ //
//================================= //

int PointObject_SetX(PointObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a float");
        return -1;
    }

	self->handle->x = val;
	return 0;
}

int PointObject_SetY(PointObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. Must be a float");
        return -1;
    }

	self->handle->y = val;
	return 0;
}

