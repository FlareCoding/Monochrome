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
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<Layer>((Layer*)native_ptr);
	else
		self->handle = MakeRef<Layer>();
	
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
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->color
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
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->color = *color->handle.get();
	return 0;
}

int LayerObject_SetFrame(LayerObject* self, PyObject* value, void* closure)
{
	FrameObject* obj;
    if (!PyArg_Parse(value, "O", &obj))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->frame = *obj->handle.get();
	return 0;
}
