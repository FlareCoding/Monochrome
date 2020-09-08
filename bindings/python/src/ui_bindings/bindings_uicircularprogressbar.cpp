#include "bindings_uicircularprogressbar.h"

PyTypeObject& UICircularProgressBarObject_GetType()
{
	return UICircularProgressBarObject_Type;
}

PyObject* UICircularProgressBarObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UICircularProgressBarObject* self = (UICircularProgressBarObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UICircularProgressBar>((UICircularProgressBar*)native_ptr);
	else
		self->handle = MakeRef<UICircularProgressBar>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UICircularProgressBarObject_Init(UICircularProgressBarObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UICircularProgressBarObject_AddValueChangedEventHandler(UICircularProgressBarObject* self, PyObject* args)
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

    self->handle->AddValueChangedEventHandler([callback, self](float value, UICircularProgressBar* sender) {
		UICircularProgressBarObject* pysender = (UICircularProgressBarObject*)PyObject_CallFunction(
			(PyObject*)&UICircularProgressBarObject_GetType(), 
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

PyObject* UICircularProgressBarObject_GetStroke(UICircularProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Stroke);
}

PyObject* UICircularProgressBarObject_GetProgressColor(UICircularProgressBarObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->ProgressColor
	);
}

PyObject* UICircularProgressBarObject_GetMaxValue(UICircularProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MaxValue);
}

PyObject* UICircularProgressBarObject_GetMinValue(UICircularProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MinValue);
}

PyObject* UICircularProgressBarObject_GetValue(UICircularProgressBarObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Value);
}

//================================= //
//============ Setters ============ //
//================================= //

int UICircularProgressBarObject_SetStroke(UICircularProgressBarObject* self, PyObject* value, void* closure)
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

int UICircularProgressBarObject_SetProgressColor(UICircularProgressBarObject* self, PyObject* value, void* closure)
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

int UICircularProgressBarObject_SetMaxValue(UICircularProgressBarObject* self, PyObject* value, void* closure)
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

int UICircularProgressBarObject_SetMinValue(UICircularProgressBarObject* self, PyObject* value, void* closure)
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

int UICircularProgressBarObject_SetValue(UICircularProgressBarObject* self, PyObject* value, void* closure)
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

