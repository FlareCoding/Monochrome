#include "bindings_uiimage.h"

PyTypeObject& UIImageObject_GetType()
{
	return UIImageObject_Type;
}

PyObject* UIImageObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIImageObject* self = (UIImageObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UIImage>((UIImage*)native_ptr);
	else
		self->handle = MakeRef<UIImage>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UIImageObject_Init(UIImageObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIImageObject_LoadImageFromFile(UIImageObject* self, PyObject* args)
{
	char* filepath;
    if (!PyArg_ParseTuple(args, "s", &filepath))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	bool result = self->handle->LoadImageFromFile(filepath);
	return result ? Py_True : Py_False;
}

PyObject* UIImageObject_LoadWebImage(UIImageObject* self, PyObject* args)
{
	char* url;
    if (!PyArg_ParseTuple(args, "s", &url))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	bool result = self->handle->LoadWebImage(url);
	return result ? Py_True : Py_False;
}

PyObject* UIImageObject_IsImageLoaded(UIImageObject* self, PyObject* args)
{
	return self->handle->IsImageLoaded() ? Py_True : Py_False;
}

PyObject* UIImageObject_GetWidth(UIImageObject* self, PyObject* args)
{
	return Py_BuildValue("f", self->handle->GetWidth());
}

PyObject* UIImageObject_GetHeight(UIImageObject* self, PyObject* args)
{
	return Py_BuildValue("f", self->handle->GetHeight());
}

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UIImageObject_GetOpacity(UIImageObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Opacity);
}

//================================= //
//============ Setters ============ //
//================================= //

int UIImageObject_SetOpacity(UIImageObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Opacity = val;
	return 0;
}

