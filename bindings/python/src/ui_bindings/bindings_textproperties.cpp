#include "bindings_textproperties.h"

PyTypeObject& TextPropertiesObject_GetType()
{
	return TextPropertiesObject_Type;
}

PyObject* TextPropertiesObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    TextPropertiesObject* self = (TextPropertiesObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	self->handle = Ref<TextProperties>((TextProperties*)native_ptr);
    return (PyObject*)self;
}

int TextPropertiesObject_Init(TextPropertiesObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* TextPropertiesObject_GetFont(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("s", self->handle->Font.c_str());
}

PyObject* TextPropertiesObject_GetFontSize(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("I", self->handle->FontSize);
}

PyObject* TextPropertiesObject_GetAlignment(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("I", (uint32_t)self->handle->Alignment);
}

PyObject* TextPropertiesObject_GetVerticalAlignment(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("I", (uint32_t)self->handle->VerticalAlignment);
}

PyObject* TextPropertiesObject_GetStyle(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("I", (uint32_t)self->handle->Style);
}

PyObject* TextPropertiesObject_GetWrapping(TextPropertiesObject* self, void* closure)
{
	return Py_BuildValue("I", (uint32_t)self->handle->Wrapping);
}

//================================= //
//============ Setters ============ //
//================================= //

int TextPropertiesObject_SetFont(TextPropertiesObject* self, PyObject* value, void* closure)
{
	char* font = (char*)"Arial";
	if (!PyArg_Parse(value, "s", &font))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a string");
		return -1;
	}

	self->handle->Font = std::string(font);
	return 0;
}

int TextPropertiesObject_SetFontSize(TextPropertiesObject* self, PyObject* value, void* closure)
{
	unsigned int size = 0;
	if (!PyArg_Parse(value, "I", &size))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an unsigned integer");
		return -1;
	}

	self->handle->FontSize = size;
	return 0;
}

int TextPropertiesObject_SetAlignment(TextPropertiesObject* self, PyObject* value, void* closure)
{
	unsigned int val = 0;
	if (!PyArg_Parse(value, "I", &val))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an unsigned integer");
		return -1;
	}

	self->handle->Alignment = (TextAlignment)val;
	return 0;
}

int TextPropertiesObject_SetVerticalAlignment(TextPropertiesObject* self, PyObject* value, void* closure)
{
	unsigned int val = 0;
	if (!PyArg_Parse(value, "I", &val))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an unsigned integer");
		return -1;
	}

	self->handle->VerticalAlignment = (TextAlignment)val;
	return 0;
}

int TextPropertiesObject_SetStyle(TextPropertiesObject* self, PyObject* value, void* closure)
{
	unsigned int val = 0;
	if (!PyArg_Parse(value, "I", &val))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an unsigned integer");
		return -1;
	}

	self->handle->Style = (TextStyle)val;
	return 0;
}

int TextPropertiesObject_SetWrapping(TextPropertiesObject* self, PyObject* value, void* closure)
{
	unsigned int val = 0;
	if (!PyArg_Parse(value, "I", &val))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an unsigned integer");
		return -1;
	}

	self->handle->Wrapping = (WordWrapping)val;
	return 0;
}

