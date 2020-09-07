#include "bindings_uicheckbox.h"

PyTypeObject& UICheckboxObject_GetType()
{
	return UICheckboxObject_Type;
}

PyObject* UICheckboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UICheckboxObject* self = (UICheckboxObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UICheckbox>((UICheckbox*)native_ptr);
	else
		self->handle = MakeRef<UICheckbox>(Frame(70, 80, 180, 40));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UICheckboxObject_Init(UICheckboxObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UICheckboxObject_Toggle(UICheckboxObject* self, PyObject* args)
{
	self->handle->Toggle();
	return Py_None;
}

PyObject* UICheckboxObject_AddValueChangedEventHandler(UICheckboxObject* self, PyObject* args)
{
	PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:ValueChangedEventHandler", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    self->handle->AddValueChangedEventHandler([callback, self](bool checked, UICheckbox* sender) {
		UIViewObject* pysender = (UIViewObject*)PyObject_CallFunction(
			(PyObject*)&UICheckboxObject_GetType(), 
			"K", 
			(unsigned long long)sender
		);

		PyObject* argslist = Py_BuildValue("(OO)", checked ? Py_True : Py_False, pysender);
		PyObject* result = PyEval_CallObject(callback, argslist);
		Py_DECREF(argslist);
		
		if (!result)
		{
			PyErr_SetString(PyExc_Exception, "Error calling event handler");
			PyErr_Occurred();
		}

		Py_XDECREF(result);
	});

    return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UICheckboxObject_GetChecked(UICheckboxObject* self, void* closure)
{
	return self->handle->Checked ? Py_True : Py_False;
}

PyObject* UICheckboxObject_GetBoxSize(UICheckboxObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->BoxSize);
}

PyObject* UICheckboxObject_GetLabelMargins(UICheckboxObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->LabelMargins);
}

PyObject* UICheckboxObject_GetCheckmarkColor(UICheckboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->CheckmarkColor
	);
}

PyObject* UICheckboxObject_GetCheckedBoxColor(UICheckboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->CheckedBoxColor
	);
}

PyObject* UICheckboxObject_GetLabel(UICheckboxObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&UILabelObject_GetType(),
		"K",
		(unsigned long long)self->handle->Label.get()
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int UICheckboxObject_SetChecked(UICheckboxObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool checked = PyObject_IsTrue(value);
	self->handle->Checked = checked;
	return 0;
}

int UICheckboxObject_SetBoxSize(UICheckboxObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->BoxSize = val;
	return 0;
}

int UICheckboxObject_SetLabelMargins(UICheckboxObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->LabelMargins = val;
	return 0;
}

int UICheckboxObject_SetCheckmarkColor(UICheckboxObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->CheckmarkColor = *color->handle.get();
	return 0;
}

int UICheckboxObject_SetCheckedBoxColor(UICheckboxObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->CheckedBoxColor = *color->handle.get();
	return 0;
}

