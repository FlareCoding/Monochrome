#include "bindings_uilabel.h"

PyTypeObject& UILabelObject_GetType()
{
	return UILabelObject_Type;
}

PyObject* UILabelObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UILabelObject* self = (UILabelObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UILabel>((UILabel*)native_ptr);
	else
		self->handle = MakeRef<UILabel>(Frame(70, 80, 180, 40));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UILabelObject_Init(UILabelObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UILabelObject_GetText(UILabelObject* self, void* closure)
{
	return Py_BuildValue("s", self->handle->Text.c_str());
}

PyObject* UILabelObject_GetProperties(UILabelObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&TextPropertiesObject_GetType(), 
		"K", 
		(unsigned long long)&self->handle->Properties
	);
}

PyObject* UILabelObject_GetColor(UILabelObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->color
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int UILabelObject_SetText(UILabelObject* self, PyObject* value, void* closure)
{
    char* text = (char*)"";
    if (!PyArg_Parse(value, "s", &text))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a string");
        return -1;
    }

	self->handle->Text = text;
	return 0;
}

int UILabelObject_SetColor(UILabelObject* self, PyObject* value, void* closure)
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

