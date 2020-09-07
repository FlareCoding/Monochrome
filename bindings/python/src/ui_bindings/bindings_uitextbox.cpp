#include "bindings_uitextbox.h"

PyTypeObject& UITextboxObject_GetType()
{
	return UITextboxObject_Type;
}

PyObject* UITextboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UITextboxObject* self = (UITextboxObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UITextbox>((UITextbox*)native_ptr);
	else
		self->handle = MakeRef<UITextbox>(Frame(70, 80, 180, 40));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UITextboxObject_Init(UITextboxObject* type, PyObject* args, PyObject* kwds)
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

PyObject* UITextboxObject_GetStroke(UITextboxObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Stroke);
}

PyObject* UITextboxObject_GetFocusedHighlightColor(UITextboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->FocusedHighlightColor
	);
}

PyObject* UITextboxObject_GetText(UITextboxObject* self, void* closure)
{
	return Py_BuildValue("s", self->handle->Text.c_str());
}

PyObject* UITextboxObject_GetPlaceholder(UITextboxObject* self, void* closure)
{
	return Py_BuildValue("s", self->handle->Placeholder.c_str());
}

PyObject* UITextboxObject_GettextProperties(UITextboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&TextPropertiesObject_GetType(), 
		"K", 
		(unsigned long long)&self->handle->textProperties
	);
}

PyObject* UITextboxObject_GetTextColor(UITextboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->TextColor
	);
}

PyObject* UITextboxObject_GetReadOnly(UITextboxObject* self, void* closure)
{
	return self->handle->ReadOnly ? Py_True : Py_False;
}

//================================= //
//============ Setters ============ //
//================================= //

int UITextboxObject_SetStroke(UITextboxObject* self, PyObject* value, void* closure)
{
	float stroke = 2.0f;
    if (!PyArg_Parse(value, "f", &stroke))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Stroke = stroke;
	return 0;
}

int UITextboxObject_SetFocusedHighlightColor(UITextboxObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->FocusedHighlightColor = *color->handle.get();
	return 0;
}

int UITextboxObject_SetText(UITextboxObject* self, PyObject* value, void* closure)
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

int UITextboxObject_SetPlaceholder(UITextboxObject* self, PyObject* value, void* closure)
{
	char* text = (char*)"";
    if (!PyArg_Parse(value, "s", &text))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a string");
        return -1;
    }

	self->handle->Placeholder = text;
	return 0;
}

int UITextboxObject_SetTextColor(UITextboxObject* self, PyObject* value, void* closure)
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

int UITextboxObject_SetReadOnly(UITextboxObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool filled = PyObject_IsTrue(value);
	self->handle->ReadOnly = filled;
	return 0;
}

