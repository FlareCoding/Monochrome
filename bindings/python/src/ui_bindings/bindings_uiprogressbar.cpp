#include "bindings_uiprogressbar.h"

PyTypeObject& UIProgressBarObject_GetType()
{
	return UIProgressBarObject_Type;
}

PyObject* UIProgressBarObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIProgressBarObject* self = (UIProgressBarObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UIProgressBar>((UIProgressBar*)native_ptr);
	else
		self->handle = MakeRef<UIProgressBar>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UIProgressBarObject_Init(UIProgressBarObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIProgressBarObject_AddValueChangedEventHandler(UIProgressBarObject* self, PyObject* args)
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

    self->handle->AddValueChangedEventHandler([callback, self](float value, UIProgressBar* sender) {
		UIProgressBarObject* pysender = (UIProgressBarObject*)PyObject_CallFunction(
			(PyObject*)&UIProgressBarObject_GetType(), 
			"K", 
			(unsigned long long)sender
		);

		PyObject* argslist = Py_BuildValue("(fO)", value, pysender);
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

PyObject* UIProgressBarObject_GetFilled(UIProgressBarObject* self, void* closure)
{
	return self->handle->Filled ? Py_True : Py_False;
}

PyObject* UIProgressBarObject_GetStroke(UIProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Stroke);
}

PyObject* UIProgressBarObject_GetProgressColor(UIProgressBarObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->ProgressColor
	);
}

PyObject* UIProgressBarObject_GetMaxValue(UIProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MaxValue);
}

PyObject* UIProgressBarObject_GetMinValue(UIProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MinValue);
}

PyObject* UIProgressBarObject_GetValue(UIProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Value);
}

//================================= //
//============ Setters ============ //
//================================= //

int UIProgressBarObject_SetFilled(UIProgressBarObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool filled = PyObject_IsTrue(value);
	self->handle->Filled = filled;
	return 0;
}

int UIProgressBarObject_SetStroke(UIProgressBarObject* self, PyObject* value, void* closure)
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

int UIProgressBarObject_SetProgressColor(UIProgressBarObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->ProgressColor = *color->handle.get();
	return 0;
}

int UIProgressBarObject_SetMaxValue(UIProgressBarObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->MaxValue = val;
	return 0;
}

int UIProgressBarObject_SetMinValue(UIProgressBarObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->MinValue = val;
	return 0;
}

int UIProgressBarObject_SetValue(UIProgressBarObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Value = val;
	return 0;
}

