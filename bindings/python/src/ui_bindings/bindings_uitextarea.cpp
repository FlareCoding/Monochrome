#include "bindings_uitextarea.h"

PyTypeObject& UITextAreaObject_GetType()
{
	return UITextAreaObject_Type;
}

PyObject* UITextAreaObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UITextAreaObject* self = (UITextAreaObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UITextArea>((UITextArea*)native_ptr);
	else
		self->handle = MakeRef<UITextArea>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UITextAreaObject_Init(UITextAreaObject* type, PyObject* args, PyObject* kwds)
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

PyObject* UITextAreaObject_GetText(UITextAreaObject* self, void* closure)
{
	return Py_BuildValue("s", self->handle->Text.c_str());
}

PyObject* UITextAreaObject_GetProperties(UITextAreaObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&TextPropertiesObject_GetType(), 
		"K", 
		(unsigned long long)&self->handle->Properties
	);
}

PyObject* UITextAreaObject_GetTextColor(UITextAreaObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->TextColor
	);
}

PyObject* UITextAreaObject_GetTopMargins(UITextAreaObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->TopMargins);
}

PyObject* UITextAreaObject_GetBottomMargins(UITextAreaObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->BottomMargins);
}

PyObject* UITextAreaObject_GetLeftMargins(UITextAreaObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->LeftMargins);
}

PyObject* UITextAreaObject_GetRightMargins(UITextAreaObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->RightMargins);
}

PyObject* UITextAreaObject_GetAutoScroll(UITextAreaObject* self, void* closure)
{
	return self->handle->AutoScroll ? Py_True : Py_False;
}

//================================= //
//============ Setters ============ //
//================================= //

int UITextAreaObject_SetText(UITextAreaObject* self, PyObject* value, void* closure)
{
	char* text;
    if (!PyArg_Parse(value, "s", &text))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a string");
        return -1;
    }

	self->handle->Text = text;
	return 0;
}

int UITextAreaObject_SetTextColor(UITextAreaObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->TextColor = *color->handle.get();
	return 0;
}

int UITextAreaObject_SetTopMargins(UITextAreaObject* self, PyObject* value, void* closure)
{
	float margins;
    if (!PyArg_Parse(value, "f", &margins))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->TopMargins = margins;
	return 0;
}

int UITextAreaObject_SetBottomMargins(UITextAreaObject* self, PyObject* value, void* closure)
{
	float margins;
    if (!PyArg_Parse(value, "f", &margins))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->BottomMargins = margins;
	return 0;
}

int UITextAreaObject_SetLeftMargins(UITextAreaObject* self, PyObject* value, void* closure)
{
	float margins;
    if (!PyArg_Parse(value, "f", &margins))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->LeftMargins = margins;
	return 0;
}

int UITextAreaObject_SetRightMargins(UITextAreaObject* self, PyObject* value, void* closure)
{
	float margins;
    if (!PyArg_Parse(value, "f", &margins))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->RightMargins = margins;
	return 0;
}

int UITextAreaObject_SetAutoScroll(UITextAreaObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool state = PyObject_IsTrue(value);
	self->handle->AutoScroll = state;
	return 0;
}

